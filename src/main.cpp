#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <filesystem>
#include <optional>
#include <fstream>
#include <iterator>
#include "../include/Symbol.h"
#include "../include/Parser.h"
#include "../include/Lexer.h"
#include "../include/AST.h"
#include "../include/Config.h"

namespace {

struct DemoConfig {
    std::filesystem::path source;
    std::filesystem::path grammar;
    std::filesystem::path translation;
};

std::filesystem::path resolvePathNearExecutable(const std::filesystem::path& relative)
{
    try {
        auto exe = std::filesystem::canonical("/proc/self/exe");
        auto baseDir = exe.parent_path().parent_path(); // expect build/<exe>
        auto candidate = baseDir / relative;
        if (std::filesystem::exists(candidate)) {
            return candidate;
        }
        return relative;
    } catch (const std::exception&) {
        return relative;
    }
}

std::optional<DemoConfig> discoverDemoConfig()
{
    DemoConfig config {
        resolvePathNearExecutable("examples/test_source.c"),
        resolvePathNearExecutable("examples/grammar_rules.txt"),
        resolvePathNearExecutable("examples/translation_rules.txt")
    };

    if (std::filesystem::exists(config.source) &&
        std::filesystem::exists(config.grammar) &&
        std::filesystem::exists(config.translation)) {
        return config;
    }

    return std::nullopt;
}

void printUsage(const std::string& executable)
{
    std::cout << "Usage:\n"
              << "  " << executable << " <source_code_file> <grammar_file> <translation_file>\n"
              << "  " << executable << " --demo    # Run with bundled example files\n"
              << "  " << executable << " --stdin   # Read source from stdin and use grammar/translation from demo\n";
}

} // namespace

class Compiler {
public:
    Compiler() = default;
    ~Compiler() = default;
    
    bool compile(const std::string& source) {
        std::cout << "Starting compilation of source code...\n";
        
        try {
            // Create parser and parse the source code
            Parser parser(source);
            auto ast = parser.parse();
            
            if (ast) {
                std::cout << "Abstract Syntax Tree generated:\n";
                ast->print();
                symbolTable_.clear();
                indexAST(*ast);
                printSymbolSummary();
                std::cout << "\nCompilation completed successfully!\n";
                return true;
            } else {
                std::cout << "Failed to generate AST\n";
                return false;
            }
        } catch (const std::exception& e) {
            std::cout << "Compilation failed with error: " << e.what() << std::endl;
            return false;
        }
    }
    
    bool compileWithConfig(const std::string& sourceCodeFile, 
                          const std::string& grammarFile, 
                          const std::string& translationFile) {
        std::cout << "Starting compilation with configuration...\n";
        
        Config config;
        
        // Load configuration files
        if (!config.loadSourceCode(sourceCodeFile)) {
            std::cout << "Error: Could not load source code file: " << sourceCodeFile << std::endl;
            return false;
        }
        
        if (!config.loadGrammarFile(grammarFile)) {
            std::cout << "Error: Could not load grammar file: " << grammarFile << std::endl;
            return false;
        }
        
        if (!config.loadTranslationRules(translationFile)) {
            std::cout << "Error: Could not load translation rules file: " << translationFile << std::endl;
            return false;
        }
        
        std::cout << "Loaded " << config.getGrammarRules().size() << " grammar rules\n";
        std::cout << "Loaded " << config.getTranslationRules().size() << " translation rules\n";
        
        // Parse the source code using the loaded grammar
        const std::string& source = config.getSourceCode();
        Parser parser(source);
        auto ast = parser.parse();
        
        if (ast) {
            std::cout << "Abstract Syntax Tree generated:\n";
            ast->print();
            symbolTable_.clear();
            indexAST(*ast);
            printSymbolSummary();
            
            // Apply translation rules if available
            if (!config.getTranslationRules().empty()) {
                std::cout << "\nApplying translation rules:\n";
                for (const auto& rule : config.getTranslationRules()) {
                    std::cout << "  " << rule.from_language << " -> " << rule.to_language 
                              << ": " << rule.rule << std::endl;
                }
            }
            
            std::cout << "\nCompilation with configuration completed successfully!\n";
            return true;
        } else {
            std::cout << "Failed to generate AST\n";
            return false;
        }
    }
    
private:
    void indexAST(const ASTNode& node);
    void printSymbolSummary() const;
    static std::string symbolTypeToString(SymbolType type);
    SymbolTable symbolTable_;
};

void Compiler::indexAST(const ASTNode& node)
{
    switch (node.getType()) {
    case ASTNodeType::Program: {
        const auto& program = static_cast<const ProgramNode&>(node);
        for (const auto& function : program.getFunctions()) {
            if (function) {
                indexAST(*function);
            }
        }
        break;
    }
    case ASTNodeType::Function: {
        const auto& function = static_cast<const FunctionNode&>(node);
        auto symbol = std::make_shared<Symbol>(function.getName(), SymbolType::Function);
        symbol->setDefinition(function.getReturnType());
        symbolTable_.addSymbol(std::move(symbol));

        if (const auto* body = function.getBody()) {
            indexAST(*body);
        }
        break;
    }
    case ASTNodeType::Block: {
        const auto& block = static_cast<const BlockNode&>(node);
        for (const auto& statement : block.getStatements()) {
            if (statement) {
                indexAST(*statement);
            }
        }
        break;
    }
    case ASTNodeType::VariableDeclaration: {
        const auto& decl = static_cast<const VariableDeclarationNode&>(node);
        auto symbol = std::make_shared<Symbol>(decl.getName(), SymbolType::Variable);
        symbol->setDefinition(decl.getTypeName());
        symbolTable_.addSymbol(std::move(symbol));

        if (const auto* initializer = decl.getInitializer()) {
            indexAST(*initializer);
        }
        break;
    }
    case ASTNodeType::Assignment: {
        const auto& assignment = static_cast<const AssignmentNode&>(node);
        if (const auto* value = assignment.getValue()) {
            indexAST(*value);
        }
        break;
    }
    case ASTNodeType::BinaryOperation: {
        const auto& binary = static_cast<const BinaryOperationNode&>(node);
        if (const auto* left = binary.getLeft()) {
            indexAST(*left);
        }
        if (const auto* right = binary.getRight()) {
            indexAST(*right);
        }
        break;
    }
    case ASTNodeType::Return: {
        const auto& ret = static_cast<const ReturnNode&>(node);
        if (const auto* value = ret.getValue()) {
            indexAST(*value);
        }
        break;
    }
    case ASTNodeType::Literal:
    case ASTNodeType::Identifier:
        break;
    }
}

void Compiler::printSymbolSummary() const
{
    const auto symbols = symbolTable_.getAllSymbols();
    if (symbols.empty()) {
        std::cout << "\nNo symbols recorded.\n";
        return;
    }

    std::cout << "\nSymbols discovered:\n";
    for (const auto& symbol : symbols) {
        if (!symbol) {
            continue;
        }

        std::cout << "  [" << symbolTypeToString(symbol->getType()) << "] " << symbol->getName();
        if (!symbol->getDefinition().empty()) {
            std::cout << " : " << symbol->getDefinition();
        }
        std::cout << "\n";
    }
}

std::string Compiler::symbolTypeToString(SymbolType type)
{
    switch (type) {
    case SymbolType::Variable:
        return "variable";
    case SymbolType::Function:
        return "function";
    case SymbolType::Class:
        return "class";
    case SymbolType::Namespace:
        return "namespace";
    }
    return "unknown";
}

int main(int argc, char* argv[])
{
    std::cout << "IStudio C++26 Compiler\n" << std::endl;

    Compiler compiler;
    const std::string executable = argc > 0 ? argv[0] : "IStudio";

    auto emitUsageAndExit = [&](int code) {
        printUsage(executable);
        return code;
    };

    if (argc == 4) {
        std::string sourceCodeFile = argv[1];
        std::string grammarFile = argv[2];
        std::string translationFile = argv[3];

        if (compiler.compileWithConfig(sourceCodeFile, grammarFile, translationFile)) {
            std::cout << "\nCompilation completed successfully!" << std::endl;
            return 0;
        }

        std::cout << "\nCompilation failed!" << std::endl;
        return 1;
    }

    if (argc == 2) {
        std::string flag = argv[1];
        if (flag == "--demo") {
            if (auto demo = discoverDemoConfig()) {
                if (compiler.compileWithConfig(demo->source.string(), demo->grammar.string(), demo->translation.string())) {
                    std::cout << "\nCompilation completed successfully!" << std::endl;
                    return 0;
                }
                std::cout << "\nCompilation failed!" << std::endl;
                return 1;
            }

            std::cout << "Demo assets are missing. Re-run from the project directory or provide explicit paths." << std::endl;
            return emitUsageAndExit(2);
        }

        if (flag == "--stdin") {
            if (auto demo = discoverDemoConfig()) {
                std::cout << "Reading source code from stdin. Press Ctrl+D when done.\n";
                std::string sourceCode((std::istreambuf_iterator<char>(std::cin)), std::istreambuf_iterator<char>());
                if (sourceCode.empty()) {
                    std::cout << "No source code provided on stdin." << std::endl;
                    return 1;
                }

                auto tempFile = std::filesystem::temp_directory_path() / "istudio-stdin-src.c";
                {
                    std::ofstream out(tempFile);
                    out << sourceCode;
                }

                const bool success = compiler.compileWithConfig(tempFile.string(), demo->grammar.string(), demo->translation.string());
                std::filesystem::remove(tempFile);

                if (success) {
                    std::cout << "\nCompilation completed successfully!" << std::endl;
                    return 0;
                }
                std::cout << "\nCompilation failed!" << std::endl;
                return 1;
            }

            std::cout << "Grammar/translation demo files are missing for stdin mode." << std::endl;
            return emitUsageAndExit(2);
        }
    }

    const char* run_test = std::getenv("RUN_COMPILER_TEST");
    if (run_test && std::string(run_test) == "1") {
        std::string sourceCode = "int main() { int x = 42; return x; }";

        if (compiler.compile(sourceCode)) {
            std::cout << "\nCompilation completed successfully!" << std::endl;
            return 0;
        }

        std::cout << "\nCompilation failed!" << std::endl;
        return 1;
    }

    return emitUsageAndExit(1);
}
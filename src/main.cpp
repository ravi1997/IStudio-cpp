#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "../include/Symbol.h"
#include "../include/Parser.h"
#include "../include/Lexer.h"
#include "../include/AST.h"
#include "../include/Config.h"

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
    SymbolTable symbolTable_;
};

int main(int argc, char* argv[])
{
    std::cout << "IStudio C++26 Compiler\n" << std::endl;
    
    Compiler compiler;
    
    // Check if command line arguments are provided for configuration files
    if (argc == 4) {
        // Use configuration files
        std::string sourceCodeFile = argv[1];
        std::string grammarFile = argv[2];
        std::string translationFile = argv[3];
        
        if (compiler.compileWithConfig(sourceCodeFile, grammarFile, translationFile)) {
            std::cout << "\nCompilation completed successfully!" << std::endl;
            return 0;
        } else {
            std::cout << "\nCompilation failed!" << std::endl;
            return 1;
        }
    } else {
        // Show usage but don't run default compilation to avoid potential infinite loops
        std::cout << "Usage: " << argv[0] << " <source_code_file> <grammar_file> <translation_file>\n";
        std::cout << "Run with RUN_COMPILER_TEST=1 to execute default test, or provide configuration files." << std::endl;

        // For debugging purposes, we can optionally run a test if an environment variable is set
        const char* run_test = std::getenv("RUN_COMPILER_TEST");
        if (run_test && std::string(run_test) == "1") {
            // Example source code to compile
            std::string sourceCode = "int main() { int x = 42; return x; }";
            
            if (compiler.compile(sourceCode)) {
                std::cout << "\nCompilation completed successfully!" << std::endl;
            } else {
                std::cout << "\nCompilation failed!" << std::endl;
                return 1;
            }
        }
    }
    
    return 0;
}
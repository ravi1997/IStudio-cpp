#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <filesystem>
#include <optional>
#include <fstream>
#include <iterator>
#include <string_view>
#include <utility>
#include <algorithm>
#include <sstream>
#include <cctype>
#include "../include/Symbol.h"
#include "../include/Parser.h"
#include "../include/AST.h"
#include "../include/Config.h"
#include "semantic/SemanticAnalyzer.h"
#include "istudio/Diagnostics.h"
#include "istudio/Lexer.h"
#include "istudio/Token.h"
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

} // namespace

namespace semantic = istudio::semantic;

namespace {

std::string_view severityToString(istudio::DiagnosticSeverity severity)
{
    switch (severity) {
    case istudio::DiagnosticSeverity::Info:
        return "info";
    case istudio::DiagnosticSeverity::Warning:
        return "warning";
    case istudio::DiagnosticSeverity::Error:
        return "error";
    case istudio::DiagnosticSeverity::Fatal:
        return "fatal";
    }
    return "unknown";
}

void printDiagnostics(const std::vector<istudio::Diagnostic>& diagnostics)
{
    for (const auto& diag : diagnostics) {
        std::cout << "[" << severityToString(diag.severity) << "] " << diag.message;
        if (diag.range) {
            const auto& range = *diag.range;
            std::cout << " (" << range.begin.line << ':' << range.begin.column;
            if (range.end.line != range.begin.line || range.end.column != range.begin.column) {
                std::cout << "-" << range.end.line << ':' << range.end.column;
            }
            std::cout << ")";
        }
        std::cout << std::endl;
    }
}

istudio::LexerOptions makeLexerOptions(const std::vector<GrammarRule>& rules)
{
    istudio::LexerOptions options;
    options.grammar.reserve(rules.size());
    for (const auto& rule : rules) {
        options.grammar.push_back({rule.pattern, rule.action});
    }
    return options;
}

istudio::PhaseResult<std::vector<istudio::Token>> lexSourceToTokens(
    std::string_view source,
    const istudio::LexerOptions& options)
{
    istudio::DiagnosticEngine diagnostics;
    istudio::Lexer lexer(source, options, diagnostics);
    auto tokensResult = lexer.tokenize();
    if (!tokensResult) {
        return std::unexpected(tokensResult.error());
    }

    std::vector<istudio::Token> filtered;
    filtered.reserve(tokensResult->size());
    for (auto& token : *tokensResult) {
        if (token.kind == istudio::TokenKind::EndOfFile ||
            token.kind == istudio::TokenKind::Comment ||
            token.kind == istudio::TokenKind::DocComment) {
            continue;
        }
        filtered.push_back(std::move(token));
    }

    return filtered;
}

bool lexDirectoryWithGrammar(const std::filesystem::path& samplesDir,
                             const std::filesystem::path& grammarFile)
{
    Config config;
    if (!config.loadGrammarFile(grammarFile.string())) {
        std::cout << "Error: Could not load grammar file: " << grammarFile << std::endl;
        return false;
    }

    const auto options = makeLexerOptions(config.getGrammarRules());
    bool allSucceeded = true;

    if (!std::filesystem::exists(samplesDir)) {
        std::cout << "Error: Samples directory not found: " << samplesDir << std::endl;
        return false;
    }

    std::cout << "Lexing samples in " << samplesDir << std::endl;

    std::vector<std::filesystem::path> files;
    for (const auto& entry : std::filesystem::directory_iterator(samplesDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".ipl") {
            files.push_back(entry.path());
        }
    }

    std::sort(files.begin(), files.end());

    for (const auto& file : files) {
        std::ifstream in(file);
        if (!in) {
            std::cout << "  " << file.filename().string() << ": unable to read" << std::endl;
            allSucceeded = false;
            continue;
        }

        std::string source((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        auto tokens = lexSourceToTokens(source, options);
        if (!tokens) {
            std::cout << "  " << file.filename().string() << ": lexing failed" << std::endl;
            printDiagnostics(tokens.error());
            allSucceeded = false;
            continue;
        }

        std::cout << "  " << file.filename().string() << ": " << tokens->size() << " tokens" << std::endl;
    }

    return allSucceeded;
}

bool loadStandardLibraryTokens(const istudio::LexerOptions& options,
                               std::size_t& totalTokens,
                               bool verbose)
{
    const auto stdlibDir = resolvePathNearExecutable("stdlib");
    if (!std::filesystem::exists(stdlibDir)) {
        if (verbose) {
            std::cout << "Warning: Standard library directory not found at " << stdlibDir << std::endl;
        }
        return true;
    }

    std::vector<std::filesystem::path> files;
    for (const auto& entry : std::filesystem::directory_iterator(stdlibDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".ipl") {
            files.push_back(entry.path());
        }
    }

    std::sort(files.begin(), files.end());

    for (const auto& file : files) {
        if (verbose) {
            std::cout << "Loading stdlib file: " << file.filename().string() << std::endl;
        }

        std::ifstream in(file);
        if (!in) {
            std::cout << "Error: Could not read standard library file " << file << std::endl;
            return false;
        }

        std::string source((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        auto tokensResult = lexSourceToTokens(source, options);
        if (!tokensResult) {
            std::cout << "Error: Failed to tokenize standard library file " << file.filename().string() << std::endl;
            printDiagnostics(tokensResult.error());
            return false;
        }
        totalTokens += tokensResult->size();
    }

    return true;
}

} // namespace

namespace {

std::filesystem::path defaultGrammarPath()
{
    return resolvePathNearExecutable("examples/ipl/grammar_rules.txt");
}

std::filesystem::path defaultTranslationPath()
{
    return resolvePathNearExecutable("examples/translation_rules.txt");
}

std::string trimString(std::string_view view)
{
    const auto begin = view.find_first_not_of(" \t\r\n");
    if (begin == std::string_view::npos) {
        return std::string{};
    }
    const auto end = view.find_last_not_of(" \t\r\n");
    return std::string{view.substr(begin, end - begin + 1)};
}

std::filesystem::path grammarForStandard(std::string_view standard)
{
    if (standard.empty()) {
        return {};
    }

    std::string lower;
    lower.reserve(standard.size());
    for (char c : standard) {
        lower.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    }

    if (lower == "ipl" || lower == "default") {
        return defaultGrammarPath();
    }

    return {};
}

std::filesystem::path resolveExistingPath(const std::filesystem::path& candidate)
{
    if (std::filesystem::exists(candidate)) {
        return candidate;
    }
    auto alternative = resolvePathNearExecutable(candidate);
    if (std::filesystem::exists(alternative)) {
        return alternative;
    }
    return candidate;
}

struct ProjectConfig {
    std::filesystem::path source;
    std::filesystem::path grammar;
    std::filesystem::path translation;
};

bool loadProjectConfig(const std::filesystem::path& configPath, ProjectConfig& out)
{
    std::ifstream file(configPath);
    if (!file.is_open()) {
        std::cout << "Error: Could not open project file " << configPath << std::endl;
        return false;
    }

    const auto root = std::filesystem::absolute(configPath).parent_path();
    std::string line;
    while (std::getline(file, line)) {
        const auto trimmed = trimString(line);
        if (trimmed.empty() || trimmed.front() == '#') {
            continue;
        }

        const auto equalsPos = trimmed.find('=');
        if (equalsPos == std::string::npos) {
            continue;
        }

        const auto key = trimString(trimmed.substr(0, equalsPos));
        const auto value = trimString(trimmed.substr(equalsPos + 1));

        auto resolve = [&](const std::string& relative) {
            std::filesystem::path path(relative);
            if (path.is_relative()) {
                path = root / path;
            }
            return path;
        };

        if (key == "source") {
            out.source = resolve(value);
        } else if (key == "grammar") {
            out.grammar = resolve(value);
        } else if (key == "translation") {
            out.translation = resolve(value);
        }
    }

    if (out.source.empty()) {
        std::cout << "Error: Project file " << configPath << " does not specify a source entry." << std::endl;
        return false;
    }

    if (out.grammar.empty()) {
        out.grammar = defaultGrammarPath();
    }
    if (out.translation.empty()) {
        out.translation = defaultTranslationPath();
    }

    return true;
}

struct CommandLineOptions {
    bool showHelp{false};
    bool showVersion{false};
    bool verbose{false};
    bool lexSamples{false};
    bool useDemo{false};
    bool useStdin{false};
    bool legacyCompile{false};
    bool emitSema{false};
    std::string command{};
    std::string sourceFile{};
    std::string grammarFile{};
    std::string translationFile{};
    std::string projectFile{};
    std::string standard{};
    std::string outputPath{};
    std::string errorMessage{};
    std::vector<std::string> positional;
};

CommandLineOptions parseCommandLine(int argc, char* argv[])
{
    CommandLineOptions opts;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--help" || arg == "-h") {
            opts.showHelp = true;
            continue;
        }
        if (arg == "--version") {
            opts.showVersion = true;
            continue;
        }
        if (arg == "--verbose" || arg == "-v") {
            opts.verbose = true;
            continue;
        }
        if (arg == "--lex-ipl-samples") {
            opts.lexSamples = true;
            opts.command = "lex-samples";
            continue;
        }
        if (arg == "--demo") {
            opts.useDemo = true;
            continue;
        }
        if (arg == "--stdin") {
            opts.useStdin = true;
            continue;
        }
        if (arg == "--emit-sema") {
            opts.emitSema = true;
            continue;
        }
        if (arg == "--grammar" || arg == "-g") {
            if (i + 1 >= argc) {
                opts.errorMessage = "Missing value for --grammar";
                return opts;
            }
            opts.grammarFile = argv[++i];
            continue;
        }
        if (arg == "--translation" || arg == "-t") {
            if (i + 1 >= argc) {
                opts.errorMessage = "Missing value for --translation";
                return opts;
            }
            opts.translationFile = argv[++i];
            continue;
        }
        if (arg == "--project" || arg == "-p") {
            if (i + 1 >= argc) {
                opts.errorMessage = "Missing value for --project";
                return opts;
            }
            opts.projectFile = argv[++i];
            continue;
        }
        if (arg == "--standard" || arg == "-s") {
            if (i + 1 >= argc) {
                opts.errorMessage = "Missing value for --standard";
                return opts;
            }
            opts.standard = argv[++i];
            continue;
        }
        if (arg == "--output" || arg == "-o") {
            if (i + 1 >= argc) {
                opts.errorMessage = "Missing value for --output";
                return opts;
            }
            opts.outputPath = argv[++i];
            continue;
        }

        if (arg == "compile" || arg == "run" || arg == "lex-samples") {
            opts.command = arg;
            continue;
        }

        if (!arg.empty() && arg.front() == '-') {
            opts.errorMessage = "Unknown option: " + arg;
            return opts;
        }

        opts.positional.push_back(std::move(arg));
    }

    if (opts.command.empty() && opts.lexSamples) {
        opts.command = "lex-samples";
    }

    if (opts.command == "compile") {
        if (!opts.positional.empty()) {
            opts.sourceFile = opts.positional.front();
            opts.positional.erase(opts.positional.begin());
        }
        if (!opts.positional.empty() && opts.grammarFile.empty()) {
            opts.grammarFile = opts.positional.front();
            opts.positional.erase(opts.positional.begin());
        }
        if (!opts.positional.empty() && opts.translationFile.empty()) {
            opts.translationFile = opts.positional.front();
            opts.positional.erase(opts.positional.begin());
        }
    }

    if (opts.command.empty() && opts.positional.size() == 3 && !opts.useDemo && !opts.useStdin && !opts.lexSamples) {
        opts.command = "compile";
        opts.legacyCompile = true;
        opts.sourceFile = opts.positional[0];
        opts.grammarFile = opts.positional[1];
        opts.translationFile = opts.positional[2];
        opts.positional.clear();
    }

    if (!opts.standard.empty() && opts.grammarFile.empty()) {
        const auto grammar = grammarForStandard(opts.standard);
        if (!grammar.empty()) {
            opts.grammarFile = grammar.string();
        } else {
            opts.errorMessage = "Unknown standard: " + opts.standard;
            return opts;
        }
    }

    return opts;
}

void printUsage(const std::string& executable)
{
    std::cout << "Usage:\n"
              << "  " << executable << " compile <source> [--grammar file] [--translation file] [options]\n"
              << "  " << executable << " run [--project file] [options]\n"
              << "  " << executable << " lex-samples [--grammar file]\n"
              << "  " << executable << " --demo\n"
              << "  " << executable << " --stdin [--grammar file] [--translation file]\n"
              << "  " << executable << " --help | --version\n"
              << "Options:\n"
              << "  --verbose, -v            Enable verbose logging\n"
              << "  --grammar, -g <file>     Override grammar rules file\n"
              << "  --translation, -t <file> Override translation rules file\n"
              << "  --project, -p <file>     Specify project manifest for run command (default: ./ipl_project.ini)\n"
              << "  --standard, -s <name>    Select grammar standard (e.g. ipl)\n"
              << "  --output, -o <path>      Output path for future phases (currently informational)\n"
              << "  --emit-sema              Run semantic analysis and print symbol summary\n"
              << "  --lex-ipl-samples        Tokenize bundled IPL samples\n";
}

void printVersion()
{
    std::cout << "IStudio compiler version 0.2.0 (prototype)\n";
}

} // namespace
class Compiler {
public:
    explicit Compiler(bool verbose = false, bool emitSemanticSummary = false)
        : verbose_(verbose), emitSemanticSummary_(emitSemanticSummary) {}
    ~Compiler() = default;

    bool compile(const std::string& source);
    bool compileWithConfig(const std::string& sourceCodeFile,
                           const std::string& grammarFile,
                           const std::string& translationFile);

private:
    bool compileInternal(std::string_view source,
                         istudio::LexerOptions lexerOptions,
                         const std::vector<TranslationRule>& translationRules);

    void indexAST(const ASTNode& node);
    void printSymbolSummary() const;
    void printSemanticSummary(const istudio::semantic::SymbolScope::Ptr& scope, int indent) const;
    static std::string symbolTypeToString(SymbolType type);

    SymbolTable symbolTable_;
    bool verbose_{false};
    bool emitSemanticSummary_{false};
};

bool Compiler::compile(const std::string& source)
{
    std::cout << "Starting compilation of source code...\n";
    istudio::LexerOptions lexerOptions{};
    if (!compileInternal(source, lexerOptions, std::vector<TranslationRule>{})) {
        return false;
    }
    std::cout << "Compilation completed successfully!\n";
    return true;
}

bool Compiler::compileWithConfig(const std::string& sourceCodeFile,
                                 const std::string& grammarFile,
                                 const std::string& translationFile)
{
    if (verbose_) {
        std::cout << "Starting compilation with configuration...\n";
    }

    Config config;

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

    if (verbose_) {
        std::cout << "Loaded " << config.getGrammarRules().size() << " grammar rules\n";
        std::cout << "Loaded " << config.getTranslationRules().size() << " translation rules\n";
    }

    auto lexerOptions = makeLexerOptions(config.getGrammarRules());
    if (!compileInternal(config.getSourceCode(), lexerOptions, config.getTranslationRules())) {
        return false;
    }

    if (verbose_ && !config.getTranslationRules().empty()) {
        std::cout << "Applying translation rules:\n";
        for (const auto& rule : config.getTranslationRules()) {
            std::cout << "  " << rule.from_language << " -> " << rule.to_language
                      << ": " << rule.rule << std::endl;
        }
    }

    std::cout << "Compilation with configuration completed successfully!\n";
    return true;
}

bool Compiler::compileInternal(std::string_view source,
                               istudio::LexerOptions lexerOptions,
                               const std::vector<TranslationRule>& translationRules)
{
    std::size_t stdlibCount = 0;
    if (!loadStandardLibraryTokens(lexerOptions, stdlibCount, verbose_)) {
        return false;
    }

    auto lexResult = lexSourceToTokens(source, lexerOptions);
    if (!lexResult) {
        printDiagnostics(lexResult.error());
        return false;
    }

    auto tokens = std::move(lexResult.value());
    const std::size_t userCount = tokens.size();

    if (verbose_) {
        std::cout << "Lexical analysis produced " << userCount << " tokens ("
                  << stdlibCount << " from standard library)\n";
    }

    Parser parser(std::move(tokens));
    auto ast = parser.parse();
    if (parser.hadError()) {
        std::cout << "Parsing encountered errors.\n";
        return false;
    }

    if (!ast) {
        std::cout << "Failed to generate AST\n";
        return false;
    }

    if (verbose_) {
        std::cout << "Abstract Syntax Tree generated:\n";
        ast->print();
    }

    semantic::SemanticAnalyzer analyzer({.verbose = verbose_});
    istudio::DiagnosticEngine semaDiagnostics;
    if (!analyzer.analyze(*ast, semaDiagnostics)) {
        printDiagnostics(semaDiagnostics.getDiagnostics());
        return false;
    }

    if (emitSemanticSummary_) {
        std::cout << "\nSemantic summary:\n";
        printSemanticSummary(analyzer.globalScope(), 0);
    }

    symbolTable_.clear();
    indexAST(*ast);
    printSymbolSummary();

    // Apply translation rules if provided
    if (!translationRules.empty() && verbose_) {
        std::cout << "\nTranslation rules applied:\n";
        for (const auto& rule : translationRules) {
            std::cout << "  " << rule.from_language << " -> " << rule.to_language 
                      << ": " << rule.rule << std::endl;
        }
    }

    return true;
}

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
    case ASTNodeType::UnaryOperation: {
        const auto& unary = static_cast<const UnaryOperationNode&>(node);
        if (const auto* operand = unary.getOperand()) {
            indexAST(*operand);
        }
        break;
    }
    case ASTNodeType::CallExpression: {
        const auto& call = static_cast<const CallExpressionNode&>(node);
        if (const auto* callee = call.getCallee()) {
            indexAST(*callee);
        }
        for (const auto& arg : call.getArguments()) {
            if (arg) {
                indexAST(*arg);
            }
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
    case ASTNodeType::ExpressionStatement: {
        const auto& exprStmt = static_cast<const ExpressionStatementNode&>(node);
        if (const auto* expr = exprStmt.getExpression()) {
            indexAST(*expr);
        }
        break;
    }
    case ASTNodeType::If: {
        const auto& ifNode = static_cast<const IfNode&>(node);
        if (const auto* condition = ifNode.getCondition()) {
            indexAST(*condition);
        }
        if (const auto* thenBranch = ifNode.getThenBranch()) {
            indexAST(*thenBranch);
        }
        if (const auto* elseBranch = ifNode.getElseBranch()) {
            indexAST(*elseBranch);
        }
        break;
    }
    case ASTNodeType::While: {
        const auto& whileNode = static_cast<const WhileNode&>(node);
        if (const auto* condition = whileNode.getCondition()) {
            indexAST(*condition);
        }
        if (const auto* body = whileNode.getBody()) {
            indexAST(*body);
        }
        break;
    }
    case ASTNodeType::For: {
        const auto& forNode = static_cast<const ForNode&>(node);
        if (const auto* init = forNode.getInit()) {
            indexAST(*init);
        }
        if (const auto* condition = forNode.getCondition()) {
            indexAST(*condition);
        }
        if (const auto* increment = forNode.getIncrement()) {
            indexAST(*increment);
        }
        if (const auto* body = forNode.getBody()) {
            indexAST(*body);
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
        std::cout << "No symbols recorded.\n";
        return;
    }

    std::cout << "Symbols discovered:\n";
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

void Compiler::printSemanticSummary(const istudio::semantic::SymbolScope::Ptr& scope, int indent) const
{
    if (!scope) {
        return;
    }

    std::vector<std::pair<std::string, istudio::semantic::Symbol>> entries;
    entries.reserve(scope->symbols().size());
    for (const auto& entry : scope->symbols()) {
        entries.emplace_back(entry.first, entry.second);
    }
    std::sort(entries.begin(), entries.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.first < rhs.first;
    });

    std::string padding(static_cast<std::size_t>(indent) * 2, ' ');
    for (const auto& [name, symbol] : entries) 
    {
        std::string kind = symbol.kind == istudio::semantic::SymbolKind::Function ? "fn" : "var";
        std::string typeName = symbol.type ? symbol.type->name() : "<unknown>";
        std::cout << padding << "- [" << kind << "] " << name << " : " << typeName << std::endl;
    }

    for (const auto& child : scope->children()) {
        printSemanticSummary(child, indent + 1);
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
    std::cout << "IStudio - Impossible Programming Language (IPL) Compiler\n" << std::endl;

    const std::string executable = argc > 0 ? argv[0] : "IStudio";
    CommandLineOptions options = parseCommandLine(argc, argv);

    if (!options.errorMessage.empty()) {
        std::cout << "Error: " << options.errorMessage << std::endl;
        printUsage(executable);
        return 1;
    }

    if (options.showVersion) {
        printVersion();
        return 0;
    }

    if (options.showHelp) {
        printUsage(executable);
        return 0;
    }

    if (!options.outputPath.empty()) {
        std::cout << "Note: --output currently has no effect (code generation is not implemented). Requested path: "
                  << options.outputPath << std::endl;
    }

    Compiler compiler(options.verbose, options.emitSema);

    auto resolveOrDefaultGrammar = [&](const std::string& overridePath) {
        if (!overridePath.empty()) {
            auto path = resolveExistingPath(overridePath);
            if (!std::filesystem::exists(path)) {
                std::cout << "Error: Grammar file not found: " << overridePath << std::endl;
                return std::filesystem::path{};
            }
            return path;
        }
        return defaultGrammarPath();
    };

    auto resolveOrDefaultTranslation = [&](const std::string& overridePath) {
        if (!overridePath.empty()) {
            auto path = resolveExistingPath(overridePath);
            if (!std::filesystem::exists(path)) {
                std::cout << "Error: Translation file not found: " << overridePath << std::endl;
                return std::filesystem::path{};
            }
            return path;
        }
        return defaultTranslationPath();
    };

    if (options.lexSamples || options.command == "lex-samples") {
        auto grammarPath = resolveOrDefaultGrammar(options.grammarFile);
        if (grammarPath.empty() || !std::filesystem::exists(grammarPath)) {
            return 1;
        }
        auto samples = resolvePathNearExecutable("examples/ipl");
        return lexDirectoryWithGrammar(samples, grammarPath) ? 0 : 1;
    }

    if (options.useDemo) {
        if (auto demo = discoverDemoConfig()) {
            auto grammar = options.grammarFile.empty() ? demo->grammar : resolveExistingPath(options.grammarFile);
            auto translation = options.translationFile.empty() ? demo->translation : resolveExistingPath(options.translationFile);
            return compiler.compileWithConfig(demo->source.string(), grammar.string(), translation.string()) ? 0 : 1;
        }
        std::cout << "Demo assets are missing. Re-run from the project directory or provide explicit paths." << std::endl;
        printUsage(executable);
        return 2;
    }

    if (options.useStdin) {
        auto grammar = resolveOrDefaultGrammar(options.grammarFile);
        auto translation = resolveOrDefaultTranslation(options.translationFile);
        if (grammar.empty() || translation.empty() ||
            !std::filesystem::exists(grammar) || !std::filesystem::exists(translation)) {
            return 1;
        }

        std::cout << "Reading source code from stdin. Press Ctrl+D when done.\n";
        std::string sourceCode((std::istreambuf_iterator<char>(std::cin)), std::istreambuf_iterator<char>());
        if (sourceCode.empty()) {
            std::cout << "No source code provided on stdin." << std::endl;
            return 1;
        }

        auto tempFile = std::filesystem::temp_directory_path() / "istudio-stdin-src.ipl";
        {
            std::ofstream out(tempFile);
            out << sourceCode;
        }

        const bool success = compiler.compileWithConfig(tempFile.string(), grammar.string(), translation.string());
        std::filesystem::remove(tempFile);
        return success ? 0 : 1;
    }

    if (options.command == "run") {
        std::filesystem::path projectPath = options.projectFile.empty()
                                            ? std::filesystem::current_path() / "ipl_project.ini"
                                            : std::filesystem::path(options.projectFile);
        projectPath = resolveExistingPath(projectPath);
        if (!std::filesystem::exists(projectPath)) {
            std::cout << "Error: Project file not found: " << projectPath << std::endl;
            return 1;
        }

        ProjectConfig project{};
        if (!loadProjectConfig(projectPath, project)) {
            return 1;
        }

        if (!options.grammarFile.empty()) {
            project.grammar = resolveExistingPath(options.grammarFile);
        }
        if (!options.translationFile.empty()) {
            project.translation = resolveExistingPath(options.translationFile);
        }

        return compiler.compileWithConfig(project.source.string(), project.grammar.string(), project.translation.string()) ? 0 : 1;
    }

    if (options.command == "compile") {
        if (options.sourceFile.empty()) {
            if (!options.positional.empty()) {
                options.sourceFile = options.positional.front();
                options.positional.erase(options.positional.begin());
            }
        }

        if (options.sourceFile.empty()) {
            std::cout << "Error: No source file specified for compilation." << std::endl;
            printUsage(executable);
            return 1;
        }

        auto grammar = resolveOrDefaultGrammar(options.grammarFile);
        auto translation = resolveOrDefaultTranslation(options.translationFile);
        if (grammar.empty() || translation.empty() ||
            !std::filesystem::exists(grammar) || !std::filesystem::exists(translation)) {
            return 1;
        }

        return compiler.compileWithConfig(options.sourceFile, grammar.string(), translation.string()) ? 0 : 1;
    }

    const char* runTest = std::getenv("RUN_COMPILER_TEST");
    if (runTest && std::string(runTest) == "1") {
        std::string sourceCode = "int main() { int x = 42; return x; }";
        return compiler.compile(sourceCode) ? 0 : 1;
    }

    if (options.legacyCompile) {
        auto grammar = resolveExistingPath(options.grammarFile);
        auto translation = resolveExistingPath(options.translationFile);
        if (!std::filesystem::exists(grammar) || !std::filesystem::exists(translation)) {
            std::cout << "Error: Grammar or translation file not found." << std::endl;
            return 1;
        }
        return compiler.compileWithConfig(options.sourceFile, grammar.string(), translation.string()) ? 0 : 1;
    }

    printUsage(executable);
    return 1;
}

#include "../include/Parser.h"
#include "../include/Lexer.h"
#include <algorithm>
#include <cctype>

namespace {

constexpr size_t MAX_TOKENS = 10000;

bool isIdentifierStart(const std::string& token)
{
    return !token.empty() && (std::isalpha(static_cast<unsigned char>(token.front())) || token.front() == '_');
}

} // namespace

Parser::Parser(const std::string& source) : position_(0) {
    Lexer lexer(source);
    std::string token;
    size_t tokenCount = 0;

    while (!(token = lexer.getNextToken()).empty() && tokenCount < MAX_TOKENS) {
        tokens_.push_back(token);
        ++tokenCount;
    }
}

std::unique_ptr<ProgramNode> Parser::parse() {
    auto program = std::make_unique<ProgramNode>();

    while (position_ < tokens_.size()) {
        if (isTypeKeyword(getCurrentToken())) {
            if (auto function = parseFunction()) {
                program->addFunction(std::move(function));
                continue;
            }
        }

        auto statement = parseStatement();
        if (!statement) {
            synchronize();
        }
    }

    return program;
}

std::unique_ptr<FunctionNode> Parser::parseFunction()
{
    if (!isTypeKeyword(getCurrentToken())) {
        return nullptr;
    }

    std::string returnType = getNextToken();
    std::string functionName = getNextToken();

    if (functionName.empty() || functionName == "(") {
        return nullptr;
    }

    if (!matchToken("(")) {
        return nullptr;
    }

    auto parameters = parseParameterList();

    if (!matchToken("{")) {
        return nullptr;
    }

    auto body = parseBlock();
    return std::make_unique<FunctionNode>(std::move(returnType), std::move(functionName), std::move(parameters), std::move(body));
}

std::unique_ptr<BlockNode> Parser::parseBlock()
{
    auto block = std::make_unique<BlockNode>();

    while (position_ < tokens_.size() && getCurrentToken() != "}") {
        auto statement = parseStatement();
        if (statement) {
            block->addStatement(std::move(statement));
        } else {
            synchronize();
        }
    }

    matchToken("}");
    return block;
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (position_ >= tokens_.size()) {
        return nullptr;
    }

    const std::string token = getCurrentToken();

    if (token == "return") {
        return parseReturn();
    }

    if (isTypeKeyword(token)) {
        const std::string type = getNextToken();
        const std::string name = getNextToken();

        if (name.empty()) {
            return nullptr;
        }

        std::unique_ptr<ASTNode> initializer;
        if (matchToken("=")) {
            initializer = parseExpression();
        }

        if (!matchToken(";")) {
            return nullptr;
        }

        return std::make_unique<VariableDeclarationNode>(type, name, std::move(initializer));
    }

    if (isIdentifierStart(token)) {
        const std::string identifier = getNextToken();
        if (matchToken("=")) {
            auto value = parseExpression();
            if (matchToken(";")) {
                return std::make_unique<AssignmentNode>(identifier, std::move(value));
            }
        }
    }

    if (matchToken(";")) {
        return nullptr;
    }

    return nullptr;
}

std::unique_ptr<ASTNode> Parser::parseReturn()
{
    getNextToken(); // consume 'return'

    if (matchToken(";")) {
        return std::make_unique<ReturnNode>(nullptr);
    }

    auto value = parseExpression();
    matchToken(";");
    return std::make_unique<ReturnNode>(std::move(value));
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    auto left = parseTerm();

    while (position_ < tokens_.size()) {
        const std::string op = getCurrentToken();
        if (op == "+" || op == "-" || op == "||" || op == "&&") {
            getNextToken();
            auto right = parseTerm();
            left = std::make_unique<BinaryOperationNode>(op, std::move(left), std::move(right));
        } else {
            break;
        }
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseTerm() {
    auto left = parseFactor();

    while (position_ < tokens_.size()) {
        const std::string op = getCurrentToken();
        if (op == "*" || op == "/" || op == "%") {
            getNextToken();
            auto right = parseFactor();
            left = std::make_unique<BinaryOperationNode>(op, std::move(left), std::move(right));
        } else {
            break;
        }
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseFactor() {
    if (position_ >= tokens_.size()) {
        return nullptr;
    }

    const std::string token = getCurrentToken();

    if (token == "(") {
        getNextToken();
        auto expr = parseExpression();
        matchToken(")");
        return expr;
    }

    if (!token.empty() && std::isdigit(static_cast<unsigned char>(token.front()))) {
        getNextToken();
        return std::make_unique<LiteralNode>(token);
    }

    if (isIdentifierStart(token)) {
        getNextToken();
        return std::make_unique<IdentifierNode>(token);
    }

    return nullptr;
}

std::vector<FunctionParameter> Parser::parseParameterList()
{
    std::vector<FunctionParameter> parameters;

    while (position_ < tokens_.size() && getCurrentToken() != ")") {
        std::string type = getNextToken();
        std::string name = getNextToken();
        if (type.empty() || name.empty()) {
            break;
        }

        parameters.push_back(FunctionParameter{type, name});

        if (!matchToken(",")) {
            break;
        }
    }

    matchToken(")");
    return parameters;
}

bool Parser::isTypeKeyword(const std::string& token) const
{
    static const std::vector<std::string> kTypeKeywords = {"int", "float", "double", "char", "bool", "void", "long", "short", "auto"};
    return std::find(kTypeKeywords.begin(), kTypeKeywords.end(), token) != kTypeKeywords.end();
}

std::string Parser::peekToken(size_t offset) const
{
    if (position_ + offset < tokens_.size()) {
        return tokens_[position_ + offset];
    }
    return "";
}

void Parser::synchronize()
{
    while (position_ < tokens_.size()) {
        const std::string token = getCurrentToken();
        if (token == ";" || token == "}") {
            getNextToken();
            break;
        }
        getNextToken();
    }
}

std::string Parser::getCurrentToken() {
    if (position_ < tokens_.size()) {
        return tokens_[position_];
    }
    return "";
}

std::string Parser::getNextToken() {
    if (position_ < tokens_.size()) {
        return tokens_[position_++];
    }
    return "";
}

bool Parser::hasNextToken() {
    return position_ < tokens_.size();
}

bool Parser::matchToken(const std::string& expected) {
    if (position_ < tokens_.size() && tokens_[position_] == expected) {
        ++position_;
        return true;
    }
    return false;
}
#include "../include/Parser.h"
#include "istudio/Diagnostics.h"
#include "istudio/Lexer.h"

#include <algorithm>
#include <cctype>
#include <utility>

namespace {

bool isIdentifierToken(const istudio::Token* token)
{
    return token && token->kind == istudio::TokenKind::Identifier;
}

bool isLiteralToken(const istudio::Token* token)
{
    if (!token) {
        return false;
    }

    switch (token->kind) {
    case istudio::TokenKind::IntegerLiteral:
    case istudio::TokenKind::FloatLiteral:
    case istudio::TokenKind::StringLiteral:
    case istudio::TokenKind::RawStringLiteral:
    case istudio::TokenKind::BooleanLiteral:
    case istudio::TokenKind::NullLiteral:
        return true;
    default:
        return false;
    }
}

} // namespace

Parser::Parser(std::vector<istudio::Token> tokens)
    : tokens_(std::move(tokens)), position_(0)
{
}

Parser::Parser(const std::string& source) : position_(0) {
    istudio::DiagnosticEngine diagnostics;
    istudio::LexerOptions options{};
    istudio::Lexer lexer(source, options, diagnostics);
    if (auto result = lexer.tokenize()) {
        tokens_.reserve(result->size());
        for (auto& token : *result) {
            if (token.kind == istudio::TokenKind::EndOfFile ||
                token.kind == istudio::TokenKind::Comment ||
                token.kind == istudio::TokenKind::DocComment) {
                continue;
            }
            tokens_.push_back(std::move(token));
        }
    }
}

std::unique_ptr<ProgramNode> Parser::parse() {
    auto program = std::make_unique<ProgramNode>();

    while (position_ < tokens_.size()) {
        if (isTypeKeyword(currentLexeme())) {
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
    if (!isTypeKeyword(currentLexeme())) {
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

    while (position_ < tokens_.size() && currentLexeme() != "}") {
        auto statement = parseStatement();
        if (statement) {
            block->addStatement(std::move(statement));
        } else {
            synchronize();
        }
    }

    if (!matchToken("}")) {
        hadError_ = true;
    }
    return block;
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (position_ >= tokens_.size()) {
        return nullptr;
    }

    if (currentLexeme() == "{") {
        advanceToken();
        return parseBlock();
    }

    if (matchKeyword("if")) {
        return parseIf();
    }

    if (matchKeyword("while")) {
        return parseWhile();
    }

    if (matchKeyword("for")) {
        return parseFor();
    }

    if (currentLexeme() == "return") {
        return parseReturn();
    }

    if (currentLexeme() == "let" || currentLexeme() == "const" || currentLexeme() == "final") {
        auto keyword = currentLexeme();
        advanceToken();
        return parseDeclarationLike(keyword);
    }

    if (isTypeKeyword(currentLexeme())) {
        const std::string type = getNextToken();
        const std::string name = getNextToken();
        if (name.empty()) {
            hadError_ = true;
            return nullptr;
        }

        std::unique_ptr<ASTNode> initializer;
        if (matchToken("=")) {
            initializer = parseExpression();
        }

        if (!matchToken(";")) {
            hadError_ = true;
            return nullptr;
        }

        return std::make_unique<VariableDeclarationNode>(type, name, std::move(initializer));
    }

    if (isIdentifierToken(getCurrentToken()) && peekToken(1) && peekToken(1)->lexeme == "=") {
        const std::string identifier = getNextToken();
        matchToken("=");
        auto value = parseExpression();
        if (!expectLexeme(";")) {
            return nullptr;
        }
        return std::make_unique<AssignmentNode>(identifier, std::move(value));
    }

    auto expression = parseExpression();
    if (!expression) {
        hadError_ = true;
        return nullptr;
    }
    if (!expectLexeme(";")) {
        return nullptr;
    }
    return std::make_unique<ExpressionStatementNode>(std::move(expression));
}

std::unique_ptr<ASTNode> Parser::parseReturn()
{
    getNextToken(); // consume 'return'

    if (matchToken(";")) {
        return std::make_unique<ReturnNode>(nullptr);
    }

    auto value = parseExpression();
    if (!expectLexeme(";")) {
        hadError_ = true;
        return nullptr;
    }
    return std::make_unique<ReturnNode>(std::move(value));
}

std::unique_ptr<ASTNode> Parser::parseIf()
{
    if (!expectLexeme("(")) {
        hadError_ = true;
        return nullptr;
    }
    auto condition = parseExpression();
    if (!expectLexeme(")")) {
        hadError_ = true;
        return nullptr;
    }

    std::unique_ptr<ASTNode> thenBranch;
    if (currentLexeme() == "{") {
        advanceToken();
        thenBranch = parseBlock();
    } else {
        thenBranch = parseStatement();
    }

    std::unique_ptr<ASTNode> elseBranch;
    if (matchKeyword("otherwise")) {
        if (currentLexeme() == "{") {
            advanceToken();
            elseBranch = parseBlock();
        } else {
            elseBranch = parseStatement();
        }
    }

    return std::make_unique<IfNode>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<ASTNode> Parser::parseWhile()
{
    if (!expectLexeme("(")) {
        hadError_ = true;
        return nullptr;
    }
    auto condition = parseExpression();
    if (!expectLexeme(")")) {
        hadError_ = true;
        return nullptr;
    }

    std::unique_ptr<ASTNode> body;
    if (currentLexeme() == "{") {
        advanceToken();
        body = parseBlock();
    } else {
        body = parseStatement();
    }

    return std::make_unique<WhileNode>(std::move(condition), std::move(body));
}

std::unique_ptr<ASTNode> Parser::parseFor()
{
    if (!expectLexeme("(")) {
        hadError_ = true;
        return nullptr;
    }

    std::unique_ptr<ASTNode> init;
    if (currentLexeme() != ";") {
        if (currentLexeme() == "let" || currentLexeme() == "const" || currentLexeme() == "final") {
            auto keyword = currentLexeme();
            advanceToken();
            init = parseDeclarationLike(keyword);
        } else {
            auto expr = parseExpression();
            if (!expectLexeme(";")) {
                hadError_ = true;
                return nullptr;
            }
            init = std::make_unique<ExpressionStatementNode>(std::move(expr));
        }
    } else {
        matchToken(";");
    }

    std::unique_ptr<ASTNode> condition;
    if (currentLexeme() != ";") {
        condition = parseExpression();
    }
    if (!expectLexeme(";")) {
        hadError_ = true;
        return nullptr;
    }

    std::unique_ptr<ASTNode> increment;
    if (currentLexeme() != ")") {
        increment = parseExpression();
    }
    if (!expectLexeme(")")) {
        hadError_ = true;
        return nullptr;
    }

    std::unique_ptr<ASTNode> body;
    if (currentLexeme() == "{") {
        advanceToken();
        body = parseBlock();
    } else {
        body = parseStatement();
    }

    return std::make_unique<ForNode>(std::move(init), std::move(condition), std::move(increment), std::move(body));
}

std::unique_ptr<ASTNode> Parser::parseDeclarationLike(const std::string& keyword)
{
    (void)keyword;

    std::string type;
    std::string name = getNextToken();
    if (name.empty()) {
        hadError_ = true;
        return nullptr;
    }

    const auto* lookahead = getCurrentToken();
    if (lookahead && lookahead->lexeme != "=" && lookahead->lexeme != ";") {
        type = std::move(name);
        name = getNextToken();
    }

    if (!matchToken("=")) {
        hadError_ = true;
        return nullptr;
    }

    auto initializer = parseExpression();
    if (!expectLexeme(";")) {
        hadError_ = true;
        return nullptr;
    }
    return std::make_unique<VariableDeclarationNode>(type, name, std::move(initializer));
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    return parseAssignment();
}

std::unique_ptr<ASTNode> Parser::parseAssignment()
{
    auto left = parseLogicalOr();

    if (matchToken("=")) {
        auto right = parseAssignment();
        if (!right) {
            hadError_ = true;
            return nullptr;
        }

        if (auto identifier = dynamic_cast<IdentifierNode*>(left.get())) {
            auto name = identifier->getName();
            return std::make_unique<AssignmentNode>(name, std::move(right));
        }
        hadError_ = true;
        return nullptr;
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseLogicalOr()
{
    auto expr = parseLogicalAnd();
    while (currentLexeme() == "or" || currentLexeme() == "||") {
        const std::string op = getNextToken();
        auto right = parseLogicalAnd();
        expr = std::make_unique<BinaryOperationNode>(op, std::move(expr), std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseLogicalAnd()
{
    auto expr = parseEquality();
    while (currentLexeme() == "and" || currentLexeme() == "&&") {
        const std::string op = getNextToken();
        auto right = parseEquality();
        expr = std::make_unique<BinaryOperationNode>(op, std::move(expr), std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseEquality()
{
    auto expr = parseComparison();
    while (currentLexeme() == "==" || currentLexeme() == "!=") {
        const std::string op = getNextToken();
        auto right = parseComparison();
        expr = std::make_unique<BinaryOperationNode>(op, std::move(expr), std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseComparison()
{
    auto expr = parseTerm();
    while (currentLexeme() == "<" || currentLexeme() == "<=" ||
           currentLexeme() == ">" || currentLexeme() == ">=") {
        const std::string op = getNextToken();
        auto right = parseTerm();
        expr = std::make_unique<BinaryOperationNode>(op, std::move(expr), std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseTerm()
{
    auto left = parseFactor();

    while (position_ < tokens_.size()) {
        const std::string op = currentLexeme();
        if (op == "+" || op == "-") {
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
    auto left = parseUnary();

    while (position_ < tokens_.size()) {
        const std::string op = currentLexeme();
        if (op == "*" || op == "/" || op == "%") {
            getNextToken();
            auto right = parseUnary();
            left = std::make_unique<BinaryOperationNode>(op, std::move(left), std::move(right));
        } else {
            break;
        }
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseUnary()
{
    const std::string op = currentLexeme();
    if (op == "!" || op == "-" || op == "+") {
        getNextToken();
        auto operand = parseUnary();
        return std::make_unique<UnaryOperationNode>(op, std::move(operand));
    }
    return parseCall();
}

std::unique_ptr<ASTNode> Parser::parseCall()
{
    auto expr = parsePrimary();

    while (currentLexeme() == "(") {
        expr = finishCall(std::move(expr));
    }

    return expr;
}

std::unique_ptr<ASTNode> Parser::finishCall(std::unique_ptr<ASTNode> callee)
{
    if (!expectLexeme("(")) {
        hadError_ = true;
        return nullptr;
    }
    std::vector<std::unique_ptr<ASTNode>> arguments;
    if (currentLexeme() != ")") {
        do {
            auto argument = parseExpression();
            if (argument) {
                arguments.push_back(std::move(argument));
            }
        } while (matchToken(","));
    }
    if (!expectLexeme(")")) {
        hadError_ = true;
        return nullptr;
    }
    return std::make_unique<CallExpressionNode>(std::move(callee), std::move(arguments));
}

std::unique_ptr<ASTNode> Parser::parsePrimary()
{
    const auto* token = getCurrentToken();
    if (!token) {
        hadError_ = true;
        return nullptr;
    }

    if (token->lexeme == "(") {
        getNextToken();
        auto expr = parseExpression();
        if (!expectLexeme(")")) {
            hadError_ = true;
            return nullptr;
        }
        return expr;
    }

    if (isIdentifierToken(token)) {
        std::string name = getNextToken();
        return std::make_unique<IdentifierNode>(name);
    }

    if (isLiteralToken(token)) {
        std::string value = getNextToken();
        return std::make_unique<LiteralNode>(value);
    }

    hadError_ = true;
    return nullptr;
}

std::vector<FunctionParameter> Parser::parseParameterList()
{
    std::vector<FunctionParameter> parameters;

    while (position_ < tokens_.size() && currentLexeme() != ")") {
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
    static const std::vector<std::string> kTypeKeywords = {
        "int", "float", "double", "char", "bool", "void", "long", "short", "auto",
        "number", "string", "bytes", "list", "dict", "set", "matrix", "tuple",
        "Result", "Optional", "any", "Self", "owned", "borrowed", "ref"
    };
    return std::find(kTypeKeywords.begin(), kTypeKeywords.end(), token) != kTypeKeywords.end();
}

const istudio::Token* Parser::peekToken(size_t offset) const
{
    if (position_ + offset < tokens_.size()) {
        return &tokens_[position_ + offset];
    }
    return nullptr;
}

const istudio::Token* Parser::getCurrentToken() const
{
    return peekToken(0);
}

const istudio::Token* Parser::advanceToken()
{
    if (position_ < tokens_.size()) {
        return &tokens_[position_++];
    }
    return nullptr;
}

std::string Parser::currentLexeme() const
{
    const auto* token = getCurrentToken();
    return token ? token->lexeme : "";
}

istudio::TokenKind Parser::currentKind() const
{
    const auto* token = getCurrentToken();
    return token ? token->kind : istudio::TokenKind::Unknown;
}

bool Parser::matchKeyword(std::string_view keyword)
{
    const auto* token = getCurrentToken();
    if (token && token->lexeme == keyword) {
        advanceToken();
        return true;
    }
    return false;
}

bool Parser::expectLexeme(const std::string& expected)
{
    if (matchToken(expected)) {
        return true;
    }
    hadError_ = true;
    return false;
}

void Parser::synchronize()
{
    while (position_ < tokens_.size()) {
        const std::string token = currentLexeme();
        if (token == ";" || token == "}") {
            getNextToken();
            break;
        }
        getNextToken();
    }
}

std::string Parser::getNextToken() {
    const auto* token = advanceToken();
    return token ? token->lexeme : "";
}

bool Parser::hasNextToken() {
    return position_ < tokens_.size();
}

bool Parser::matchToken(const std::string& expected) {
    const auto* token = getCurrentToken();
    if (token && token->lexeme == expected) {
        advanceToken();
        return true;
    }
    return false;
}

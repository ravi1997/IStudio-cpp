#pragma once
#include "AST.h"
#include "Symbol.h"
#include "istudio/Token.h"
#include <memory>
#include <string>
#include <string_view>
#include <vector>

class Parser {
public:
    Parser(const std::string& source);
    explicit Parser(std::vector<istudio::Token> tokens);
    ~Parser() = default;
    
    std::unique_ptr<ProgramNode> parse();
    [[nodiscard]] bool hadError() const noexcept { return hadError_; }
    
private:
    std::unique_ptr<FunctionNode> parseFunction();
    std::unique_ptr<BlockNode> parseBlock();
    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<ASTNode> parseReturn();
    std::unique_ptr<ASTNode> parseIf();
    std::unique_ptr<ASTNode> parseWhile();
    std::unique_ptr<ASTNode> parseFor();
    std::unique_ptr<ASTNode> parseDeclarationLike(const std::string& keyword);
    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseAssignment();
    std::unique_ptr<ASTNode> parseLogicalOr();
    std::unique_ptr<ASTNode> parseLogicalAnd();
    std::unique_ptr<ASTNode> parseEquality();
    std::unique_ptr<ASTNode> parseComparison();
    std::unique_ptr<ASTNode> parseTerm();
    std::unique_ptr<ASTNode> parseFactor();
    std::unique_ptr<ASTNode> parseUnary();
    std::unique_ptr<ASTNode> parseCall();
    std::unique_ptr<ASTNode> finishCall(std::unique_ptr<ASTNode> callee);
    std::unique_ptr<ASTNode> parsePrimary();
    std::vector<FunctionParameter> parseParameterList();
    bool isTypeKeyword(const std::string& token) const;
    const istudio::Token* peekToken(size_t offset = 0) const;
    const istudio::Token* getCurrentToken() const;
    const istudio::Token* advanceToken();
    std::string currentLexeme() const;
    istudio::TokenKind currentKind() const;
    bool matchKeyword(std::string_view keyword);
    bool expectLexeme(const std::string& expected);
    void synchronize();
    std::string getNextToken();
    bool hasNextToken();
    bool matchToken(const std::string& expected);

    std::vector<istudio::Token> tokens_;
    size_t position_;
    bool hadError_{false};
};

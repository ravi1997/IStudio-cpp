#pragma once
#include "AST.h"
#include "Symbol.h"
#include <string>
#include <vector>
#include <memory>

class Parser {
public:
    Parser(const std::string& source);
    ~Parser() = default;
    
    std::unique_ptr<ProgramNode> parse();
    
private:
    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseTerm();
    std::unique_ptr<ASTNode> parseFactor();
    std::string getCurrentToken();
    std::string getNextToken();
    bool hasNextToken();
    bool matchToken(const std::string& expected);
    
    std::vector<std::string> tokens_;
    size_t position_;
};
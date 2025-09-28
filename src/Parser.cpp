#include "../include/Parser.h"
#include "../include/Lexer.h"
#include <sstream>
#include <algorithm>

Parser::Parser(const std::string& source) : position_(0) {
    // Tokenize source using our Lexer
    Lexer lexer(source);
    std::string token;
    size_t tokenCount = 0;  // Prevent infinite loop
    const size_t MAX_TOKENS = 10000;  // Reasonable limit
    
    while (!(token = lexer.getNextToken()).empty() && tokenCount < MAX_TOKENS) {
        tokens_.push_back(token);
        tokenCount++;
    }
    
    if (tokenCount >= MAX_TOKENS) {
        // This indicates a potential issue in the lexer or an extremely large input
        // We'll still continue but with a warning conceptually
    }
}

std::unique_ptr<ProgramNode> Parser::parse() {
    auto program = std::make_unique<ProgramNode>();
    
    while (position_ < tokens_.size()) {
        if (getCurrentToken() == "int" || getCurrentToken() == "void") {
            // Parse function declaration
            std::string returnType = getNextToken(); // Skip return type
            std::string functionName = getNextToken();
            
            if (getNextToken() == "(") { // Function declaration
                // Skip parameters for now, but avoid infinite loop
                while (position_ < tokens_.size()) {
                    std::string current = getCurrentToken();
                    if (current == "{") {
                        break; // Found opening brace, exit loop
                    }
                    getNextToken(); // Move to next token
                }
                
                // Parse function body if we found the opening brace
                if (position_ < tokens_.size() && getCurrentToken() == "{") {
                    getNextToken(); // Skip '{'
                } else {
                    // If no opening brace found, we can't parse the function properly
                    // Skip to next function or end of input
                    break;
                }
                
                auto body = std::make_unique<ProgramNode>(); // Create a program node for function body
                
                // For simplicity, just create a simple function node for now
                auto funcNode = std::make_unique<FunctionNode>(functionName, std::move(body));
                program->addFunction(std::move(funcNode));
            }
        } else {
            // Parse other statements
            auto stmt = parseStatement();
            if (stmt) {
                // Add to program - in a real implementation we'd have appropriate handling
            }
        }
    }
    
    return program;
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (position_ >= tokens_.size()) return nullptr;
    
    std::string token = getCurrentToken();
    
    // Variable declaration
    if (token == "int" || token == "float" || token == "double" || token == "char" || token == "bool") {
        getNextToken(); // Skip type
        std::string varName = getNextToken();
        
        if (matchToken("=")) {
            auto initializer = parseExpression();
            return std::make_unique<VariableDeclarationNode>(varName, std::move(initializer));
        } else if (matchToken(";")) {
            return std::make_unique<VariableDeclarationNode>(varName, nullptr);
        }
    }
    // Assignment
    else if (isalpha(token[0]) || token[0] == '_') {
        std::string varName = getNextToken();
        if (matchToken("=")) {
            auto value = parseExpression();
            if (matchToken(";")) {
                return std::make_unique<AssignmentNode>(varName, std::move(value));
            }
        }
    }
    
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    auto left = parseTerm();
    
    while (position_ < tokens_.size()) {
        std::string op = getCurrentToken();
        if (op == "+" || op == "-" || op == "||" || op == "&&") {
            getNextToken(); // Skip operator
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
        std::string op = getCurrentToken();
        if (op == "*" || op == "/" || op == "%") {
            getNextToken(); // Skip operator
            auto right = parseFactor();
            left = std::make_unique<BinaryOperationNode>(op, std::move(left), std::move(right));
        } else {
            break;
        }
    }
    
    return left;
}

std::unique_ptr<ASTNode> Parser::parseFactor() {
    if (position_ >= tokens_.size()) return nullptr;
    
    std::string token = getCurrentToken();
    
    if (isdigit(token[0])) {
        // Literal number
        getNextToken();
        return std::make_unique<LiteralNode>(token);
    } else if (isalpha(token[0]) || token[0] == '_') {
        // Identifier
        getNextToken();
        return std::make_unique<IdentifierNode>(token);
    } else if (token == "(") {
        getNextToken(); // Skip '('
        auto expr = parseExpression();
        if (matchToken(")")) {
            return expr;
        }
    }
    
    return nullptr;
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
        position_++;
        return true;
    }
    return false;
}
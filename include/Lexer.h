#pragma once
#include <string>

class Lexer {
public:
    Lexer(const std::string& source);
    ~Lexer() = default;
    
    std::string getNextToken();
    bool hasMoreTokens() const;
    
private:
    std::string source_;
    size_t position_;
};
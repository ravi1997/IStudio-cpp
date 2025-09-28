#include "../include/Lexer.h"
#include <cctype>
#include <cstring>

Lexer::Lexer(const std::string& source) : source_(source), position_(0) {}

std::string Lexer::getNextToken() {
    // Skip whitespace
    while (position_ < source_.length() && isspace(source_[position_])) {
        position_++;
    }
    
    if (position_ >= source_.length()) {
        return "";
    }
    
    std::string token = "";
    char current = source_[position_];
    
    // Handle identifiers and keywords
    if (isalpha(current) || current == '_') {
        while (position_ < source_.length() && 
               (isalnum(source_[position_]) || source_[position_] == '_')) {
            token += source_[position_++];
        }
    }
    // Handle numbers
    else if (isdigit(current)) {
        while (position_ < source_.length() && 
               isdigit(source_[position_])) {
            token += source_[position_++];
        }
    }
    // Handle operators and special characters
    else if (strchr("+-*/=<>!&|", current)) {
        token = current;
        position_++;
        
        // Handle multi-character operators
        if (position_ < source_.length()) {
            char next = source_[position_];
            if ((current == '=' && next == '=') ||
                (current == '!' && next == '=') ||
                (current == '&' && next == '&') ||
                (current == '|' && next == '|') ||
                (current == '<' && next == '=') ||
                (current == '>' && next == '=')) {
                token += next;
                position_++;
            }
        }
    }
    // Handle single character symbols
    else if (strchr("(){}[];,", current)) {
        token = current;
        position_++;
    }
    // Handle string literals
    else if (current == '"') {
        token += source_[position_++]; // Add opening quote
        while (position_ < source_.length() && source_[position_] != '"') {
            token += source_[position_++];
        }
        if (position_ < source_.length()) {
            token += source_[position_++]; // Add closing quote
        }
    }
    else {
        // Skip unknown characters
        position_++;
    }
    
    return token;
}

bool Lexer::hasMoreTokens() const {
    size_t temp_pos = position_;
    
    // Skip whitespace
    while (temp_pos < source_.length() && isspace(source_[temp_pos])) {
        temp_pos++;
    }
    
    return temp_pos < source_.length();
}
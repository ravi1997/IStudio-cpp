#include "istudio/Lexer.h"
#include <sstream>
#include <algorithm>

namespace istudio {

Lexer::Lexer(std::string_view source, const LexerOptions& options, DiagnosticEngine& diagnostics)
    : source_(source), options_(options), diagnostics_(diagnostics) {
}

std::expected<std::vector<Token>, std::vector<Diagnostic>> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    // Convert std::string_view to std::string first to avoid the parsing ambiguity
    std::string source_string(source_);
    std::istringstream stream(source_string);
    std::string line;
    std::size_t lineNum = 1;
    
    while (std::getline(stream, line)) {
        // For now, just add the line as a token with lexeme (this is a placeholder)
        if (!line.empty()) {
            Token token;
            token.kind = TokenKind::Unknown; // In a real implementation, this would be properly classified
            token.text = std::string_view(line);
            token.lexeme = line; // Set the lexeme to the actual string
            token.line = lineNum;
            token.column = 1;
            tokens.push_back(token);
        }
        ++lineNum;
    }
    
    // Add EOF token
    Token eofToken;
    eofToken.kind = TokenKind::EndOfFile;
    eofToken.text = std::string_view();
    eofToken.lexeme = ""; // EOF has empty lexeme
    eofToken.line = lineNum;
    eofToken.column = 1;
    tokens.push_back(eofToken);
    
    return tokens;
}

} // namespace istudio
#ifndef ISTUDIO_LEXER_H
#define ISTUDIO_LEXER_H

#include "Token.h"
#include "Diagnostics.h"
#include <string_view>
#include <vector>
#include <expected>

namespace istudio {

class Lexer {
public:
    Lexer(std::string_view source, const LexerOptions& options, DiagnosticEngine& diagnostics);
    
    std::expected<std::vector<Token>, std::vector<Diagnostic>> tokenize();

private:
    std::string_view source_;
    LexerOptions options_;
    DiagnosticEngine& diagnostics_;
};

} // namespace istudio

#endif // ISTUDIO_LEXER_H
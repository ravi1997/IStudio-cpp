#ifndef ISTUDIO_TOKEN_H
#define ISTUDIO_TOKEN_H

#include <string>
#include <string_view>
#include <vector>
#include <expected>
#include <optional>
#include "Diagnostics.h"  // Include the Diagnostics header

namespace istudio {

struct GrammarRule {
    std::string pattern;
    std::string action;
};

enum class TokenKind {
    Unknown,
    EndOfFile,
    Comment,
    DocComment,
    Identifier,
    Keyword,
    Number,
    String,
    Operator,
    Punctuation,
    Whitespace,
    IntegerLiteral,
    FloatLiteral,
    StringLiteral,
    RawStringLiteral,
    BooleanLiteral,
    NullLiteral,
};

struct Token {
    TokenKind kind;
    std::string_view text;
    std::string lexeme;  // Store the actual text of the token
    std::size_t line;
    std::size_t column;
};

struct LexerOptions {
    std::vector<GrammarRule> grammar;
};

template<typename T>
using PhaseResult = std::expected<T, std::vector<Diagnostic>>;

} // namespace istudio

#endif // ISTUDIO_TOKEN_H
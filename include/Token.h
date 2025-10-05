#pragma once

#include "Diagnostics.h"

#include <string>
#include <string_view>
#include <variant>

namespace istudio {

enum class TokenKind {
    Identifier,
    Keyword,
    BooleanLiteral,
    NullLiteral,
    IntegerLiteral,
    FloatLiteral,
    StringLiteral,
    RawStringLiteral,
    Operator,
    Punctuator,
    DocComment,
    Comment,
    EndOfFile,
    Unknown
};

struct Token {
    TokenKind kind{TokenKind::Unknown};
    std::string lexeme{};
    SourceRange range{};
};

struct GrammarRuleCompiled {
    std::string pattern;
    std::string action;
};

} // namespace istudio

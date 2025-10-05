# 05 — Lexical Conventions (Normative)

## 5.1 Source Text and Encoding
- Source files **shall** be encoded in UTF‑8 without BOM.
- A line terminator is LF (U+000A). Implementations **may** accept CRLF as a courtesy.

## 5.2 Tokens
The input stream is partitioned into tokens: identifiers, keywords, literals, operators, punctuators, and comments. Tokenization uses a maximal‑munch rule.

### 5.2.1 Identifiers
```
identifier = ( letter | \"_\" ) { letter | digit | \"_\" } ;
letter     = UnicodeLetter ;
digit      = \"0\"… \"9\" ;
```
- Identifiers are case‑sensitive.
- Keywords are reserved and cannot be identifiers.

**Valid**
```ipl
count
_compute_total
Δmatrix   // Unicode letters permitted
```
**Invalid (requires diagnostic)**
```ipl
2ndValue      // cannot start with digit
user-name     // '-' not permitted in identifiers
```

### 5.2.2 Keywords (reserved)
`let, const, final, function, contract, class, inherits, implements, interface, trait, mixin, module, import, export, if, otherwise, repeat, until, for, each, while, break, continue, try, catch, finally, throw, return, true, false, null, new, operator, where, template, concept, constexpr, ensure, require, public, private, protected, static, this, super, match, case, default, owned, borrowed, move, ref, interface, meta, final`

### 5.2.3 Literals
- **Numeric**: decimal integers and decimals; implementations **may** support hex with `0x`, octal with `0o`, and binary with `0b`.
```
integer = [ \"-\" ] digit { digit } ;
decimal = [ \"-\" ] digit { digit } \".\" digit { digit } ;
```
- **String**: double‑quoted; backslash escapes `\\\" \\\\ \\n \\t \\u{{XXXX}}` where `XXXX` is 4 hex digits.
- **Raw string**: prefixed with `r\"...\"` for literal strings without escape processing.
- **Matrix**: multi-dimensional array notation `[ [1, 2], [3, 4] ]` or `matrix[rows, cols] { ... }`
- **Boolean**: `true`, `false`.
- **Null**: `null`.

**Valid**
```ipl
42
3.1415
\"Hello, \\nWorld\"
r\"Raw string \\\\n no escapes\"
[ [1, 2], [3, 4] ]     // matrix literal
true
null
```
**Invalid**
```ipl
.5           // leading '.' not permitted
\"unclosed    // missing closing quote
```

### 5.2.4 Comments
- Single‑line: `// ...` to end of line.
- Multi‑line: `/* ... */`, nestable.
- Doc: `///` begins a documentation line associated with the next declaration.

### 5.2.5 Whitespace and Semicolons
- Whitespace separates tokens and has no semantic meaning except within strings.
- Statements **shall** end with a semicolon `;`. Implementations **may** support optional semicolon insertion as an extension but must diagnose when it changes meaning.

### 5.2.6 Operators and Punctuators
Punctuators include `() [] {} , ; : . :: => <- ->` and others defined in Clause 12. Additional punctuators for memory safety: `&` (borrow), `*` (dereference), `^` (exclusive ownership, ownership transfer).

IPL supports user-defined operators enclosed in Unicode angle brackets `«...»`, allowing for custom symbolic operators like `«dot»`, `«outer»`, etc.

# 15 — Portability: Implementation‑Defined and Undefined Behavior (Normative)

## 15.1 Implementation‑Defined
Implementations **shall** document:
- Numeric precision and range for `number` (e.g., IEEE‑754 double).
- Maximum recursion depth.
- File encoding handling beyond UTF‑8.
- Size and alignment of built‑in value types, if exposed.

## 15.2 Undefined Behavior (UB) — Programs shall not rely on these
- Division by zero when not trapped (if implementation chooses non‑trapping behavior).
- Out‑of‑bounds indexing without bounds checks in “unsafe” modes.
- Dereferencing `null` references.
- Concurrent data races (if concurrency extension is provided).

## 15.3 Locale and Internationalization
String collation and case mapping are implementation‑defined unless specified by the standard library profile.

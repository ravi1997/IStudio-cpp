# 04 — Conformance (Normative)

## 4.1 Conforming Implementations
An implementation **shall**:
- Accept source encoded in UTF‑8.
- Recognize all tokens and grammar productions in this standard.
- Enforce static typing rules, concept constraints, and contract semantics.
- Implement memory safety either through garbage collection or ownership/borrowing checking.
- Support null safety with compile-time checking of nullable types.
- Implement all standard library modules specified in Clause 16.
- Document all implementation‑defined behaviors (see Clause 15).
- Diagnose all **ill‑formed** programs (violations detectable at translation time).
- Support both compile-time and runtime error detection as specified.
- Implement template and concept checking for generic programming.
- Support decorator processing if decorators are implemented.
- Provide diagnostics for ownership and borrowing violations.

## 4.2 Conforming Programs
A program **shall not** rely on undefined behavior and **shall** obey all constraints marked *shall/must*. Use of implementation‑defined features is permitted.
- Programs **shall** follow ownership rules to avoid use-after-move and double-free errors.
- Programs **shall** properly handle nullable types to prevent null pointer exceptions.
- Programs **shall** satisfy concept constraints when using generic types.
- Programs **shall** implement all required interface methods when implementing interfaces.
- Programs **shall** respect borrowing rules to avoid data races in safe code.

## 4.3 Diagnostics
For each ill‑formed construct in this standard marked “**requires diagnostic**”, an implementation **shall** emit at least one diagnostic containing the source location and a description.
- Implementations **shall** provide specific error messages for ownership violations.
- Implementations **shall** warn about potential null dereferences in safe code.
- Implementations **shall** provide detailed messages for concept constraint violations.
- Implementations **should** suggest fixes for common errors.

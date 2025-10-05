# 01 — Scope (Normative)

This standard specifies the **Impossible Programming Language (IPL)**, a strongly-typed, English-like programming language supporting imperative, functional, object-oriented, and declarative styles with C++-style OOP concepts (inheritance, operator overloading, templates, concepts, contracts), Python-style meta-classes and decorators, Java-style inheritance and exception handling, Dart-style null safety and const/final semantics, Rust-style memory safety, and MATLAB-style matrix/array operations.

It defines:
1. Lexical structure, tokens, and source encoding.
2. Syntactic grammar and operator precedence, including user-defined operators.
3. Static semantics: name binding, typing, overload resolution, generics, concepts, memory safety, meta-classes, interfaces, and nullability.
4. Runtime semantics: evaluation order, memory model, ownership and lifetimes, exceptions, and garbage collection options.
5. Conformance requirements for implementations and programs.
6. A comprehensive normative core library including I/O, math, collections, meta-programming, and matrix operations.

Out of scope:
- ABI, object file formats, linkers.
- Platform-specific threading and FFI (may be specified in future parts).
- Implementation-specific performance characteristics.

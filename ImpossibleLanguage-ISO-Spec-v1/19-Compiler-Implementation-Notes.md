# 19 — Compiler Implementation Notes (Informative)

## 19.1 Scanner
- Use Unicode‑aware identifier classes for international identifiers.
- Implement maximal‑munch with backtracking for `>=`, `==`, `=>`, `<<`, `>>`, etc.
- Support raw string literals with `r"..."` syntax.
- Recognize custom operators with unicode angle brackets `«...»`.

## 19.2 Parser
- Recommended: **Pratt parser** for expressions; **recursive descent** for statements/declarations.
- Support for pattern matching in match expressions.
- Parse decorator annotations before declarations.
- Handle ownership and borrowing syntax: `&`, `*`, `move`, `owned<T>`, `borrowed<T>`.

## 19.3 AST
Enhanced AST node types to support all language features:

Core nodes: Program, Module, Import, Export, VarDecl, ConstDecl, FinalDecl,
FuncDecl, ContractFunc, ClassDecl, InterfaceDecl, TraitDecl, MetaClassDecl, OperatorDecl, TemplateDecl, ConceptDecl,
Block, If, Match, While, Repeat, For, ForEach, ForEachKV, Try, Throw, Return, Break, Continue, UnsafeBlock, OwnedBlock,
Assign, OwnershipTransfer (Move, Borrow, Dereference),
Binary, Unary, Call, Index, MatrixIndex, MemberAccess, Literal, Identifier.

Enhanced nodes: Decorator, ContractClause (Require, Ensure, Invariant), 
Pattern (LiteralPattern, IdentifierPattern, TuplePattern, ConstructorPattern),
GenericType, ConceptConstraint, InheritanceSpec, TraitImpl.

## 19.4 Type Checker
- **Unification for generics and overload resolution**: Support concepts and constraint checking.
- **Nullability tracking (flow‑sensitive)**: Require checks before dereference using control flow analysis.
- **Ownership and Borrowing Checker**: Implement affine type system to enforce Rust-style ownership rules at compile time:
  - Each value has a single owner at a time
  - Borrowing rules: multiple immutable borrows OR one mutable borrow
  - Lifetime analysis to ensure references remain valid
- **Concept Satisfaction Checking**: Verify that types meet concept constraints before template instantiation.
- **Trait Implementation Checking**: Ensure classes properly implement required trait methods.
- **Interface Implementation Checking**: Verify contracts between interface and implementing classes.

## 19.5 Memory Safety Analysis
- **Lifetime Analysis**: Track the lifetime of owned and borrowed values to prevent use-after-move and dangling references.
- **Borrowing Checker**: Implement a sophisticated borrowing analysis similar to Rust's borrow checker to prevent data races at compile time.
- **Dropping Semantics**: For deterministic memory management, implement drop checker to ensure destructors are called appropriately.

## 19.6 IR and Optimization
- **Lower to Ownership-aware SSA**: Preserve ownership semantics in the intermediate representation.
- **Constant folding and propagation**: Including compile-time evaluation of constexpr functions.
- **Function inlining**: Especially for generic functions and trait method calls.
- **Specialization**: For generic functions based on concept constraints.
- **Dead code elimination**: Including unreachable match cases.
- **Matrix operation optimizations**: For MATLAB-style matrix operations.
- **Escape analysis**: To optimize memory allocation and potentially stack-allocate objects that don't escape their scope.

## 19.7 Code Generation
- **Multiple backend support**: GC-based for ease of use and ownership-based for performance/safety.
- **Trait method dispatch**: Virtual dispatch for trait objects, monomorphization for generic functions.
- **Exception handling**: Stack unwinding mechanism for exception propagation.
- **Async/await lowering**: Transform async functions into state machines.

## 19.8 Diagnostics
- **Rich, multi‑line messages** with primary and secondary spans; show candidate overloads on ambiguity.
- **Suggested fixes** for common ownership and borrowing errors.
- **Lifetime error explanations** for complex borrowing scenarios.
- **Concept constraint violation details** with specific requirements that weren't met.
- **Pattern matching exhaustiveness** checking to ensure all cases are covered.
- **Null safety diagnostics** with flow-sensitive analysis to reduce false positives.

## 19.9 Meta-Programming Support
- **Compile-time reflection**: Allow type introspection during compilation.
- **Macro system**: Support for procedural and declarative macros if implemented.
- **Template metaprogramming**: Enable computation at compile time through templates and concepts.

## 19.10 Debugging Support
- **Step-through debugging** for ownership and borrowing concepts.
- **Memory visualization** tools to help developers understand ownership transfers.
- **Lifetime visualization** to show when values are created, moved, and destroyed.

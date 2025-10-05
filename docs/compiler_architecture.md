# IStudio Compiler Architecture

## Goals

- Modular pipeline that supports lexing, syntax analysis, semantic analysis, intermediate representations, optimization, and code generation.
- Config-driven language support powered by grammar and translation files.
- Extensibility for future phases such as advanced optimizations, target-specific code generators, and tooling integration.

## Pipeline Overview

1. **Configuration Loading**
   - Load grammar, translation, and source files using `Config`.
   - Preprocess grammar rules into token classification helpers.

2. **Lexical Analysis**
   - `Lexer` converts the input source into a stream of `Token` objects.
   - Diagnostics recorded via `DiagnosticEngine` for invalid characters or malformed tokens.

3. **Syntax Analysis**
   - `Parser` consumes tokens to produce an abstract syntax tree (`AST`).
   - Grammar-driven parsing, parameterizable by configuration.

4. **Semantic Analysis**
   - `SemanticAnalyzer` validates symbol definitions, types, and scopes.
   - Populates `SymbolTable` with rich metadata.

5. **Intermediate Representation (IR)**
   - `IRModule` / `IRBuilder` translate AST into a target-neutral IR.
   - Translation rules guide mapping from language constructs to IR nodes.

6. **Optimization Pipeline**
   - Pass manager orchestrates optimizations on IR (constant folding, dead-code elimination, etc.).
   - Designed for future expansion via pass registry.

7. **Code Generation**
   - Backends convert optimized IR into target outputs (e.g., pseudo-assembly, LLVM IR, WebAssembly).
   - Translation rules select backend strategies.

8. **Diagnostics & Reporting**
   - Unified diagnostics across all phases with source ranges.
   - Summaries and symbol reports for user feedback.

## Key Abstractions

- `DiagnosticEngine` – Collects warnings/errors and exposes `PhaseResult<T>` that can short-circuit on failure.
- `PipelineContext` – Carries shared state (configuration, symbol tables, IR).
- `CompilerPhase` concept – Each phase implements `PhaseResult<void> run(PipelineContext&)`.
- `PassManager` – Registers and executes optimization/codegen passes.

## Extensibility Hooks

- Grammar/translation files provide language-specific configuration without recompilation.
- Phases discover optional passes via configuration and plugin registry (future work).
- Code generation backends selected via command-line flags or config entries.

## Testing Strategy

- Unit tests per phase (lexer tokens, parser constructs, semantic rules).
- Integration tests running full pipeline on sample programs.
- Golden-file tests for generated code outputs.

## Future Enhancements

- Incremental compilation support with cached translation units.
- Advanced type inference and generics handling.
- Multi-threaded optimization pass scheduling.
- Language server protocol (LSP) integration using diagnostics and symbol data.

## IPL Grammar & Samples

- The complete, normative EBNF grammar for IPL now lives in `docs/ipl_full_grammar.ebnf`.
- A suite of runnable snippets covering core language features is available under `examples/ipl/`. The set currently includes
  20 focused programs spanning control flow, ownership, contracts, generics, traits, and meta-programming.
- Use `IStudio --lex-ipl-samples` to tokenize every sample with the bundled grammar and report token counts/diagnostics.
- `scripts/test_ipl_samples.sh` bootstraps the build and runs the same verification in one step.
- The parser now consumes structured tokens from the lexer, supporting conditionals, loops, function calls, unary operators, and expression statements, producing rich AST nodes for downstream phases.
- See `docs/usage.md` for the CLI reference, project manifest format, and the stub standard library shipped with the toolchain.

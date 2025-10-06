# IStudio Feature Matrix

This matrix summarizes the major capabilities present in the repository, their implementation status, and follow-up notes for contributors.

| Area | Functionality | Status | Notes |
| --- | --- | --- | --- |
| CLI | `compile <file>` | Implemented | Reads grammar/translation files via `Config`, primes stdlib, runs parser + semantic analyzer (`src/main.cpp`). |
| CLI | `run [--project file]` | Implemented | Loads `ipl_project.ini`, resolves relative paths, honours CLI overrides (`src/main.cpp`). |
| CLI | `compile --stdin` | Implemented | Persists stdin payload to `istudio-stdin-src.ipl`, compiles, then deletes file. |
| CLI | `--demo` | Implemented (legacy) | Compiles `examples/test_source.c` with sample grammar; use only for historical C-style demo. |
| CLI | `--lex-ipl-samples` | Implemented | Tokenizes every `examples/ipl/*.ipl` file with configured grammar; reports counts. |
| CLI | `--emit-sema` | Implemented | Runs semantic analyzer after parsing and prints scope summary. |
| CLI | `--standard` | Implemented (limited) | Only `ipl` / `default` resolve to bundled grammar; other values produce an error. |
| CLI | `--output` | Informational | Placeholder flag; no code generation emitted yet. |
| Pipeline | Lexical analysis | Placeholder | `src/istudio/Lexer.cpp` returns one token per line; grammar-driven lexer still TODO. |
| Pipeline | Parser | Implemented (MVP) | Handles functions, blocks, control flow, expressions; basic error recovery via `synchronize()`. |
| Pipeline | Semantic analysis | Implemented (MVP) | Tracks scopes, validates redeclarations, checks assignments to undefined identifiers (`src/semantic/`). |
| Pipeline | IR lowering | Placeholder | `ir/Lowering.cpp` creates module/function shells but does not translate statements yet. |
| Pipeline | Code generation | Not started | No backend emits target code; roadmap Phase 3. |
| Build | CMake preset (`Configure preset using toolchain file`) | Implemented | Provides Ninja generator, staged build/install directories (`CMakePresets.json`). |
| Build | Testing targets | Implemented | `CMakeLists.txt` registers CTest cases and custom `test_suite` target when scripts are present. |
| Samples | `examples/ipl` programs | Implemented | Curated set of valid programs covering language features. |
| Samples | `examples/ipl_invalid` | Implemented | Negative suite ensuring diagnostics for malformed programs. |
| Tooling | `scripts/run_ipl_suite.sh` | Implemented | Builds project and compiles valid + invalid sample suites. |
| Tooling | `scripts/test_ipl_samples.sh` | Implemented | Runs tokenizer over curated samples. |
| Diagnostics | `RUN_COMPILER_TEST=1` | Implemented | Environment flag triggers internal smoke test compiling a hardcoded C-style program. |
| Documentation | Architecture, usage, developer guides | Implemented | Updated regularly (`docs/compiler_architecture.md`, `docs/usage.md`, `docs/developer_guide.md`). |
| Documentation | Roadmap & status tracking | Implemented | `docs/roadmap_semantic_ir_codegen.md`, `docs/project_status.md`, this matrix. |
| Future work | Grammar-driven lexer | Planned (P0) | Captured in TODO backlog and roadmap Phase 1. |
| Future work | Typed AST + IR instructions | Planned (P1) | Required before IR lowering becomes functional. |

Use this table to verify features during code reviews and to spot areas where placeholder implementations remain.

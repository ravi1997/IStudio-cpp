# IStudio CLI Usage

## Quick Start

```
IStudio compile path/to/source.ipl
```

By default the compiler loads the bundled grammar (`examples/ipl/grammar_rules.txt`),
translation rules (`examples/translation_rules.txt`), and the stub standard library in
`stdlib/` before parsing your program. The resulting Abstract Syntax Tree (AST) and
collected symbols are printed when compilation succeeds.

## Commands

| Command | Description |
| --- | --- |
| `compile <source>` | Compile a single IPL source file. Use `--grammar`/`--translation` to override rule files. |
| `run [--project file]` | Compile a project described by an `ipl_project.ini` manifest. Paths inside the manifest are resolved relative to the file. |
| `lex-samples` | Tokenize every `examples/ipl/*.ipl` file with the bundled grammar and report token counts. |
| `--demo` | Compile the bundled C-style demo assets (legacy compatibility). |
| `--stdin` | Read source code from standard input, then compile using the selected grammar/translation files. |

### Global Options

- `--verbose, -v` – print detailed progress (AST, symbol table, stdlib files loaded).
- `--grammar, -g <file>` – specify a grammar rules file.
- `--translation, -t <file>` – specify a translation rules file.
- `--project, -p <file>` – project manifest for `run` (defaults to `./ipl_project.ini`).
- `--standard, -s <name>` – select a pre-defined grammar standard (`ipl`).
- `--output, -o <path>` – reserved for future code-generation targets (currently informational).
- `--lex-ipl-samples` – convenience flag identical to `lex-samples`.
- `--help`, `--version` – print usage or version information.

## Project Manifests

Create an `ipl_project.ini` at your project root:

```
# Minimal project file
source=src/main.ipl
grammar=config/grammar_rules.txt
translation=config/translation_rules.txt
```

Relative paths are resolved from the manifest directory. Grammar and translation entries
are optional; the compiler falls back to the bundled defaults when they are omitted.

Invoke the project with:

```
IStudio run
```

or specify a custom manifest:

```
IStudio run --project path/to/custom_project.ini
```

## Standard Library Stub

The `stdlib/` directory ships with lightweight IPL stubs for:

- `core.io` – console helpers (`print`, `println`, `printf`, `readFile`, ...)
- `core.math` – numeric utilities (`abs`, `clamp`, `min`, `max`, `pow`, `sqrt`, `sin`, `floor`, ...)
- `core.string` – string helpers (`length`, `concat`, `substring`, `indexOf`, `trim`, ...)
- `core.collections` – placeholder list/dict helpers for future collection support
- `core.ctype` – character classification/transforms (`isAlpha`, `toUpperChar`, ...)
- `core.memory` – byte buffer operations (`memcpy`, `memmove`, `memset`, ...)
- `core.meta` – reflection hooks such as `typeof`, `fields`, and `hasField`

These modules are lexed before your program so that imports can rely on the expected API
surface even though full implementations and code generation are not yet in place.

## Test Automation

Use the helper scripts in `scripts/` to exercise the toolchain:

- `scripts/test_ipl_samples.sh` – build the compiler and tokenize bundled samples.
- `scripts/run_ipl_suite.sh` – compile every valid program in `examples/ipl/` and ensure
  programs under `examples/ipl_invalid/` fail with diagnostics.

The scripts respect the new CLI options and will surface any parsing or lexing regressions.

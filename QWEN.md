# IStudio - Impossible Programming Language (IPL) Compiler

## Project Overview

IStudio is a conceptual compiler for the Impossible Programming Language (IPL), designed to experiment with next-generation C++ compilation concepts. It is a C++23 project built with CMake that provides both a standalone compiler (`ipl_compiler`) and a main IStudio executable with extended functionality.

The project features:
- Lexical analysis with configurable grammar rules
- Syntax analysis and Abstract Syntax Tree (AST) generation
- Semantic analysis with symbol table management
- Configurable language support through grammar and translation files
- Modular pipeline architecture

The IPL language supports modern programming constructs including modules, imports, functions with return types, variables with type annotations, control flow statements, and more. Examples of IPL syntax can be found in the `examples/ipl/` directory.

## Building and Running

### Prerequisites
- CMake 3.28.3 or higher
- C++23 compatible compiler (GCC 14+, Clang 16+)
- Git

### Building from Source
```bash
git clone https://github.com/your-username/IStudio.git
cd IStudio

mkdir build
cd build

cmake ..
make
```

For a release build:
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

### Running the Compiler
```bash
# Compile an IPL source file
./IStudio compile path/to/source.ipl

# Run with project manifest
./IStudio run

# Tokenize IPL samples
./IStudio --lex-ipl-samples

# Get help
./IStudio --help
```

### Standalone Compiler
```bash
./ipl_compiler input_file.ipl -o output_file
```

### Running Tests
The project includes a comprehensive test suite using CTest:
```bash
# Build the project and run all tests
cmake --build .
ctest

# Run tests with more verbose output
ctest --verbose

# Run specific tests
ctest -R "ipl_help_test"  # Run only the help test

# Run tests excluding specific ones
ctest -E "ipl_full_suite_test"

# Run with BUILD_TESTING enabled
cmake -DBUILD_TESTING=ON .
ctest
```

### Installation
To install the compiler system-wide:
```bash
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
make
sudo make install
```

Or to install to a custom directory:
```bash
cmake -DCMAKE_INSTALL_PREFIX=/path/to/install ..
make
make install
```

The installation includes:
- Executables in `bin/`
- Header files in `include/IStudio/`
- Examples in `share/IStudio/examples/`
- Standard library in `share/IStudio/stdlib/`
- Documentation in `share/doc/IStudio/`
- CMake configuration files for dependency management

## Development Conventions

### Code Structure
The project follows a modular architecture:
- `src/` - Source code
  - `ipl_compiler/` - Standalone compiler implementation
  - `istudio/` - IStudio-specific components
  - `semantic/` - Semantic analysis components
  - `ir/` - Intermediate representation components
- `include/` - Header files
- `examples/` - IPL example programs
- `stdlib/` - Standard library stubs
- `docs/` - Documentation
- `scripts/` - Utility scripts

### Coding Standards
- Uses C++23 with `-Wall -Wextra -Wpedantic` compiler flags
- Implements RAII and smart pointers for memory management
- Follows modern CMake practices with exported targets
- Uses `std::expected` or similar for error handling (with `std::unexpected`)
- Includes comprehensive diagnostics with range information

### Testing Practices
- Comprehensive test suite using CTest framework
- Tests cover basic functionality, lexing, compilation, expected failures, and integration
- Includes both valid and invalid program tests
- Custom test scripts for complex scenarios
- Uses both CTest native tests and custom test execution scripts

### Available CMake Targets
- `IStudio` - Main IPL compiler executable
- `ipl_compiler` - Standalone compiler executable
- `sample_tests` - Run sample IPL tests
- `install` - Install the project
- `install-all` - Build and install everything
- `test_suite` - Run full test suite script

## File Structure
- `src/` - Contains all source files for the compiler components
- `include/` - Header files defining interfaces and core types
- `examples/` - IPL example programs and grammar/translation rules
- `stdlib/` - Standard library stubs for IPL
- `docs/` - Documentation files
- `scripts/` - Shell scripts for running tests and other utilities
- `tests/` - Unit and integration tests
- `cmake/` - CMake modules and configuration files
- `CMakeLists.txt` - Main build configuration
- `README.md` - Project overview and usage information
- `package-lock.json` - JavaScript package metadata
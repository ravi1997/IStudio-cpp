# IStudio - Impossible Programming Language (IPL) Compiler

IStudio is a conceptual compiler for the Impossible Programming Language (IPL), designed to experiment with next-generation C++ compilation concepts.

## Features

- Lexical analysis with configurable grammar rules
- Syntax analysis and Abstract Syntax Tree (AST) generation
- Semantic analysis with symbol table management
- Configurable language support through grammar and translation files
- Modular pipeline architecture

## Building

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

## Installation

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

## Usage

### Main Compiler (IStudio)
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

### Standalone Compiler (ipl_compiler)
```bash
./ipl_compiler input_file.ipl -o output_file
```

## Available CMake Targets

- `IStudio` - Main IPL compiler executable
- `ipl_compiler` - Standalone compiler executable
- `sample_tests` - Run sample IPL tests
- `install` - Install the project
- `install-all` - Build and install everything
- `test_suite` - Run full test suite script

## Running Tests

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

### Test Categories

The test suite includes:
- **Basic functionality tests**: Help, version, basic operation
- **Lexing tests**: Tokenization of valid IPL samples
- **Compilation tests**: Both valid and invalid programs
- **Expected failure tests**: Invalid programs that should fail to compile
- **Integration tests**: Full test suite scripts

## Project Structure

- `src/` - Source code
  - `ipl_compiler/` - Standalone compiler implementation
  - `istudio/` - IStudio-specific components
  - `semantic/` - Semantic analysis components
- `include/` - Header files
- `examples/` - IPL example programs
- `stdlib/` - Standard library stubs
- `docs/` - Documentation
- `scripts/` - Utility scripts

## License

This project is licensed under the MIT License.

## Export and Dependency Management

The project provides CMake package configuration files that allow other projects to easily use IStudio as a dependency:

```cmake
find_package(IStudio REQUIRED)
target_link_libraries(my_target IStudio::IStudio)
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
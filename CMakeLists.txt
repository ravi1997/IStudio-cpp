cmake_minimum_required(VERSION 3.28.3)

# Project definition with version
project(IStudio 
    VERSION 1.0.0
    DESCRIPTION "Impossible Programming Language (IPL) Compiler"
    HOMEPAGE_URL "https://github.com/istudio-project/IStudio"
    LANGUAGES CXX)

# Set C++ standard requirements
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Compiler options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Define source and include directories
set(IPL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(IPL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Define the main IStudio executable
add_executable(IStudio 
    src/main.cpp
    src/AST.cpp
    src/Parser.cpp
    src/Lexer.cpp
    src/Config.cpp
    src/Symbol.cpp
    src/istudio/Lexer.cpp
    src/istudio/Diagnostics.cpp
    src/semantic/Type.cpp
    src/semantic/SymbolTable.cpp
    src/semantic/SemanticAnalyzer.cpp
    src/ir/IR.cpp
    src/ir/Lowering.cpp
    src/codegen/GenericCodeGenerator.cpp
    src/codegen/RuleParser.cpp
)

# Define the ipl_compiler executable
add_executable(ipl_compiler 
    src/ipl_compiler/ipl_compiler.cpp
)

# Set include directories for the main executable
target_include_directories(IStudio PRIVATE
    ${IPL_INCLUDE_DIR}
)

# Set include directories for ipl_compiler
target_include_directories(ipl_compiler PRIVATE
    ${IPL_INCLUDE_DIR}
)

# Set properties for both executables
set_target_properties(IStudio ipl_compiler PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Install targets
include(GNUInstallDirs)

# Install the main executable
install(TARGETS IStudio
    EXPORT IStudioTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install the ipl_compiler executable
install(TARGETS ipl_compiler
    EXPORT IStudioTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install header files
install(DIRECTORY ${IPL_INCLUDE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/IStudio
    FILES_MATCHING PATTERN "*.h"
)

# Install example files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples/
    DESTINATION ${CMAKE_INSTALL_DATADIR}/IStudio/examples
    FILES_MATCHING 
    PATTERN "*.ipl" 
    PATTERN "*.txt"
    PATTERN "*.c"
)

# Install standard library files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/
    DESTINATION ${CMAKE_INSTALL_DATADIR}/IStudio/stdlib
    FILES_MATCHING PATTERN "*.ipl"
)

# Install documentation
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs/
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
    FILES_MATCHING PATTERN "*.md"
)

# Install scripts
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/scripts/
    DESTINATION ${CMAKE_INSTALL_DATADIR}/IStudio/scripts
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    FILES_MATCHING PATTERN "*.sh"
)

# Export targets for find_package
install(EXPORT IStudioTargets
    FILE IStudioTargets.cmake
    NAMESPACE IStudio::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/IStudio
)

# Create package configuration files
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/IStudioConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/IStudioConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/IStudio
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/IStudioConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Install the config and version files
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/IStudioConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/IStudioConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/IStudio
)

# Create export set for use from build directory
export(EXPORT IStudioTargets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/IStudioTargets.cmake
    NAMESPACE IStudio::
)

# Option to build tests (disabled by default)
option(BUILD_TESTING "Build tests" OFF)
if(BUILD_TESTING)
    enable_testing()
endif()

# Enable testing framework
include(CTest)
enable_testing()

# Create a custom target for running tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_ipl_suite.sh)
    add_custom_target(test_suite
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_ipl_suite.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running IPL test suite"
        VERBATIM
    )
    set_target_properties(test_suite PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()

# Create a custom target for running sample tests  
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_ipl_samples.sh)
    add_custom_target(sample_tests
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_ipl_samples.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running IPL sample tests"
        VERBATIM
    )
    set_target_properties(sample_tests PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()

# Create a custom target for installation
add_custom_target(install-all
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target install
    COMMENT "Installing all targets"
    VERBATIM
)
set_target_properties(install-all PROPERTIES EXCLUDE_FROM_ALL TRUE)

# Define test cases using CTest framework

# Test basic functionality of IStudio
add_test(NAME ipl_help_test
    COMMAND $<TARGET_FILE:IStudio> --help
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_test(NAME ipl_version_test
    COMMAND $<TARGET_FILE:IStudio> --version
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# Test lexing of valid samples
add_test(NAME ipl_sample_lex_test
    COMMAND $<TARGET_FILE:IStudio> --lex-ipl-samples
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# Test with a simple valid program (with required grammar file)
add_test(NAME ipl_simple_compile_test
    COMMAND $<TARGET_FILE:IStudio> compile examples/ipl/00_hello_world.ipl --grammar examples/grammar_rules.txt --translation examples/translation_rules.txt
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# Define expected failures for invalid programs
add_test(NAME ipl_invalid_unterminated_string
    COMMAND $<TARGET_FILE:IStudio> compile examples/ipl_invalid/00_unterminated_string.ipl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
set_property(TEST ipl_invalid_unterminated_string PROPERTY WILL_FAIL TRUE)

add_test(NAME ipl_invalid_unknown_character
    COMMAND $<TARGET_FILE:IStudio> compile examples/ipl_invalid/01_unknown_character.ipl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
set_property(TEST ipl_invalid_unknown_character PROPERTY WILL_FAIL TRUE)

add_test(NAME ipl_invalid_missing_paren
    COMMAND $<TARGET_FILE:IStudio> compile examples/ipl_invalid/02_missing_paren.ipl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
set_property(TEST ipl_invalid_missing_paren PROPERTY WILL_FAIL TRUE)

add_test(NAME ipl_invalid_missing_semicolon
    COMMAND $<TARGET_FILE:IStudio> compile examples/ipl_invalid/03_missing_semicolon.ipl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
set_property(TEST ipl_invalid_missing_semicolon PROPERTY WILL_FAIL TRUE)

# Add test for demo functionality\nadd_test(NAME ipl_demo_test\n    COMMAND $<TARGET_FILE:IStudio> --demo\n    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}\n)\nset_tests_properties(ipl_demo_test PROPERTIES FIXTURES_REQUIRED demo_files)\n\n# Add tests for stdin functionality\nadd_test(NAME ipl_stdin_test\n    COMMAND bash -c \"echo 'module test; import core.io; function main() { print(\\\"Hello\\\"); }' | $<TARGET_FILE:IStudio> --stdin --grammar examples/grammar_rules.txt --translation examples/translation_rules.txt\"\n    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}\n)\n\n# Add tests for various valid programs\nadd_test(NAME ipl_variables_test\n    COMMAND $<TARGET_FILE:IStudio> compile examples/ipl/01_variables.ipl --grammar examples/grammar_rules.txt --translation examples/translation_rules.txt\n    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}\n)\n\nadd_test(NAME ipl_control_flow_test\n    COMMAND $<TARGET_FILE:IStudio> compile examples/ipl/02_control_flow_if.ipl --grammar examples/grammar_rules.txt --translation examples/translation_rules.txt\n    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}\n)\n\nadd_test(NAME ipl_function_contracts_test\n    COMMAND $<TARGET_FILE:IStudio> compile examples/ipl/05_function_contracts.ipl --grammar examples/grammar_rules.txt --translation examples/translation_rules.txt\n    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}\n)

# Custom test script targets as tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_ipl_suite.sh)
    add_test(NAME ipl_full_suite_test
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_ipl_suite.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_ipl_samples.sh)
    add_test(NAME ipl_samples_test
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_ipl_samples.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

# Set test timeout (120 seconds)
set_tests_properties(
    ipl_full_suite_test
    ipl_samples_test
    PROPERTIES TIMEOUT 120
)

# Installation summary message
message(STATUS "IStudio ${PROJECT_VERSION} Configuration:")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Available targets: IStudio, ipl_compiler, install, test_suite, sample_tests, install-all")
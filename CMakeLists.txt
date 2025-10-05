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
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
        add_subdirectory(tests)
    endif()
endif()

# Create a custom target for running tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_ipl_suite.sh)
    add_custom_target(test_suite
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_ipl_suite.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running IPL test suite"
    )
endif()

# Create a custom target for running sample tests  
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_ipl_samples.sh)
    add_custom_target(sample_tests
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_ipl_samples.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running IPL sample tests"
    )
endif()

# Create a custom target for installation
add_custom_target(install-all
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target install
    COMMENT "Installing all targets"
)

# Installation summary message
message(STATUS "IStudio ${PROJECT_VERSION} Configuration:")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Available targets: IStudio, ipl_compiler, install, test_suite, sample_tests, install-all")
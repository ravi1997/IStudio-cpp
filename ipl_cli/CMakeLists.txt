cmake_minimum_required(VERSION 3.15)
project(ipl_cli LANGUAGES)

# Find cargo/rustc
find_program(CARGO cargo)
find_program(RUSTC rustc)

if(NOT CARGO OR NOT RUSTC)
    message(WARNING "Rust compiler and Cargo are required to build IPL CLI")
    return()
endif()

# Build the CLI as a custom target
add_custom_target(
    ipl_cli_build_cargo ALL
    COMMAND cargo build --release
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building IPL CLI with Cargo"
)

# Make the CLI binary available in the main build directory
set(IPL_CLI_BINARY ${CMAKE_CURRENT_SOURCE_DIR}/target/release/ipl_cli)
set(IPL_CLI_INSTALL_DIR ${CMAKE_BINARY_DIR}/bin)

# Create the bin directory if it doesn't exist
file(MAKE_DIRECTORY ${IPL_CLI_INSTALL_DIR})

# Copy the built binary to the IStudio build directory after building
add_custom_command(
    TARGET ipl_cli_build_cargo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${IPL_CLI_BINARY}
        ${IPL_CLI_INSTALL_DIR}/ipl
    COMMENT "Installing IPL CLI as 'ipl'"
)

add_dependencies(ipl_all ipl_cli_build_cargo)
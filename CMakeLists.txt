# IStudio - A conceptual C++26 compiler
# NOTE: C++23 is the current standard, C++26 are future concepts 
# This project implements concepts for next-generation C++ compilation
cmake_minimum_required(VERSION 3.28.3)

PROJECT(IStudio)
set(CMAKE_CXX_STANDARD 23)  # Using C++23 standard  
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ISTUDIO_BUILD_IPL_CLI "Build the experimental Rust-based IPL CLI" OFF)

# Collect source files
add_subdirectory(src)

# Add IPL CLI if available
if(ISTUDIO_BUILD_IPL_CLI AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ipl_cli/CMakeLists.txt)
    # Define a target that includes the CLI build
    add_custom_target(ipl_all)
    add_subdirectory(ipl_cli)
endif()

# Note: include directory only contains headers, no need to add_subdirectory
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Add include directory to the compiler search path
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

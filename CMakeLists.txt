# IStudio - A conceptual C++26 compiler
# NOTE: C++26 and CMake 4.0 are future standards
# This project implements concepts for next-generation C++ compilation
cmake_minimum_required(VERSION 3.28.3)

PROJECT(IStudio)
set(CMAKE_CXX_STANDARD 23)  # Using C++23 as C++26 doesn't exist yet  
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Collect source files
add_subdirectory(src)
# Note: include directory only contains headers, no need to add_subdirectory

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${INC_DIRS})
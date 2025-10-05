set(SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Symbol.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AST.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Lexer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/istudio/Lexer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/istudio/Diagnostics.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/semantic/Type.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/semantic/SymbolTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/semantic/SemanticAnalyzer.cpp
    PARENT_SCOPE
)

# Add IPL compiler executable
add_executable(ipl_compiler
    ${CMAKE_CURRENT_SOURCE_DIR}/ipl_compiler/ipl_compiler.cpp
)

# Install the IPL compiler to the build directory
install(TARGETS ipl_compiler DESTINATION bin)

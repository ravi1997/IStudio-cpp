#pragma once

#include "CodeGenerator.h"
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <regex>

namespace istudio {
namespace codegen {

// Structure to hold rules for mapping IPL constructs to target language
struct CodeGenerationRule {
    std::string nodeType;  // AST node type (e.g., "Function", "VariableDeclaration", etc.)
    std::string templateString;  // Template for generating code
    std::unordered_map<std::string, std::string> mappings;  // Type/keyword mappings
};

class GenericCodeGenerator : public CodeGenerator {
public:
    explicit GenericCodeGenerator(const std::string& targetLanguage);
    virtual ~GenericCodeGenerator() = default;

    // Method to load rules from a configuration
    void loadRules(const std::vector<CodeGenerationRule>& rules);
    
    // Main entry point to generate code from AST
    std::string generate(const ASTNode& node) override;
    
    // Generate code for a complete program
    std::string generateProgram(const ProgramNode& program) override;
    
    // Generate code for a function
    std::string generateFunction(const FunctionNode& function) override;
    
    // Generate code for a variable declaration
    std::string generateVariableDeclaration(const VariableDeclarationNode& varDecl) override;
    
    // Generate code for an assignment
    std::string generateAssignment(const AssignmentNode& assignment) override;
    
    // Generate code for a binary operation
    std::string generateBinaryOperation(const BinaryOperationNode& binOp) override;
    
    // Generate code for a unary operation
    std::string generateUnaryOperation(const UnaryOperationNode& unaryOp) override;
    
    // Generate code for a function call
    std::string generateCallExpression(const CallExpressionNode& call) override;
    
    // Generate code for a literal
    std::string generateLiteral(const LiteralNode& literal) override;
    
    // Generate code for an identifier
    std::string generateIdentifier(const IdentifierNode& identifier) override;
    
    // Generate code for a block
    std::string generateBlock(const BlockNode& block) override;
    
    // Generate code for a return statement
    std::string generateReturn(const ReturnNode& ret) override;
    
    // Generate code for an expression statement
    std::string generateExpressionStatement(const ExpressionStatementNode& exprStmt) override;
    
    // Generate code for an if statement
    std::string generateIf(const IfNode& ifNode) override;
    
    // Generate code for a while loop
    std::string generateWhile(const WhileNode& whileNode) override;
    
    // Generate code for a for loop
    std::string generateFor(const ForNode& forNode) override;

private:
    std::string targetLanguageName_;
    std::unordered_map<std::string, CodeGenerationRule> rules_;
    
    // Helper method to replace placeholders in templates
    std::string applyTemplate(const std::string& tmpl, 
                             const std::unordered_map<std::string, std::string>& replacements) const;
    
    // Method to get type mapping for the target language
    std::string mapType(const std::string& iplType) const;
    
    // Method to get keyword mapping for the target language
    std::string mapKeyword(const std::string& iplKeyword) const;
};

} // namespace codegen
} // namespace istudio
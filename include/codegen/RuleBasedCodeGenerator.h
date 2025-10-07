#pragma once

#include "../include/AST.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>

namespace istudio {
namespace codegen {

// Enum for supported target languages
enum class TargetLanguage {
    C,
    CPP,
    JAVA,
    PYTHON,
    UNKNOWN
};

// Structure to hold transformation rules
struct CodegenRule {
    std::string pattern;  // Pattern to match in IPL AST
    std::string replacement;  // Replacement code for the target language
    std::function<std::string(const ASTNode&)> generator;  // Custom code generator function
};

class RuleBasedCodeGenerator {
public:
    explicit RuleBasedCodeGenerator(TargetLanguage target);
    virtual ~RuleBasedCodeGenerator() = default;
    
    // Main entry point to generate code from AST
    virtual std::string generate(const ASTNode& node);
    
    // Generate code for a complete program
    virtual std::string generateProgram(const ProgramNode& program);
    
    // Generate code for a function
    virtual std::string generateFunction(const FunctionNode& function);
    
    // Generate code for a variable declaration
    virtual std::string generateVariableDeclaration(const VariableDeclarationNode& varDecl);
    
    // Generate code for an assignment
    virtual std::string generateAssignment(const AssignmentNode& assignment);
    
    // Generate code for a binary operation
    virtual std::string generateBinaryOperation(const BinaryOperationNode& binOp);
    
    // Generate code for a unary operation
    virtual std::string generateUnaryOperation(const UnaryOperationNode& unaryOp);
    
    // Generate code for a function call
    virtual std::string generateCallExpression(const CallExpressionNode& call);
    
    // Generate code for a literal
    virtual std::string generateLiteral(const LiteralNode& literal);
    
    // Generate code for an identifier
    virtual std::string generateIdentifier(const IdentifierNode& identifier);
    
    // Generate code for a block
    virtual std::string generateBlock(const BlockNode& block);
    
    // Generate code for a return statement
    virtual std::string generateReturn(const ReturnNode& ret);
    
    // Generate code for an expression statement
    virtual std::string generateExpressionStatement(const ExpressionStatementNode& exprStmt);
    
    // Generate code for an if statement
    virtual std::string generateIf(const IfNode& ifNode);
    
    // Generate code for a while loop
    virtual std::string generateWhile(const WhileNode& whileNode);
    
    // Generate code for a for loop
    virtual std::string generateFor(const ForNode& forNode);
    
    // Load language-specific rules
    void loadLanguageRules(TargetLanguage target);
    
    // Add a custom rule
    void addRule(const std::string& nodeType, const CodegenRule& rule);
    
    // Get the target language of this generator
    TargetLanguage getTargetLanguage() const { return targetLanguage_; }

private:
    TargetLanguage targetLanguage_;
    std::unordered_map<std::string, std::vector<CodegenRule>> rules_;
    
    // Helper to get appropriate rule for a node type
    std::string applyRule(const std::string& nodeType, const ASTNode& node);
};

} // namespace codegen
} // namespace istudio
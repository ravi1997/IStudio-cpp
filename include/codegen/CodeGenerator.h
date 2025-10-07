#pragma once

#include "../include/AST.h"
#include <string>
#include <memory>
#include <sstream>
#include <vector>

namespace istudio {
namespace codegen {

// Enum for supported target languages
enum class TargetLanguage {
    C,
    CPP,
    JAVA,
    PYTHON
};

// Abstract base class for code generators
class CodeGenerator {
public:
    explicit CodeGenerator(TargetLanguage target) : targetLanguage_(target) {}
    virtual ~CodeGenerator() = default;
    
    // Main entry point to generate code from AST
    virtual std::string generate(const ASTNode& node) = 0;
    
    // Generate code for a complete program
    virtual std::string generateProgram(const ProgramNode& program) = 0;
    
    // Generate code for a function
    virtual std::string generateFunction(const FunctionNode& function) = 0;
    
    // Generate code for a variable declaration
    virtual std::string generateVariableDeclaration(const VariableDeclarationNode& varDecl) = 0;
    
    // Generate code for an assignment
    virtual std::string generateAssignment(const AssignmentNode& assignment) = 0;
    
    // Generate code for a binary operation
    virtual std::string generateBinaryOperation(const BinaryOperationNode& binOp) = 0;
    
    // Generate code for a unary operation
    virtual std::string generateUnaryOperation(const UnaryOperationNode& unaryOp) = 0;
    
    // Generate code for a function call
    virtual std::string generateCallExpression(const CallExpressionNode& call) = 0;
    
    // Generate code for a literal
    virtual std::string generateLiteral(const LiteralNode& literal) = 0;
    
    // Generate code for an identifier
    virtual std::string generateIdentifier(const IdentifierNode& identifier) = 0;
    
    // Generate code for a block
    virtual std::string generateBlock(const BlockNode& block) = 0;
    
    // Generate code for a return statement
    virtual std::string generateReturn(const ReturnNode& ret) = 0;
    
    // Generate code for an expression statement
    virtual std::string generateExpressionStatement(const ExpressionStatementNode& exprStmt) = 0;
    
    // Generate code for an if statement
    virtual std::string generateIf(const IfNode& ifNode) = 0;
    
    // Generate code for a while loop
    virtual std::string generateWhile(const WhileNode& whileNode) = 0;
    
    // Generate code for a for loop
    virtual std::string generateFor(const ForNode& forNode) = 0;
    
    // Get the target language of this generator
    TargetLanguage getTargetLanguage() const { return targetLanguage_; }

protected:
    TargetLanguage targetLanguage_;
    std::ostringstream output_;
};

} // namespace codegen
} // namespace istudio
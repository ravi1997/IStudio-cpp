#pragma once

#include "CodeGenerator.h"
#include <string>
#include <sstream>
#include <vector>

namespace istudio {
namespace codegen {

class PythonCodeGenerator : public CodeGenerator {
public:
    PythonCodeGenerator() : CodeGenerator(TargetLanguage::PYTHON) {}
    virtual ~PythonCodeGenerator() = default;
    
    std::string generate(const ASTNode& node) override;
    std::string generateProgram(const ProgramNode& program) override;
    std::string generateFunction(const FunctionNode& function) override;
    std::string generateVariableDeclaration(const VariableDeclarationNode& varDecl) override;
    std::string generateAssignment(const AssignmentNode& assignment) override;
    std::string generateBinaryOperation(const BinaryOperationNode& binOp) override;
    std::string generateUnaryOperation(const UnaryOperationNode& unaryOp) override;
    std::string generateCallExpression(const CallExpressionNode& call) override;
    std::string generateLiteral(const LiteralNode& literal) override;
    std::string generateIdentifier(const IdentifierNode& identifier) override;
    std::string generateBlock(const BlockNode& block) override;
    std::string generateReturn(const ReturnNode& ret) override;
    std::string generateExpressionStatement(const ExpressionStatementNode& exprStmt) override;
    std::string generateIf(const IfNode& ifNode) override;
    std::string generateWhile(const WhileNode& whileNode) override;
    std::string generateFor(const ForNode& forNode) override;
};

} // namespace codegen
} // namespace istudio
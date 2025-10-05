#pragma once

#include "AST.h"
#include "Diagnostics.h"
#include "semantic/SymbolTable.h"
#include "semantic/Type.h"

namespace istudio::semantic {

struct SemanticOptions {
    bool verbose{false};
};

class SemanticAnalyzer {
public:
    explicit SemanticAnalyzer(SemanticOptions options = {});

    bool analyze(const ProgramNode& program, DiagnosticEngine& diagnostics);

    [[nodiscard]] const SymbolScope::Ptr& globalScope() const noexcept { return globalScope_; }

private:
    void visit(const ASTNode& node);
    void visitProgram(const ProgramNode& node);
    void visitFunction(const FunctionNode& node);
    void visitBlock(const BlockNode& node);
    void visitVariableDeclaration(const VariableDeclarationNode& node);
    void visitAssignment(const AssignmentNode& node);
    void visitBinary(const BinaryOperationNode& node);
    void visitUnary(const UnaryOperationNode& node);
    void visitCall(const CallExpressionNode& node);
    void visitReturn(const ReturnNode& node);
    void visitExpressionStatement(const ExpressionStatementNode& node);
    void visitIf(const IfNode& node);
    void visitWhile(const WhileNode& node);
    void visitFor(const ForNode& node);

    void pushScope();
    void popScope();

    void report(DiagnosticSeverity severity, std::string message, const ASTNode& node);

private:
    SemanticOptions options_;
    TypeContext types_;
    SymbolScope::Ptr globalScope_;
    SymbolScope::Ptr currentScope_;
    DiagnosticEngine* diagnostics_{nullptr};
    bool success_{true};
};

} // namespace istudio::semantic

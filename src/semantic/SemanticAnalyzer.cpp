#include "semantic/SemanticAnalyzer.h"

#include <sstream>

namespace istudio::semantic {

SemanticAnalyzer::SemanticAnalyzer(SemanticOptions options)
    : options_(options), globalScope_(std::make_shared<SymbolScope>()), currentScope_(globalScope_)
{
}

bool SemanticAnalyzer::analyze(const ProgramNode& program, DiagnosticEngine& diagnostics)
{
    diagnostics_ = &diagnostics;
    success_ = true;
    globalScope_ = std::make_shared<SymbolScope>();
    currentScope_ = globalScope_;

    visitProgram(program);

    diagnostics_ = nullptr;
    currentScope_.reset();
    return success_;
}

void SemanticAnalyzer::visitProgram(const ProgramNode& node)
{
    for (const auto& fn : node.getFunctions()) {
        if (!fn) {
            continue;
        }
        visitFunction(static_cast<const FunctionNode&>(*fn));
    }
}

void SemanticAnalyzer::visitFunction(const FunctionNode& node)
{
    Symbol symbol{node.getName(), SymbolKind::Function, types_.getBuiltin(node.getReturnType())};
    if (!currentScope_->declare(symbol)) {
        report(DiagnosticSeverity::Error, "Function redeclared: " + node.getName(), node);
    }

    auto savedScope = currentScope_;
    currentScope_ = currentScope_->createChild();
    for (const auto& param : node.getParameters()) {
        Symbol paramSymbol{param.name, SymbolKind::Variable, types_.getBuiltin(param.type)};
        if (!currentScope_->declare(paramSymbol)) {
            report(DiagnosticSeverity::Error, "Parameter redeclared: " + param.name, node);
        }
    }

    if (const auto* body = node.getBody()) {
        visit(*body);
    }

    currentScope_ = savedScope;
}

void SemanticAnalyzer::visitBlock(const BlockNode& node)
{
    pushScope();
    for (const auto& stmt : node.getStatements()) {
        if (stmt) {
            visit(*stmt);
        }
    }
    popScope();
}

void SemanticAnalyzer::visitVariableDeclaration(const VariableDeclarationNode& node)
{
    Symbol symbol{node.getName(), SymbolKind::Variable, types_.getBuiltin(node.getTypeName())};
    if (!currentScope_->declare(symbol)) {
        report(DiagnosticSeverity::Error, "Variable redeclared: " + node.getName(), node);
    }

    if (const auto* init = node.getInitializer()) {
        visit(*init);
    }
}

void SemanticAnalyzer::visitAssignment(const AssignmentNode& node)
{
    auto symbol = currentScope_->lookup(node.getVariable());
    if (!symbol) {
        report(DiagnosticSeverity::Error, "Assignment to undefined identifier: " + node.getVariable(), node);
    }
    if (const auto* value = node.getValue()) {
        visit(*value);
    }
}

void SemanticAnalyzer::visitBinary(const BinaryOperationNode& node)
{
    if (const auto* left = node.getLeft()) {
        visit(*left);
    }
    if (const auto* right = node.getRight()) {
        visit(*right);
    }
}

void SemanticAnalyzer::visitUnary(const UnaryOperationNode& node)
{
    if (const auto* operand = node.getOperand()) {
        visit(*operand);
    }
}

void SemanticAnalyzer::visitCall(const CallExpressionNode& node)
{
    if (const auto* callee = node.getCallee()) {
        visit(*callee);
    }
    for (const auto& arg : node.getArguments()) {
        if (arg) {
            visit(*arg);
        }
    }
}

void SemanticAnalyzer::visitReturn(const ReturnNode& node)
{
    if (const auto* value = node.getValue()) {
        visit(*value);
    }
}

void SemanticAnalyzer::visitExpressionStatement(const ExpressionStatementNode& node)
{
    if (const auto* expr = node.getExpression()) {
        visit(*expr);
    }
}

void SemanticAnalyzer::visitIf(const IfNode& node)
{
    if (const auto* cond = node.getCondition()) {
        visit(*cond);
    }
    if (const auto* thenBranch = node.getThenBranch()) {
        visit(*thenBranch);
    }
    if (const auto* elseBranch = node.getElseBranch()) {
        visit(*elseBranch);
    }
}

void SemanticAnalyzer::visitWhile(const WhileNode& node)
{
    if (const auto* cond = node.getCondition()) {
        visit(*cond);
    }
    if (const auto* body = node.getBody()) {
        visit(*body);
    }
}

void SemanticAnalyzer::visitFor(const ForNode& node)
{
    pushScope();
    if (const auto* init = node.getInit()) {
        visit(*init);
    }
    if (const auto* cond = node.getCondition()) {
        visit(*cond);
    }
    if (const auto* inc = node.getIncrement()) {
        visit(*inc);
    }
    if (const auto* body = node.getBody()) {
        visit(*body);
    }
    popScope();
}

void SemanticAnalyzer::visit(const ASTNode& node)
{
    switch (node.getType()) {
    case ASTNodeType::Program:
        visitProgram(static_cast<const ProgramNode&>(node));
        break;
    case ASTNodeType::Function:
        visitFunction(static_cast<const FunctionNode&>(node));
        break;
    case ASTNodeType::Block:
        visitBlock(static_cast<const BlockNode&>(node));
        break;
    case ASTNodeType::VariableDeclaration:
        visitVariableDeclaration(static_cast<const VariableDeclarationNode&>(node));
        break;
    case ASTNodeType::Assignment:
        visitAssignment(static_cast<const AssignmentNode&>(node));
        break;
    case ASTNodeType::BinaryOperation:
        visitBinary(static_cast<const BinaryOperationNode&>(node));
        break;
    case ASTNodeType::UnaryOperation:
        visitUnary(static_cast<const UnaryOperationNode&>(node));
        break;
    case ASTNodeType::CallExpression:
        visitCall(static_cast<const CallExpressionNode&>(node));
        break;
    case ASTNodeType::Return:
        visitReturn(static_cast<const ReturnNode&>(node));
        break;
    case ASTNodeType::ExpressionStatement:
        visitExpressionStatement(static_cast<const ExpressionStatementNode&>(node));
        break;
    case ASTNodeType::If:
        visitIf(static_cast<const IfNode&>(node));
        break;
    case ASTNodeType::While:
        visitWhile(static_cast<const WhileNode&>(node));
        break;
    case ASTNodeType::For:
        visitFor(static_cast<const ForNode&>(node));
        break;
    case ASTNodeType::Literal:
    case ASTNodeType::Identifier:
        break;
    }
}

void SemanticAnalyzer::pushScope()
{
    currentScope_ = currentScope_->createChild();
}

void SemanticAnalyzer::popScope()
{
    if (auto parent = currentScope_->parent()) {
        currentScope_ = parent;
    }
}

void SemanticAnalyzer::report(DiagnosticSeverity severity, std::string message, const ASTNode&)
{
    success_ = false;
    if (!diagnostics_) {
        return;
    }
    diagnostics_->report(severity, std::move(message));
}

} // namespace istudio::semantic

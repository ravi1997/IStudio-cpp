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
    TypePtr returnType = types_.getBuiltin(node.getReturnType());
    if (!returnType && !node.getReturnType().empty()) {
        report(DiagnosticSeverity::Error, "Unknown return type: " + node.getReturnType(), node);
        returnType = types_.getBuiltin("void"); // fallback to void
    }
    
    Symbol symbol{node.getName(), SymbolKind::Function, returnType};
    if (!currentScope_->declare(symbol)) {
        report(DiagnosticSeverity::Error, "Function redeclared: " + node.getName(), node);
    }

    auto savedScope = currentScope_;
    currentScope_ = currentScope_->createChild();
    
    // Add parameters to scope with proper types
    for (const auto& param : node.getParameters()) {
        TypePtr paramType = types_.getBuiltin(param.type);
        if (!paramType) {
            report(DiagnosticSeverity::Error, "Unknown parameter type: " + param.type, node);
            paramType = types_.getBuiltin("any"); // fallback
        }
        
        Symbol paramSymbol{param.name, SymbolKind::Variable, paramType};
        if (!currentScope_->declare(paramSymbol)) {
            report(DiagnosticSeverity::Error, "Parameter redeclared: " + param.name, node);
        }
    }

    // Check for missing returns in functions with non-void return type
    if (returnType && returnType->name() != "void") {
        hasReturnStatement_ = false;
        if (const auto* body = node.getBody()) {
            visit(*body);
        }
        
        // If a non-void function doesn't have a guaranteed return statement,
        // report it (this is a simplified check)
        // In a full implementation, we'd need to check control flow paths
        if (!hasReturnStatement_) {
            // This is a basic check - in a full implementation we'd need to analyze all paths
            // For now, we'll just look at the last statement in a simple block
            if (const auto* bodyBlock = dynamic_cast<const BlockNode*>(node.getBody())) {
                const auto& statements = bodyBlock->getStatements();
                if (!statements.empty()) {
                    const auto& lastStmt = statements.back();
                    if (lastStmt->getType() != ASTNodeType::Return) {
                        report(DiagnosticSeverity::Error, 
                               "Function '" + node.getName() + "' with non-void return type must return a value", 
                               node);
                    }
                }
            }
        }
    } else {
        if (const auto* body = node.getBody()) {
            visit(*body);
        }
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
    TypePtr declaredType = types_.getBuiltin(node.getTypeName());
    if (!declaredType && !node.getTypeName().empty()) {
        report(DiagnosticSeverity::Error, "Unknown variable type: " + node.getTypeName(), node);
        declaredType = types_.getBuiltin("any"); // fallback
    }

    // Determine ownership based on type name or other cues
    OwnershipKind ownership = OwnershipKind::Unknown;
    if (node.getTypeName().find("owned") != std::string::npos) {
        ownership = OwnershipKind::Owned;
    } else if (node.getTypeName().find("borrowed") != std::string::npos) {
        ownership = OwnershipKind::Borrowed;
    } else if (node.getTypeName().find("ref") != std::string::npos) {
        ownership = OwnershipKind::Reference;
    }

    Symbol symbol{node.getName(), SymbolKind::Variable, declaredType, ownership, false, false};
    if (!currentScope_->declare(symbol)) {
        report(DiagnosticSeverity::Error, "Variable redeclared: " + node.getName(), node);
    }

    if (const auto* init = node.getInitializer()) {
        TypePtr initType = checkExpressionType(*init);
        if (declaredType && initType && declaredType->name() != initType->name() && 
            initType->name() != "null" && declaredType->name() != "any" && initType->name() != "any") {
            report(DiagnosticSeverity::Error, 
                   "Type mismatch: cannot assign " + initType->name() + " to " + declaredType->name(), 
                   node);
        }
        
        // Mark the symbol as initialized
        auto updatedSymbol = currentScope_->lookup(node.getName());
        if (updatedSymbol) {
            const_cast<Symbol*>(&updatedSymbol.value())->isInitialized = true;
        }
    }
}

void SemanticAnalyzer::visitAssignment(const AssignmentNode& node)
{
    auto symbol = currentScope_->lookup(node.getVariable());
    if (!symbol) {
        report(DiagnosticSeverity::Error, "Assignment to undefined identifier: " + node.getVariable(), node);
        return;
    }

    if (const auto* value = node.getValue()) {
        TypePtr assignedType = checkExpressionType(*value);
        if (symbol->type && assignedType && 
            symbol->type->name() != assignedType->name() &&
            assignedType->name() != "null" && symbol->type->name() != "any" && assignedType->name() != "any") {
            report(DiagnosticSeverity::Error, 
                   "Type mismatch: cannot assign " + assignedType->name() + " to " + symbol->type->name(), 
                   node);
        }
        
        // Mark the symbol as initialized
        const_cast<Symbol*>(&symbol.value())->isInitialized = true;
    }
}

TypePtr SemanticAnalyzer::checkExpressionType(const ASTNode& expr)
{
    switch (expr.getType()) {
    case ASTNodeType::Literal: {
        const auto& literal = static_cast<const LiteralNode&>(expr);
        // Determine literal type based on value
        const std::string& value = literal.getValue();
        if (value == "true" || value == "false") {
            return types_.getBuiltin("bool");
        } else if (value.find('.') != std::string::npos) {
            return types_.getBuiltin("float");
        } else {
            // Try to parse as integer
            try {
                std::stoi(value);
                return types_.getBuiltin("int");
            } catch (...) {
                return types_.getBuiltin("string");
            }
        }
    }
    case ASTNodeType::Identifier: {
        const auto& identifier = static_cast<const IdentifierNode&>(expr);
        auto symbol = currentScope_->lookup(identifier.getName());
        if (symbol) {
            // Check if it's an owned value that's been moved
            if (symbol->ownership == OwnershipKind::Owned && symbol->hasMoved) {
                report(DiagnosticSeverity::Error, 
                       "Use of moved value: " + identifier.getName(), expr);
            }
            return symbol->type;
        } else {
            report(DiagnosticSeverity::Error, "Undefined identifier: " + identifier.getName(), expr);
            return types_.getBuiltin("any");
        }
    }
    case ASTNodeType::BinaryOperation: {
        const auto& binary = static_cast<const BinaryOperationNode&>(expr);
        TypePtr leftType = checkExpressionType(*binary.getLeft());
        TypePtr rightType = checkExpressionType(*binary.getRight());
        
        // For now, assume binary operation result type is same as operands if they match
        if (leftType && rightType && leftType->name() == rightType->name()) {
            return leftType;
        }
        // For arithmetic operations, result is usually int or float
        const std::string& op = binary.getOperator();
        if (op == "+" || op == "-" || op == "*" || op == "/") {
            if (leftType->name() == "float" || rightType->name() == "float") {
                return types_.getBuiltin("float");
            }
            return types_.getBuiltin("int");
        }
        return types_.getBuiltin("bool"); // comparison ops return bool
    }
    case ASTNodeType::CallExpression: {
        const auto& call = static_cast<const CallExpressionNode&>(expr);
        if (const auto* callee = call.getCallee()) {
            if (callee->getType() == ASTNodeType::Identifier) {
                const auto& calleeId = static_cast<const IdentifierNode&>(*callee);
                auto symbol = currentScope_->lookup(calleeId.getName());
                if (symbol && symbol->kind == SymbolKind::Function) {
                    // For now return the function's return type
                    return symbol->type;
                }
            }
        }
        return types_.getBuiltin("any");
    }
    case ASTNodeType::UnaryOperation: {
        const auto& unary = static_cast<const UnaryOperationNode&>(expr);
        return checkExpressionType(*unary.getOperand());
    }
    default:
        return types_.getBuiltin("any");
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
    
    // Check type compatibility for binary operations
    TypePtr leftType = checkExpressionType(*node.getLeft());
    TypePtr rightType = checkExpressionType(*node.getRight());
    
    const std::string& op = node.getOperator();
    if (leftType && rightType && 
        leftType->name() != rightType->name() && 
        leftType->name() != "any" && rightType->name() != "any") {
        // Allow arithmetic operations between int and float
        if ((op == "+" || op == "-" || op == "*" || op == "/") &&
            ((leftType->name() == "int" && rightType->name() == "float") ||
             (leftType->name() == "float" && rightType->name() == "int"))) {
            // These are allowed
        } else {
            report(DiagnosticSeverity::Error, 
                   "Type mismatch in binary operation: " + leftType->name() + " " + op + " " + rightType->name(), 
                   node);
        }
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
        
        // Check if the callee is an identifier (function name)
        if (callee->getType() == ASTNodeType::Identifier) {
            const auto& calleeId = static_cast<const IdentifierNode&>(*callee);
            auto symbol = currentScope_->lookup(calleeId.getName());
            if (symbol && symbol->kind == SymbolKind::Function) {
                // Check argument count and types match function parameters
                if (symbol->type) {
                    // For now, just check that it's a function call
                    // More detailed function type checking would require proper function type info
                }
            } else {
                report(DiagnosticSeverity::Error, "Call to undefined function: " + calleeId.getName(), node);
            }
        }
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
        
        // In a real implementation, we would check the return type against the function's return type
        // For now we just check that the expression type is valid
        TypePtr returnType = checkExpressionType(*value);
        if (!returnType) {
            report(DiagnosticSeverity::Error, "Invalid return value type", node);
        }
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
        
        // Check that condition is boolean
        TypePtr condType = checkExpressionType(*cond);
        if (condType && condType->name() != "bool" && condType->name() != "any") {
            report(DiagnosticSeverity::Error, "If condition must be boolean", *cond);
        }
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
        
        // Check that condition is boolean
        TypePtr condType = checkExpressionType(*cond);
        if (condType && condType->name() != "bool" && condType->name() != "any") {
            report(DiagnosticSeverity::Error, "While condition must be boolean", *cond);
        }
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
        
        // Check that condition is boolean
        TypePtr condType = checkExpressionType(*cond);
        if (condType && condType->name() != "bool" && condType->name() != "any") {
            report(DiagnosticSeverity::Error, "For condition must be boolean", *cond);
        }
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
        hasReturnStatement_ = true;
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

void SemanticAnalyzer::report(DiagnosticSeverity severity, std::string message, const ASTNode& node)
{
    success_ = false;
    if (!diagnostics_) {
        return;
    }
    diagnostics_->report(severity, std::move(message));
}

} // namespace istudio::semantic

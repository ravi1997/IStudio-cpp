#include "../include/AST.h"
#include <iostream>
#include <sstream>

// Implementation of AST node print methods
void ProgramNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Program:\n";
    for (const auto& func : functions_) {
        func->print(indent + 1);
    }
}

void FunctionNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Function: " << return_type_ << ' ' << name_;
    std::ostringstream params;
    for (size_t i = 0; i < parameters_.size(); ++i) {
        params << parameters_[i].type << ' ' << parameters_[i].name;
        if (i + 1 < parameters_.size()) {
            params << ", ";
        }
    }
    std::cout << "(" << params.str() << ")\n";
    if (body_) {
        body_->print(indent + 1);
    }
}

void VariableDeclarationNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "VariableDeclaration: " << type_ << ' ' << name_ << "\n";
    if (initializer_) {
        initializer_->print(indent + 1);
    }
}

void AssignmentNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Assignment: " << variable_ << "\n";
    if (value_) {
        value_->print(indent + 1);
    }
}

void BinaryOperationNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "BinaryOperation: " << op_ << "\n";
    if (left_) {
        left_->print(indent + 1);
    }
    if (right_) {
        right_->print(indent + 1);
    }
}

void UnaryOperationNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "UnaryOperation: " << op_ << "\n";
    if (operand_) {
        operand_->print(indent + 1);
    }
}

void CallExpressionNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "CallExpression:\n";
    if (callee_) {
        callee_->print(indent + 1);
    }
    for (const auto& arg : arguments_) {
        if (arg) {
            arg->print(indent + 1);
        }
    }
}

void LiteralNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Literal: " << value_ << "\n";
}

void IdentifierNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Identifier: " << name_ << "\n";
}

void BlockNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Block:\n";
    for (const auto& statement : statements_) {
        statement->print(indent + 1);
    }
}

void ReturnNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Return";
    if (value_) {
        std::cout << ":\n";
        value_->print(indent + 1);
    } else {
        std::cout << "\n";
    }
}

void ExpressionStatementNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "ExpressionStatement:\n";
    if (expression_) {
        expression_->print(indent + 1);
    }
}

void IfNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "If:\n";
    if (condition_) {
        for (int i = 0; i < indent + 1; i++) std::cout << "  ";
        std::cout << "Condition:\n";
        condition_->print(indent + 2);
    }
    if (thenBranch_) {
        for (int i = 0; i < indent + 1; i++) std::cout << "  ";
        std::cout << "Then:\n";
        thenBranch_->print(indent + 2);
    }
    if (elseBranch_) {
        for (int i = 0; i < indent + 1; i++) std::cout << "  ";
        std::cout << "Else:\n";
        elseBranch_->print(indent + 2);
    }
}

void WhileNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "While:\n";
    if (condition_) {
        for (int i = 0; i < indent + 1; i++) std::cout << "  ";
        std::cout << "Condition:\n";
        condition_->print(indent + 2);
    }
    if (body_) {
        for (int i = 0; i < indent + 1; i++) std::cout << "  ";
        std::cout << "Body:\n";
        body_->print(indent + 2);
    }
}

void ForNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "For:\n";
    if (init_) {
        for (int i = 0; i < indent + 1; i++) std::cout << "  ";
        std::cout << "Init:\n";
        init_->print(indent + 2);
    }
    if (condition_) {
        for (int i = 0; i < indent + 1; i++) std::cout << "  ";
        std::cout << "Condition:\n";
        condition_->print(indent + 2);
    }
    if (increment_) {
        for (int i = 0; i < indent + 1; i++) std::cout << "  ";
        std::cout << "Increment:\n";
        increment_->print(indent + 2);
    }
    if (body_) {
        for (int i = 0; i < indent + 1; i++) std::cout << "  ";
        std::cout << "Body:\n";
        body_->print(indent + 2);
    }
}

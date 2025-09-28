#include "../include/AST.h"
#include <iostream>

// Implementation of AST node print methods
void ProgramNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Program:\
";
    for (const auto& func : functions_) {
        func->print(indent + 1);
    }
}

void FunctionNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Function: " << name_ << "\
";
    if (body_) {
        body_->print(indent + 1);
    }
}

void VariableDeclarationNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "VariableDeclaration: " << name_ << "\
";
    if (initializer_) {
        initializer_->print(indent + 1);
    }
}

void AssignmentNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Assignment: " << variable_ << "\
";
    if (value_) {
        value_->print(indent + 1);
    }
}

void BinaryOperationNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "BinaryOperation: " << op_ << "\
";
    if (left_) {
        left_->print(indent + 1);
    }
    if (right_) {
        right_->print(indent + 1);
    }
}

void LiteralNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Literal: " << value_ << "\
";
}

void IdentifierNode::print(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Identifier: " << name_ << "\
";
}
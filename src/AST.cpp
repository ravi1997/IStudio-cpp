#include "../include/AST.h"
#include <iostream>
#include <sstream>

namespace {

void writeIndent(int indent)
{
    for (int i = 0; i < indent; ++i) {
        std::cout << "  ";
    }
}

std::string formatParameters(const std::vector<FunctionParameter>& parameters)
{
    std::ostringstream oss;
    for (size_t i = 0; i < parameters.size(); ++i) {
        oss << parameters[i].type << ' ' << parameters[i].name;
        if (i + 1 < parameters.size()) {
            oss << ", ";
        }
    }
    return oss.str();
}

} // namespace

void ProgramNode::print(int indent) const {
    writeIndent(indent);
    std::cout << "Program:\n";
    for (const auto& func : functions_) {
        func->print(indent + 1);
    }
}

void FunctionNode::print(int indent) const {
    writeIndent(indent);
    std::cout << "Function: " << return_type_ << ' ' << name_;
    const auto signature = formatParameters(parameters_);
    if (!signature.empty()) {
        std::cout << "(" << signature << ")";
    } else {
        std::cout << "()";
    }
    std::cout << "\n";
    if (body_) {
        body_->print(indent + 1);
    }
}

void VariableDeclarationNode::print(int indent) const {
    writeIndent(indent);
    std::cout << "VariableDeclaration: " << type_ << ' ' << name_ << "\n";
    if (initializer_) {
        initializer_->print(indent + 1);
    }
}

void AssignmentNode::print(int indent) const {
    writeIndent(indent);
    std::cout << "Assignment: " << variable_ << "\n";
    if (value_) {
        value_->print(indent + 1);
    }
}

void BinaryOperationNode::print(int indent) const {
    writeIndent(indent);
    std::cout << "BinaryOperation: " << op_ << "\n";
    if (left_) {
        left_->print(indent + 1);
    }
    if (right_) {
        right_->print(indent + 1);
    }
}

void LiteralNode::print(int indent) const {
    writeIndent(indent);
    std::cout << "Literal: " << value_ << "\n";
}

void IdentifierNode::print(int indent) const {
    writeIndent(indent);
    std::cout << "Identifier: " << name_ << "\n";
}

void BlockNode::print(int indent) const {
    writeIndent(indent);
    std::cout << "Block:\n";
    for (const auto& statement : statements_) {
        statement->print(indent + 1);
    }
}

void ReturnNode::print(int indent) const {
    writeIndent(indent);
    std::cout << "Return";
    if (value_) {
        std::cout << ":\n";
        value_->print(indent + 1);
    } else {
        std::cout << "\n";
    }
}
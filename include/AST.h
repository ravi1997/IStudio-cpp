#pragma once
#include <string>
#include <vector>
#include <memory>

// Forward declarations
class ASTNode;
class Symbol;

// AST Node types
enum class ASTNodeType {
    Program,
    Function,
    VariableDeclaration,
    Assignment,
    BinaryOperation,
    Literal,
    Identifier
};

// Base AST Node class
class ASTNode {
public:
    ASTNode(ASTNodeType type) : type_(type) {}
    virtual ~ASTNode() = default;
    
    ASTNodeType getType() const { return type_; }
    virtual void print(int indent = 0) const = 0;

protected:
    ASTNodeType type_;
};

// Program node (root of AST)
class ProgramNode : public ASTNode {
public:
    ProgramNode() : ASTNode(ASTNodeType::Program) {}
    
    void addFunction(std::unique_ptr<ASTNode> func) {
        functions_.push_back(std::move(func));
    }
    
    void print(int indent = 0) const override;
    
private:
    std::vector<std::unique_ptr<ASTNode>> functions_;
};

// Function node
class FunctionNode : public ASTNode {
public:
    FunctionNode(const std::string& name, std::unique_ptr<ASTNode> body) 
        : ASTNode(ASTNodeType::Function), name_(name), body_(std::move(body)) {}
    
    void print(int indent = 0) const override;
    
private:
    std::string name_;
    std::unique_ptr<ASTNode> body_;
};

// Variable declaration node
class VariableDeclarationNode : public ASTNode {
public:
    VariableDeclarationNode(const std::string& name, std::unique_ptr<ASTNode> initializer) 
        : ASTNode(ASTNodeType::VariableDeclaration), name_(name), initializer_(std::move(initializer)) {}
    
    void print(int indent = 0) const override;
    
private:
    std::string name_;
    std::unique_ptr<ASTNode> initializer_;
};

// Assignment node
class AssignmentNode : public ASTNode {
public:
    AssignmentNode(const std::string& variable, std::unique_ptr<ASTNode> value) 
        : ASTNode(ASTNodeType::Assignment), variable_(variable), value_(std::move(value)) {}
    
    void print(int indent = 0) const override;
    
private:
    std::string variable_;
    std::unique_ptr<ASTNode> value_;
};

// Binary operation node
class BinaryOperationNode : public ASTNode {
public:
    BinaryOperationNode(const std::string& op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right) 
        : ASTNode(ASTNodeType::BinaryOperation), op_(op), left_(std::move(left)), right_(std::move(right)) {}
    
    void print(int indent = 0) const override;
    
private:
    std::string op_;
    std::unique_ptr<ASTNode> left_;
    std::unique_ptr<ASTNode> right_;
};

// Literal node
class LiteralNode : public ASTNode {
public:
    LiteralNode(const std::string& value) 
        : ASTNode(ASTNodeType::Literal), value_(value) {}
    
    void print(int indent = 0) const override;
    
private:
    std::string value_;
};

// Identifier node
class IdentifierNode : public ASTNode {
public:
    IdentifierNode(const std::string& name) 
        : ASTNode(ASTNodeType::Identifier), name_(name) {}
    
    void print(int indent = 0) const override;
    
private:
    std::string name_;
};
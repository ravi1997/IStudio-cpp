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
    Identifier,
    Block,
    Return
};

struct FunctionParameter {
    std::string type;
    std::string name;
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

    const std::vector<std::unique_ptr<ASTNode>>& getFunctions() const { return functions_; }
    
private:
    std::vector<std::unique_ptr<ASTNode>> functions_;
};

// Function node
class FunctionNode : public ASTNode {
public:
    FunctionNode(std::string returnType,
                 std::string name,
                 std::vector<FunctionParameter> parameters,
                 std::unique_ptr<ASTNode> body)
        : ASTNode(ASTNodeType::Function),
          return_type_(std::move(returnType)),
          name_(std::move(name)),
          parameters_(std::move(parameters)),
          body_(std::move(body)) {}
    
    void print(int indent = 0) const override;

    const std::string& getReturnType() const { return return_type_; }
    const std::string& getName() const { return name_; }
    const std::vector<FunctionParameter>& getParameters() const { return parameters_; }
    const ASTNode* getBody() const { return body_.get(); }
    
private:
    std::string return_type_;
    std::string name_;
    std::vector<FunctionParameter> parameters_;
    std::unique_ptr<ASTNode> body_;
};

// Variable declaration node
class VariableDeclarationNode : public ASTNode {
public:
    VariableDeclarationNode(std::string type,
                            std::string name,
                            std::unique_ptr<ASTNode> initializer) 
        : ASTNode(ASTNodeType::VariableDeclaration),
          type_(std::move(type)),
          name_(std::move(name)),
          initializer_(std::move(initializer)) {}
    
    void print(int indent = 0) const override;

    const std::string& getTypeName() const { return type_; }
    const std::string& getName() const { return name_; }
    const ASTNode* getInitializer() const { return initializer_.get(); }
    
private:
    std::string type_;
    std::string name_;
    std::unique_ptr<ASTNode> initializer_;
};

// Assignment node
class AssignmentNode : public ASTNode {
public:
    AssignmentNode(const std::string& variable, std::unique_ptr<ASTNode> value) 
        : ASTNode(ASTNodeType::Assignment), variable_(variable), value_(std::move(value)) {}
    
    void print(int indent = 0) const override;

    const std::string& getVariable() const { return variable_; }
    const ASTNode* getValue() const { return value_.get(); }
    
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

    const std::string& getOperator() const { return op_; }
    const ASTNode* getLeft() const { return left_.get(); }
    const ASTNode* getRight() const { return right_.get(); }
    
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

    const std::string& getValue() const { return value_; }
    
private:
    std::string value_;
};

// Identifier node
class IdentifierNode : public ASTNode {
public:
    IdentifierNode(const std::string& name) 
        : ASTNode(ASTNodeType::Identifier), name_(name) {}
    
    void print(int indent = 0) const override;

    const std::string& getName() const { return name_; }
    
private:
    std::string name_;
};

// Block node representing { ... }
class BlockNode : public ASTNode {
public:
    BlockNode() : ASTNode(ASTNodeType::Block) {}

    void addStatement(std::unique_ptr<ASTNode> statement) {
        if (statement) {
            statements_.push_back(std::move(statement));
        }
    }

    const std::vector<std::unique_ptr<ASTNode>>& getStatements() const { return statements_; }

    void print(int indent = 0) const override;

private:
    std::vector<std::unique_ptr<ASTNode>> statements_;
};

// Return statement node
class ReturnNode : public ASTNode {
public:
    explicit ReturnNode(std::unique_ptr<ASTNode> value)
        : ASTNode(ASTNodeType::Return), value_(std::move(value)) {}

    void print(int indent = 0) const override;

    const ASTNode* getValue() const { return value_.get(); }

private:
    std::unique_ptr<ASTNode> value_;
};
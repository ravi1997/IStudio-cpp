#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace istudio::ir {

// Forward declarations
class IRValue;
class IRInstruction;
class IRBasicBlock;
class IRFunction;
class IRModule;

// Base value class
class IRValue {
public:
    explicit IRValue(std::string name) : name_(std::move(name)) {}
    virtual ~IRValue() = default;
    
    const std::string& getName() const { return name_; }
    
protected:
    std::string name_;
};

// Instruction opcodes
enum class IRInstructionOp {
    Add,
    Sub,
    Mul,
    Div,
    Rem,
    And,
    Or,
    Xor,
    Shl,
    Shr,
    Eq,
    Ne,
    Lt,
    Le,
    Gt,
    Ge,
    Assign,
    Call,
    Return,
    Branch,
    BranchIf,
    Phi,
    Load,
    Store,
    Alloca,
    GetElementPtr
};

// Base instruction class
class IRInstruction : public IRValue {
public:
    explicit IRInstruction(std::string name, IRInstructionOp op)
        : IRValue(std::move(name)), op_(op) {}
    virtual ~IRInstruction() = default;
    
    IRInstructionOp getOp() const { return op_; }
    
private:
    IRInstructionOp op_;
};

// Basic block class
class IRBasicBlock : public IRValue {
public:
    explicit IRBasicBlock(std::string name)
        : IRValue(std::move(name)) {}
    
    void addInstruction(std::unique_ptr<IRInstruction> inst) {
        instructions_.push_back(std::move(inst));
    }
    
    const std::vector<std::unique_ptr<IRInstruction>>& getInstructions() const {
        return instructions_;
    }
    
private:
    std::vector<std::unique_ptr<IRInstruction>> instructions_;
};

// Function class
class IRFunction : public IRValue {
public:
    explicit IRFunction(std::string name)
        : IRValue(std::move(name)) {}
    
    void addBasicBlock(std::unique_ptr<IRBasicBlock> block) {
        basicBlocks_.push_back(std::move(block));
    }
    
    const std::vector<std::unique_ptr<IRBasicBlock>>& getBasicBlocks() const {
        return basicBlocks_;
    }
    
private:
    std::vector<std::unique_ptr<IRBasicBlock>> basicBlocks_;
};

// Module class
class IRModule {
public:
    explicit IRModule(std::string name)
        : name_(std::move(name)) {}
    
    void addFunction(std::unique_ptr<IRFunction> func) {
        functions_.push_back(std::move(func));
    }
    
    const std::vector<std::unique_ptr<IRFunction>>& getFunctions() const {
        return functions_;
    }
    
    const std::string& getName() const { return name_; }
    
private:
    std::string name_;
    std::vector<std::unique_ptr<IRFunction>> functions_;
};

} // namespace istudio::ir
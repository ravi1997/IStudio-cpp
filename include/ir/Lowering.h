#pragma once

#include "ir/IR.h"
#include "AST.h"
#include <memory>

namespace istudio::ir {

class LoweringPass {
public:
    explicit LoweringPass() = default;
    ~LoweringPass() = default;
    
    std::unique_ptr<IRModule> lower(const ProgramNode& program);
    
private:
    std::unique_ptr<IRFunction> lowerFunction(const FunctionNode& func);
    std::unique_ptr<IRBasicBlock> lowerBlock(const BlockNode& block);
    std::unique_ptr<IRInstruction> lowerInstruction(const ASTNode& node);
    
    std::unique_ptr<IRModule> module_;
};

} // namespace istudio::ir
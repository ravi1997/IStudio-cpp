#include \"CCodeGenerator.h\"
#include <sstream>
#include <string>
#include <vector>

namespace istudio {
namespace codegen {

std::string CCodeGenerator::generate(const ASTNode& node) {
    std::ostringstream oss;
    output_.str(\"\"); // Clear the stream
    output_.clear(); // Clear any error flags
    
    switch (node.getType()) {
        case ASTNodeType::Program:
            return generateProgram(static_cast<const ProgramNode&>(node));
        case ASTNodeType::Function:
            return generateFunction(static_cast<const FunctionNode&>(node));
        case ASTNodeType::VariableDeclaration:
            return generateVariableDeclaration(static_cast<const VariableDeclarationNode&>(node));
        case ASTNodeType::Assignment:
            return generateAssignment(static_cast<const AssignmentNode&>(node));
        case ASTNodeType::BinaryOperation:
            return generateBinaryOperation(static_cast<const BinaryOperationNode&>(node));
        case ASTNodeType::UnaryOperation:
            return generateUnaryOperation(static_cast<const UnaryOperationNode&>(node));
        case ASTNodeType::CallExpression:
            return generateCallExpression(static_cast<const CallExpressionNode&>(node));
        case ASTNodeType::Literal:
            return generateLiteral(static_cast<const LiteralNode&>(node));
        case ASTNodeType::Identifier:
            return generateIdentifier(static_cast<const IdentifierNode&>(node));
        case ASTNodeType::Block:
            return generateBlock(static_cast<const BlockNode&>(node));
        case ASTNodeType::Return:
            return generateReturn(static_cast<const ReturnNode&>(node));
        case ASTNodeType::ExpressionStatement:
            return generateExpressionStatement(static_cast<const ExpressionStatementNode&>(node));
        case ASTNodeType::If:
            return generateIf(static_cast<const IfNode&>(node));
        case ASTNodeType::While:
            return generateWhile(static_cast<const WhileNode&>(node));
        case ASTNodeType::For:
            return generateFor(static_cast<const ForNode&>(node));
        default:
            return \"// Unsupported node type in C code generation\\n\";
    }
}

std::string CCodeGenerator::generateProgram(const ProgramNode& program) {
    std::ostringstream oss;
    
    // Add standard headers
    oss << \"#include <stdio.h>\\n\";
    oss << \"#include <stdlib.h>\\n\\n\";
    
    // Generate each function in the program
    for (const auto& func : program.getFunctions()) {
        if (func) {
            oss << generate(*func) << \"\\n\";
        }
    }
    
    return oss.str();
}

std::string CCodeGenerator::generateFunction(const FunctionNode& function) {
    std::ostringstream oss;
    
    // Map IPL return type to C type
    std::string cReturnType = function.getReturnType();
    if (cReturnType == \"int\") {
        cReturnType = \"int\";
    } else if (cReturnType == \"float\") {
        cReturnType = \"float\";
    } else if (cReturnType == \"double\") {
        cReturnType = \"double\";
    } else if (cReturnType == \"bool\") {
        cReturnType = \"int\"; // C doesn't have bool by default
    } else if (cReturnType == \"string\" || cReturnType == \"char*\") {
        cReturnType = \"char*\";
    } else if (cReturnType == \"void\") {
        cReturnType = \"void\";
    } else {
        // Default to int for unknown types
        cReturnType = \"int\";
    }
    
    oss << cReturnType << \" \" << function.getName() << \"(\";
    
    // Generate parameters
    const auto& params = function.getParameters();
    for (size_t i = 0; i < params.size(); ++i) {
        if (i > 0) oss << \", \";
        
        // Map IPL parameter type to C type
        std::string cParamType = params[i].type;
        if (cParamType == \"int\" || cParamType == \"float\" || cParamType == \"double\" || 
            cParamType == \"bool\" || cParamType == \"string\" || cParamType == \"char*\") {
            if (cParamType == \"bool\") cParamType = \"int\";
            else if (cParamType == \"string\") cParamType = \"char*\";
        }
        
        oss << cParamType << \" \" << params[i].name;
    }
    oss << \")\";
    
    if (function.getBody()) {
        oss << \"\\n\" << generate(*function.getBody());
    } else {
        oss << \"; // Function declaration\\n\";
    }
    
    return oss.str();
}

std::string CCodeGenerator::generateVariableDeclaration(const VariableDeclarationNode& varDecl) {
    std::ostringstream oss;
    
    // Map IPL type to C type
    std::string cType = varDecl.getTypeName();
    if (cType == \"int\" || cType == \"float\" || cType == \"double\") {
        // Types already match
    } else if (cType == \"bool\") {
        cType = \"int\"; // C doesn't have bool by default
    } else if (cType == \"string\") {
        cType = \"char*\";
    } else {
        // Default to int for unknown types
        cType = \"int\";
    }
    
    oss << cType << \" \" << varDecl.getName();
    
    if (varDecl.getInitializer()) {
        oss << \" = \" << generate(*varDecl.getInitializer());
    }
    oss << \";\";
    
    return oss.str();
}

std::string CCodeGenerator::generateAssignment(const AssignmentNode& assignment) {
    std::ostringstream oss;
    oss << assignment.getVariable() << \" = \" << generate(*assignment.getValue()) << \";\";
    return oss.str();
}

std::string CCodeGenerator::generateBinaryOperation(const BinaryOperationNode& binOp) {
    std::ostringstream oss;
    oss << \"(\" << generate(*binOp.getLeft()) << \" \" << binOp.getOperator() << \" \" 
        << generate(*binOp.getRight()) << \")\";
    return oss.str();
}

std::string CCodeGenerator::generateUnaryOperation(const UnaryOperationNode& unaryOp) {
    std::ostringstream oss;
    oss << unaryOp.getOperator() << \"(\" << generate(*unaryOp.getOperand()) << \")\";
    return oss.str();
}

std::string CCodeGenerator::generateCallExpression(const CallExpressionNode& call) {
    std::ostringstream oss;
    
    if (call.getCallee()) {
        oss << generate(*call.getCallee()) << \"(\";
        
        const auto& args = call.getArguments();
        for (size_t i = 0; i < args.size(); ++i) {
            if (i > 0) oss << \", \";
            if (args[i]) {
                oss << generate(*args[i]);
            }
        }
        oss << \")\";
    }
    
    return oss.str();
}

std::string CCodeGenerator::generateLiteral(const LiteralNode& literal) {
    // For literals, we can directly return the value
    return literal.getValue();
}

std::string CCodeGenerator::generateIdentifier(const IdentifierNode& identifier) {
    // For identifiers, return the name
    return identifier.getName();
}

std::string CCodeGenerator::generateBlock(const BlockNode& block) {
    std::ostringstream oss;
    oss << \" {\\n\";
    
    for (const auto& stmt : block.getStatements()) {
        if (stmt) {
            oss << \"    \" << generate(*stmt) << \"\\n\";
        }
    }
    
    oss << \"}\";
    return oss.str();
}

std::string CCodeGenerator::generateReturn(const ReturnNode& ret) {
    std::ostringstream oss;
    oss << \"return\";
    
    if (ret.getValue()) {
        oss << \" \" << generate(*ret.getValue());
    }
    oss << \";\";
    
    return oss.str();
}

std::string CCodeGenerator::generateExpressionStatement(const ExpressionStatementNode& exprStmt) {
    std::ostringstream oss;
    if (exprStmt.getExpression()) {
        oss << generate(*exprStmt.getExpression()) << \";\";
    }
    return oss.str();
}

std::string CCodeGenerator::generateIf(const IfNode& ifNode) {
    std::ostringstream oss;
    oss << \"if (\";
    
    if (ifNode.getCondition()) {
        oss << generate(*ifNode.getCondition());
    }
    oss << \")\";
    
    if (ifNode.getThenBranch()) {
        oss << \" \" << generate(*ifNode.getThenBranch());
    }
    
    if (ifNode.getElseBranch()) {
        oss << \" else \" << generate(*ifNode.getElseBranch());
    }
    
    return oss.str();
}

std::string CCodeGenerator::generateWhile(const WhileNode& whileNode) {
    std::ostringstream oss;
    oss << \"while (\";
    
    if (whileNode.getCondition()) {
        oss << generate(*whileNode.getCondition());
    }
    oss << \")\";
    
    if (whileNode.getBody()) {
        oss << \" \" << generate(*whileNode.getBody());
    }
    
    return oss.str();
}

std::string CCodeGenerator::generateFor(const ForNode& forNode) {
    std::ostringstream oss;
    oss << \"for (\";
    
    if (forNode.getInit()) {
        oss << generate(*forNode.getInit()) << \" \";
    } else {
        oss << \"; \";
    }
    
    if (forNode.getCondition()) {
        oss << generate(*forNode.getCondition()) << \" \";
    } else {
        oss << \"; \";
    }
    
    if (forNode.getIncrement()) {
        oss << generate(*forNode.getIncrement());
    }
    oss << \")\";
    
    if (forNode.getBody()) {
        oss << \" \" << generate(*forNode.getBody());
    }
    
    return oss.str();
}

} // namespace codegen
} // namespace istudio
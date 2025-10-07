#include \"CppCodeGenerator.h\"
#include <sstream>
#include <string>
#include <vector>

namespace istudio {
namespace codegen {

std::string CppCodeGenerator::generate(const ASTNode& node) {
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
            return \"// Unsupported node type in C++ code generation\\n\";
    }
}

std::string CppCodeGenerator::generateProgram(const ProgramNode& program) {
    std::ostringstream oss;
    
    // Add standard headers
    oss << \"#include <iostream>\\n\";
    oss << \"#include <string>\\n\";
    oss << \"#include <vector>\\n\";
    oss << \"\\nusing namespace std;\\n\\n\";
    
    // Generate each function in the program
    for (const auto& func : program.getFunctions()) {
        if (func) {
            oss << generate(*func) << \"\\n\";
        }
    }
    
    return oss.str();
}

std::string CppCodeGenerator::generateFunction(const FunctionNode& function) {
    std::ostringstream oss;
    
    // Map IPL return type to C++ type
    std::string cppReturnType = function.getReturnType();
    if (cppReturnType == \"int\" || cppReturnType == \"float\" || cppReturnType == \"double\" || 
        cppReturnType == \"bool\" || cppReturnType == \"string\" || cppReturnType == \"void\") {
        // Types already match C++
    } else {
        // Default to int for unknown types
        cppReturnType = \"int\";
    }
    
    oss << cppReturnType << \" \" << function.getName() << \"(\";
    
    // Generate parameters
    const auto& params = function.getParameters();
    for (size_t i = 0; i < params.size(); ++i) {
        if (i > 0) oss << \", \";
        
        // Map IPL parameter type to C++ type
        std::string cppParamType = params[i].type;
        if (cppParamType == \"int\" || cppParamType == \"float\" || cppParamType == \"double\" || 
            cppParamType == \"bool\" || cppParamType == \"string\" || cppParamType == \"char*\") {
            if (cppParamType == \"char*\") cppParamType = \"string\";  // Use std::string instead of char*
        }
        
        oss << cppParamType << \" \" << params[i].name;
    }
    oss << \")\";
    
    if (function.getBody()) {
        oss << \"\\n\" << generate(*function.getBody());
    } else {
        oss << \"; // Function declaration\\n\";
    }
    
    return oss.str();
}

std::string CppCodeGenerator::generateVariableDeclaration(const VariableDeclarationNode& varDecl) {
    std::ostringstream oss;
    
    // Map IPL type to C++ type
    std::string cppType = varDecl.getTypeName();
    if (cppType == \"int\" || cppType == \"float\" || cppType == \"double\" || 
        cppType == \"bool\" || cppType == \"string\") {
        // Types already match C++
    } else if (cppType == \"char*\") {
        cppType = \"string\";  // Use std::string instead of char*
    } else {
        // Default to int for unknown types
        cppType = \"int\";
    }
    
    oss << cppType << \" \" << varDecl.getName();
    
    if (varDecl.getInitializer()) {
        oss << \" = \" << generate(*varDecl.getInitializer());
    }
    oss << \";\";
    
    return oss.str();
}

std::string CppCodeGenerator::generateAssignment(const AssignmentNode& assignment) {
    std::ostringstream oss;
    oss << assignment.getVariable() << \" = \" << generate(*assignment.getValue()) << \";\";
    return oss.str();
}

std::string CppCodeGenerator::generateBinaryOperation(const BinaryOperationNode& binOp) {
    std::ostringstream oss;
    oss << \"(\" << generate(*binOp.getLeft()) << \" \" << binOp.getOperator() << \" \" 
        << generate(*binOp.getRight()) << \")\";
    return oss.str();
}

std::string CppCodeGenerator::generateUnaryOperation(const UnaryOperationNode& unaryOp) {
    std::ostringstream oss;
    oss << unaryOp.getOperator() << \"(\" << generate(*unaryOp.getOperand()) << \")\";
    return oss.str();
}

std::string CppCodeGenerator::generateCallExpression(const CallExpressionNode& call) {
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

std::string CppCodeGenerator::generateLiteral(const LiteralNode& literal) {
    // For literals, we can directly return the value
    return literal.getValue();
}

std::string CppCodeGenerator::generateIdentifier(const IdentifierNode& identifier) {
    // For identifiers, return the name
    return identifier.getName();
}

std::string CppCodeGenerator::generateBlock(const BlockNode& block) {
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

std::string CppCodeGenerator::generateReturn(const ReturnNode& ret) {
    std::ostringstream oss;
    oss << \"return\";
    
    if (ret.getValue()) {
        oss << \" \" << generate(*ret.getValue());
    }
    oss << \";\";
    
    return oss.str();
}

std::string CppCodeGenerator::generateExpressionStatement(const ExpressionStatementNode& exprStmt) {
    std::ostringstream oss;
    if (exprStmt.getExpression()) {
        oss << generate(*exprStmt.getExpression()) << \";\";
    }
    return oss.str();
}

std::string CppCodeGenerator::generateIf(const IfNode& ifNode) {
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

std::string CppCodeGenerator::generateWhile(const WhileNode& whileNode) {
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

std::string CppCodeGenerator::generateFor(const ForNode& forNode) {
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
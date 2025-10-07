#include \"JavaCodeGenerator.h\"
#include <sstream>
#include <string>
#include <vector>

namespace istudio {
namespace codegen {

std::string JavaCodeGenerator::generate(const ASTNode& node) {
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
            return \"// Unsupported node type in Java code generation\\n\";
    }
}

std::string JavaCodeGenerator::generateProgram(const ProgramNode& program) {
    std::ostringstream oss;
    
    // Create a class to contain all functions
    oss << \"public class IPLProgram {\\n\\n\";
    
    // Generate each function in the program as a method
    for (const auto& func : program.getFunctions()) {
        if (func) {
            oss << \"    \" << generate(*func) << \"\\n\\n\";
        }
    }
    
    oss << \"}\\n\";
    return oss.str();
}

std::string JavaCodeGenerator::generateFunction(const FunctionNode& function) {
    std::ostringstream oss;
    
    // Map IPL return type to Java type
    std::string javaReturnType = function.getReturnType();
    if (javaReturnType == \"int\") {
        javaReturnType = \"int\";
    } else if (javaReturnType == \"float\") {
        javaReturnType = \"float\";
    } else if (javaReturnType == \"double\") {
        javaReturnType = \"double\";
    } else if (javaReturnType == \"bool\") {
        javaReturnType = \"boolean\";
    } else if (javaReturnType == \"string\" || javaReturnType == \"String\") {
        javaReturnType = \"String\";
    } else if (javaReturnType == \"void\") {
        javaReturnType = \"void\";
    } else {
        // Default to int for unknown types
        javaReturnType = \"int\";
    }
    
    oss << \"public static \" << javaReturnType << \" \" << function.getName() << \"(\";
    
    // Generate parameters
    const auto& params = function.getParameters();
    for (size_t i = 0; i < params.size(); ++i) {
        if (i > 0) oss << \", \";
        
        // Map IPL parameter type to Java type
        std::string javaParamType = params[i].type;
        if (javaParamType == \"int\" || javaParamType == \"float\" || javaParamType == \"double\") {
            // Types already match
        } else if (javaParamType == \"bool\") {
            javaParamType = \"boolean\";
        } else if (javaParamType == \"string\" || javaParamType == \"String\" || javaParamType == \"char*\") {
            javaParamType = \"String\";
        } else {
            javaParamType = \"int\"; // Default for unknown types
        }
        
        oss << javaParamType << \" \" << params[i].name;
    }
    oss << \")\";
    
    if (function.getBody()) {
        oss << \"\\n\" << generate(*function.getBody());
    } else {
        oss << \";\";
    }
    
    return oss.str();
}

std::string JavaCodeGenerator::generateVariableDeclaration(const VariableDeclarationNode& varDecl) {
    std::ostringstream oss;
    
    // Map IPL type to Java type
    std::string javaType = varDecl.getTypeName();
    if (javaType == \"int\" || javaType == \"float\" || javaType == \"double\") {
        // Types already match
    } else if (javaType == \"bool\") {
        javaType = \"boolean\";
    } else if (javaType == \"string\" || javaType == \"String\" || javaType == \"char*\") {
        javaType = \"String\";
    } else {
        // Default to int for unknown types
        javaType = \"int\";
    }
    
    oss << javaType << \" \" << varDecl.getName();
    
    if (varDecl.getInitializer()) {
        oss << \" = \" << generate(*varDecl.getInitializer());
    }
    oss << \";\";
    
    return oss.str();
}

std::string JavaCodeGenerator::generateAssignment(const AssignmentNode& assignment) {
    std::ostringstream oss;
    oss << assignment.getVariable() << \" = \" << generate(*assignment.getValue()) << \";\";
    return oss.str();
}

std::string JavaCodeGenerator::generateBinaryOperation(const BinaryOperationNode& binOp) {
    std::ostringstream oss;
    oss << \"(\" << generate(*binOp.getLeft()) << \" \" << binOp.getOperator() << \" \" 
        << generate(*binOp.getRight()) << \")\";
    return oss.str();
}

std::string JavaCodeGenerator::generateUnaryOperation(const UnaryOperationNode& unaryOp) {
    std::ostringstream oss;
    oss << unaryOp.getOperator() << \"(\" << generate(*unaryOp.getOperand()) << \")\";
    return oss.str();
}

std::string JavaCodeGenerator::generateCallExpression(const CallExpressionNode& call) {
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

std::string JavaCodeGenerator::generateLiteral(const LiteralNode& literal) {
    std::string value = literal.getValue();
    
    // Handle string literals in Java (need double quotes)
    if (value.length() >= 2 && value[0] == '\"' && value[value.length()-1] == '\"') {
        return value; // Already properly quoted
    }
    
    return value; // For numbers and other literals
}

std::string JavaCodeGenerator::generateIdentifier(const IdentifierNode& identifier) {
    // For identifiers, return the name
    return identifier.getName();
}

std::string JavaCodeGenerator::generateBlock(const BlockNode& block) {
    std::ostringstream oss;
    oss << \" {\\n\";
    
    for (const auto& stmt : block.getStatements()) {
        if (stmt) {
            oss << \"        \" << generate(*stmt) << \"\\n\";
        }
    }
    
    oss << \"    }\";
    return oss.str();
}

std::string JavaCodeGenerator::generateReturn(const ReturnNode& ret) {
    std::ostringstream oss;
    oss << \"return\";
    
    if (ret.getValue()) {
        oss << \" \" << generate(*ret.getValue());
    }
    oss << \";\";
    
    return oss.str();
}

std::string JavaCodeGenerator::generateExpressionStatement(const ExpressionStatementNode& exprStmt) {
    std::ostringstream oss;
    if (exprStmt.getExpression()) {
        oss << generate(*exprStmt.getExpression()) << \";\";
    }
    return oss.str();
}

std::string JavaCodeGenerator::generateIf(const IfNode& ifNode) {
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

std::string JavaCodeGenerator::generateWhile(const WhileNode& whileNode) {
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

std::string JavaCodeGenerator::generateFor(const ForNode& forNode) {
    std::ostringstream oss;
    oss << \"for (\";
    
    if (forNode.getInit()) {
        oss << generate(*forNode.getInit()) << \"; \";
    } else {
        oss << \"; \";
    }
    
    if (forNode.getCondition()) {
        oss << generate(*forNode.getCondition()) << \"; \";
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
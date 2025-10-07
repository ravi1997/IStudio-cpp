#include \"PythonCodeGenerator.h\"
#include <sstream>
#include <string>
#include <vector>

namespace istudio {
namespace codegen {

std::string PythonCodeGenerator::generate(const ASTNode& node) {
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
            return \"# Unsupported node type in Python code generation\\n\";
    }
}

std::string PythonCodeGenerator::generateProgram(const ProgramNode& program) {
    std::ostringstream oss;
    
    // Add standard imports if needed
    oss << \"# Generated Python code from IPL program\\n\\n\";
    
    // Generate each function in the program
    for (const auto& func : program.getFunctions()) {
        if (func) {
            oss << generate(*func) << \"\\n\\n\";
        }
    }
    
    return oss.str();
}

std::string PythonCodeGenerator::generateFunction(const FunctionNode& function) {
    std::ostringstream oss;
    
    oss << \"def \" << function.getName() << \"(\";
    
    // Generate parameters
    const auto& params = function.getParameters();
    for (size_t i = 0; i < params.size(); ++i) {
        if (i > 0) oss << \", \";
        oss << params[i].name;
    }
    oss << \"):\";
    
    if (function.getBody()) {
        // Python doesn't require explicit return type, so we just add the body
        std::string body = generate(*function.getBody());
        // Indent the body properly
        size_t pos = 0;
        while ((pos = body.find(\"\\n\", pos)) != std::string::npos) {
            body.insert(pos + 1, \"    \"); // Add 4 spaces for Python indentation
            pos += 5; // Move past the inserted spaces
        }
        oss << body;
    } else {
        oss << \"\\n    pass\"; // Empty function
    }
    
    return oss.str();
}

std::string PythonCodeGenerator::generateVariableDeclaration(const VariableDeclarationNode& varDecl) {
    std::ostringstream oss;
    
    oss << varDecl.getName();
    
    if (varDecl.getInitializer()) {
        oss << \" = \" << generate(*varDecl.getInitializer());
    } else {
        oss << \" = None\"; // Default to None if no initializer
    }
    
    return oss.str();
}

std::string PythonCodeGenerator::generateAssignment(const AssignmentNode& assignment) {
    std::ostringstream oss;
    oss << assignment.getVariable() << \" = \" << generate(*assignment.getValue());
    return oss.str();
}

std::string PythonCodeGenerator::generateBinaryOperation(const BinaryOperationNode& binOp) {
    std::ostringstream oss;
    oss << \"(\" << generate(*binOp.getLeft()) << \" \" << binOp.getOperator() << \" \" 
        << generate(*binOp.getRight()) << \")\";
    return oss.str();
}

std::string PythonCodeGenerator::generateUnaryOperation(const UnaryOperationNode& unaryOp) {
    std::ostringstream oss;
    oss << unaryOp.getOperator() << generate(*unaryOp.getOperand());
    return oss.str();
}

std::string PythonCodeGenerator::generateCallExpression(const CallExpressionNode& call) {
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

std::string PythonCodeGenerator::generateLiteral(const LiteralNode& literal) {
    // For literals, we can directly return the value
    return literal.getValue();
}

std::string PythonCodeGenerator::generateIdentifier(const IdentifierNode& identifier) {
    // For identifiers, return the name
    return identifier.getName();
}

std::string PythonCodeGenerator::generateBlock(const BlockNode& block) {
    std::ostringstream oss;
    oss << \"\\n\"; // Start with a newline
    
    for (const auto& stmt : block.getStatements()) {
        if (stmt) {
            oss << \"    \" << generate(*stmt) << \"\\n\"; // Python uses 4-space indentation
        }
    }
    
    return oss.str();
}

std::string PythonCodeGenerator::generateReturn(const ReturnNode& ret) {
    std::ostringstream oss;
    oss << \"return\";
    
    if (ret.getValue()) {
        oss << \" \" << generate(*ret.getValue());
    }
    
    return oss.str();
}

std::string PythonCodeGenerator::generateExpressionStatement(const ExpressionStatementNode& exprStmt) {
    std::ostringstream oss;
    if (exprStmt.getExpression()) {
        oss << generate(*exprStmt.getExpression());
    }
    return oss.str();
}

std::string PythonCodeGenerator::generateIf(const IfNode& ifNode) {
    std::ostringstream oss;
    oss << \"if \";
    
    if (ifNode.getCondition()) {
        oss << generate(*ifNode.getCondition());
    }
    oss << \":\";
    
    if (ifNode.getThenBranch()) {
        // Process the then branch, adding proper indentation
        std::string thenBranch = generate(*ifNode.getThenBranch());
        size_t pos = 0;
        while ((pos = thenBranch.find(\"\\n\", pos)) != std::string::npos) {
            if (pos + 1 < thenBranch.length()) { // If there's content after the newline
                thenBranch.insert(pos + 1, \"    \"); // Add 4 spaces for Python indentation
                pos += 5; // Move past the inserted spaces
            } else {
                break;
            }
        }
        oss << thenBranch;
    }
    
    if (ifNode.getElseBranch()) {
        oss << \"\\nelse:\";
        std::string elseBranch = generate(*ifNode.getElseBranch());
        size_t pos = 0;
        while ((pos = elseBranch.find(\"\\n\", pos)) != std::string::npos) {
            if (pos + 1 < elseBranch.length()) { // If there's content after the newline
                elseBranch.insert(pos + 1, \"    \"); // Add 4 spaces for Python indentation
                pos += 5; // Move past the inserted spaces
            } else {
                break;
            }
        }
        oss << elseBranch;
    }
    
    return oss.str();
}

std::string PythonCodeGenerator::generateWhile(const WhileNode& whileNode) {
    std::ostringstream oss;
    oss << \"while \";
    
    if (whileNode.getCondition()) {
        oss << generate(*whileNode.getCondition());
    }
    oss << \":\";
    
    if (whileNode.getBody()) {
        // Process the body, adding proper indentation
        std::string body = generate(*whileNode.getBody());
        size_t pos = 0;
        while ((pos = body.find(\"\\n\", pos)) != std::string::npos) {
            if (pos + 1 < body.length()) { // If there's content after the newline
                body.insert(pos + 1, \"    \"); // Add 4 spaces for Python indentation
                pos += 5; // Move past the inserted spaces
            } else {
                break;
            }
        }
        oss << body;
    }
    
    return oss.str();
}

std::string PythonCodeGenerator::generateFor(const ForNode& forNode) {
    // For simplicity, we'll use a Python for-in loop pattern
    // This assumes we're dealing with basic loops like for(i=0; i<n; i++)
    std::ostringstream oss;
    
    // If we have all init, condition, and increment, try to convert to Python range
    if (forNode.getInit() && forNode.getCondition() && forNode.getIncrement()) {
        // This is a basic implementation - in a full system we'd need more complex parsing
        oss << \"# IPL for-loop converted to Python\\n\";
        oss << \"for i in range(0, 10):  # Placeholder - actual range needs to be determined from condition\";
        
        if (forNode.getBody()) {
            // Process the body, adding proper indentation
            std::string body = generate(*forNode.getBody());
            size_t pos = 0;
            while ((pos = body.find(\"\\n\", pos)) != std::string::npos) {
                if (pos + 1 < body.length()) { // If there's content after the newline
                    body.insert(pos + 1, \"    \"); // Add 4 spaces for Python indentation
                    pos += 5; // Move past the inserted spaces
                } else {
                    break;
                }
            }
            oss << body;
        }
    } else {
        // Fallback: convert to while loop
        oss << \"\\n# Converted IPL for-loop to Python while loop\\n\";
        if (forNode.getInit()) {
            oss << generate(*forNode.getInit()) << \"\\n\";
        }
        
        oss << \"while \";
        if (forNode.getCondition()) {
            oss << generate(*forNode.getCondition());
        }
        oss << \":\";
        
        if (forNode.getBody()) {
            std::string body = generate(*forNode.getBody());
            size_t pos = 0;
            while ((pos = body.find(\"\\n\", pos)) != std::string::npos) {
                if (pos + 1 < body.length()) { // If there's content after the newline
                    body.insert(pos + 1, \"    \"); // Add 4 spaces for Python indentation
                    pos += 5; // Move past the inserted spaces
                } else {
                    break;
                }
            }
            oss << body;
        }
        
        if (forNode.getIncrement()) {
            oss << \"\\n    \" << generate(*forNode.getIncrement());
        }
    }
    
    return oss.str();
}

} // namespace codegen
} // namespace istudio
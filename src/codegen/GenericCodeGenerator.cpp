#include "GenericCodeGenerator.h"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace istudio {
namespace codegen {

GenericCodeGenerator::GenericCodeGenerator(const std::string& targetLanguage)
    : CodeGenerator(TargetLanguage::C), targetLanguageName_(targetLanguage) {}

void GenericCodeGenerator::loadRules(const std::vector<CodeGenerationRule>& rules) {
    for (const auto& rule : rules) {
        rules_[rule.nodeType] = rule;
    }
}

std::string GenericCodeGenerator::generate(const ASTNode& node) {
    std::ostringstream oss;
    output_.str(""); // Clear the stream
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
            return "// Unsupported node type in generic code generation\n";
    }
}

std::string GenericCodeGenerator::generateProgram(const ProgramNode& program) {
    std::ostringstream oss;
    
    // Check if there's a specific rule for Program
    auto it = rules_.find("Program");
    if (it != rules_.end()) {
        // Use template if available
        std::unordered_map<std::string, std::string> replacements = {
            {"{{PROLOGUE}}", "// Program in " + targetLanguageName_ + "\n"},
            {"{{BODY}}", ""},
            {"{{EPILOGUE}}", ""}
        };
        
        // Generate the body of the program
        std::ostringstream bodyStream;
        for (const auto& func : program.getFunctions()) {
            if (func) {
                bodyStream << generate(*func) << "\n";
            }
        }
        replacements["{{BODY}}"] = bodyStream.str();
        
        return applyTemplate(it->second.templateString, replacements);
    } else {
        // Default behavior if no rule exists
        oss << "// Program in " << targetLanguageName_ << "\n";
        for (const auto& func : program.getFunctions()) {
            if (func) {
                oss << generate(*func) << "\n";
            }
        }
        return oss.str();
    }
}

std::string GenericCodeGenerator::generateFunction(const FunctionNode& function) {
    auto it = rules_.find("Function");
    if (it != rules_.end()) {
        // Map the return type according to the rules
        std::string mappedReturnType = mapType(function.getReturnType());
        
        // Process parameters
        std::ostringstream paramsStream;
        const auto& params = function.getParameters();
        for (size_t i = 0; i < params.size(); ++i) {
            if (i > 0) paramsStream << ", ";
            
            std::string mappedParamType = mapType(params[i].type);
            paramsStream << mappedParamType << " " << params[i].name;
        }
        
        std::ostringstream bodyStream;
        if (function.getBody()) {
            bodyStream << generate(*function.getBody());
        } else {
            bodyStream << " // Function declaration";
        }
        
        std::unordered_map<std::string, std::string> replacements = {
            {"{{NAME}}", function.getName()},
            {"{{RETURN_TYPE}}", mappedReturnType},
            {"{{PARAMS}}", paramsStream.str()},
            {"{{BODY}}", bodyStream.str()}
        };
        
        return applyTemplate(it->second.templateString, replacements);
    } else {
        // Default behavior if no rule exists
        std::ostringstream oss;
        std::string mappedReturnType = mapType(function.getReturnType());
        
        oss << mappedReturnType << " " << function.getName() << "(";
        
        const auto& params = function.getParameters();
        for (size_t i = 0; i < params.size(); ++i) {
            if (i > 0) oss << ", ";
            
            std::string mappedParamType = mapType(params[i].type);
            oss << mappedParamType << " " << params[i].name;
        }
        oss << ")";
        
        if (function.getBody()) {
            oss << "\n" << generate(*function.getBody());
        } else {
            oss << "; // Function declaration\n";
        }
        
        return oss.str();
    }
}

std::string GenericCodeGenerator::generateVariableDeclaration(const VariableDeclarationNode& varDecl) {
    auto it = rules_.find("VariableDeclaration");
    if (it != rules_.end()) {
        std::string mappedType = mapType(varDecl.getTypeName());
        
        std::string initValue = "";
        if (varDecl.getInitializer()) {
            initValue = generate(*varDecl.getInitializer());
        }
        
        std::unordered_map<std::string, std::string> replacements = {
            {"{{TYPE}}", mappedType},
            {"{{NAME}}", varDecl.getName()},
            {"{{INIT_VALUE}}", initValue}
        };
        
        return applyTemplate(it->second.templateString, replacements);
    } else {
        // Default behavior if no rule exists
        std::ostringstream oss;
        std::string mappedType = mapType(varDecl.getTypeName());
        
        oss << mappedType << " " << varDecl.getName();
        
        if (varDecl.getInitializer()) {
            oss << " = " << generate(*varDecl.getInitializer());
        }
        
        oss << ";";
        return oss.str();
    }
}

std::string GenericCodeGenerator::generateAssignment(const AssignmentNode& assignment) {
    auto it = rules_.find("Assignment");
    if (it != rules_.end()) {
        std::string valueStr = generate(*assignment.getValue());
        
        std::unordered_map<std::string, std::string> replacements = {
            {"{{VARIABLE}}", assignment.getVariable()},
            {"{{VALUE}}", valueStr}
        };
        
        return applyTemplate(it->second.templateString, replacements);
    } else {
        // Default behavior if no rule exists
        std::ostringstream oss;
        oss << assignment.getVariable() << " = " << generate(*assignment.getValue()) << ";";
        return oss.str();
    }
}

std::string GenericCodeGenerator::generateBinaryOperation(const BinaryOperationNode& binOp) {
    auto it = rules_.find("BinaryOperation");
    if (it != rules_.end()) {
        std::string leftStr = generate(*binOp.getLeft());
        std::string rightStr = generate(*binOp.getRight());
        
        std::unordered_map<std::string, std::string> replacements = {
            {"{{LEFT}}", leftStr},
            {"{{OPERATOR}}", binOp.getOperator()},
            {"{{RIGHT}}", rightStr}
        };
        
        return applyTemplate(it->second.templateString, replacements);
    } else {
        // Default behavior if no rule exists
        std::ostringstream oss;
        oss << "(" << generate(*binOp.getLeft()) << " " << binOp.getOperator() 
            << " " << generate(*binOp.getRight()) << ")";
        return oss.str();
    }
}

std::string GenericCodeGenerator::generateUnaryOperation(const UnaryOperationNode& unaryOp) {
    auto it = rules_.find("UnaryOperation");
    if (it != rules_.end()) {
        std::string operandStr = generate(*unaryOp.getOperand());
        
        std::unordered_map<std::string, std::string> replacements = {
            {"{{OPERATOR}}", unaryOp.getOperator()},
            {"{{OPERAND}}", operandStr}
        };
        
        return applyTemplate(it->second.templateString, replacements);
    } else {
        // Default behavior if no rule exists
        std::ostringstream oss;
        oss << unaryOp.getOperator() << "(" << generate(*unaryOp.getOperand()) << ")";
        return oss.str();
    }
}

std::string GenericCodeGenerator::generateCallExpression(const CallExpressionNode& call) {
    auto it = rules_.find("CallExpression");
    if (it != rules_.end()) {
        std::ostringstream argsStream;
        if (call.getCallee()) {
            const auto& args = call.getArguments();
            for (size_t i = 0; i < args.size(); ++i) {
                if (i > 0) argsStream << ", ";
                if (args[i]) {
                    argsStream << generate(*args[i]);
                }
            }
            
            std::string calleeStr = generate(*call.getCallee());
            
            std::unordered_map<std::string, std::string> replacements = {
                {"{{CALLEE}}", calleeStr},
                {"{{ARGS}}", argsStream.str()}
            };
            
            return applyTemplate(it->second.templateString, replacements);
        }
        return "";
    } else {
        // Default behavior if no rule exists
        std::ostringstream oss;
        if (call.getCallee()) {
            oss << generate(*call.getCallee()) << "(";
            
            const auto& args = call.getArguments();
            for (size_t i = 0; i < args.size(); ++i) {
                if (i > 0) oss << ", ";
                if (args[i]) {
                    oss << generate(*args[i]);
                }
            }
            oss << ")";
        }
        
        return oss.str();
    }
}

std::string GenericCodeGenerator::generateLiteral(const LiteralNode& literal) {
    auto it = rules_.find("Literal");
    if (it != rules_.end()) {
        std::unordered_map<std::string, std::string> replacements = {
            {"{{VALUE}}", literal.getValue()}
        };
        
        return applyTemplate(it->second.templateString, replacements);
    } else {
        return literal.getValue();
    }
}

std::string GenericCodeGenerator::generateIdentifier(const IdentifierNode& identifier) {
    return identifier.getName();
}

std::string GenericCodeGenerator::generateBlock(const BlockNode& block) {
    auto it = rules_.find("Block");
    if (it != rules_.end()) {
        std::ostringstream bodyStream;
        for (const auto& stmt : block.getStatements()) {
            if (stmt) {
                bodyStream << generate(*stmt) << "\n";
            }
        }
        
        std::unordered_map<std::string, std::string> replacements = {
            {"{{BODY}}", bodyStream.str()}
        };
        
        return applyTemplate(it->second.templateString, replacements);
    } else {
        // Default behavior if no rule exists
        std::ostringstream oss;
        oss << " {\n";
        
        for (const auto& stmt : block.getStatements()) {
            if (stmt) {
                oss << "    " << generate(*stmt) << "\n";
            }
        }
        
        oss << "}";
        return oss.str();
    }
}

std::string GenericCodeGenerator::generateReturn(const ReturnNode& ret) {
    auto it = rules_.find("Return");
    if (it != rules_.end()) {
        std::string valueStr = "";
        if (ret.getValue()) {
            valueStr = generate(*ret.getValue());
        }
        
        std::unordered_map<std::string, std::string> replacements = {
            {"{{VALUE}}", valueStr}
        };
        
        return applyTemplate(it->second.templateString, replacements);
    } else {
        // Default behavior if no rule exists
        std::ostringstream oss;
        oss << "return";
        
        if (ret.getValue()) {
            oss << " " << generate(*ret.getValue());
        }
        oss << ";";
        
        return oss.str();
    }
}

std::string GenericCodeGenerator::generateExpressionStatement(const ExpressionStatementNode& exprStmt) {
    if (exprStmt.getExpression()) {
        return generate(*exprStmt.getExpression()) + ";";
    }
    return "";
}

std::string GenericCodeGenerator::generateIf(const IfNode& ifNode) {
    auto it = rules_.find("If");
    if (it != rules_.end()) {
        std::string conditionStr = "";
        if (ifNode.getCondition()) {
            conditionStr = generate(*ifNode.getCondition());
        }
        
        std::string thenStr = "";
        if (ifNode.getThenBranch()) {
            thenStr = generate(*ifNode.getThenBranch());
        }
        
        std::string elseStr = "";
        if (ifNode.getElseBranch()) {
            elseStr = generate(*ifNode.getElseBranch());
        }
        
        std::unordered_map<std::string, std::string> replacements = {
            {"{{CONDITION}}", conditionStr},
            {"{{THEN_BRANCH}}", thenStr},
            {"{{ELSE_BRANCH}}", elseStr}
        };
        
        return applyTemplate(it->second.templateString, replacements);
    } else {
        // Default behavior if no rule exists
        std::ostringstream oss;
        oss << "if (";
        
        if (ifNode.getCondition()) {
            oss << generate(*ifNode.getCondition());
        }
        oss << ")";
        
        if (ifNode.getThenBranch()) {
            oss << " " << generate(*ifNode.getThenBranch());
        }
        
        if (ifNode.getElseBranch()) {
            oss << " else " << generate(*ifNode.getElseBranch());
        }
        
        return oss.str();
    }
}

std::string GenericCodeGenerator::generateWhile(const WhileNode& whileNode) {
    auto it = rules_.find("While");
    if (it != rules_.end()) {
        std::string conditionStr = "";
        if (whileNode.getCondition()) {
            conditionStr = generate(*whileNode.getCondition());
        }
        
        std::string bodyStr = "";
        if (whileNode.getBody()) {
            bodyStr = generate(*whileNode.getBody());
        }
        
        std::unordered_map<std::string, std::string> replacements = {
            {"{{CONDITION}}", conditionStr},
            {"{{BODY}}", bodyStr}
        };
        
        return applyTemplate(it->second.templateString, replacements);
    } else {
        // Default behavior if no rule exists
        std::ostringstream oss;
        oss << "while (";
        
        if (whileNode.getCondition()) {
            oss << generate(*whileNode.getCondition());
        }
        oss << ")";
        
        if (whileNode.getBody()) {
            oss << " " << generate(*whileNode.getBody());
        }
        
        return oss.str();
    }
}

std::string GenericCodeGenerator::generateFor(const ForNode& forNode) {
    auto it = rules_.find("For");
    if (it != rules_.end()) {
        std::string initStr = "";
        if (forNode.getInit()) {
            initStr = generate(*forNode.getInit());
        }
        
        std::string conditionStr = "";
        if (forNode.getCondition()) {
            conditionStr = generate(*forNode.getCondition());
        }
        
        std::string incrementStr = "";
        if (forNode.getIncrement()) {
            incrementStr = generate(*forNode.getIncrement());
        }
        
        std::string bodyStr = "";
        if (forNode.getBody()) {
            bodyStr = generate(*forNode.getBody());
        }
        
        std::unordered_map<std::string, std::string> replacements = {
            {"{{INIT}}", initStr},
            {"{{CONDITION}}", conditionStr},
            {"{{INCREMENT}}", incrementStr},
            {"{{BODY}}", bodyStr}
        };
        
        return applyTemplate(it->second.templateString, replacements);
    } else {
        // Default behavior if no rule exists
        std::ostringstream oss;
        oss << "for (";
        
        if (forNode.getInit()) {
            oss << generate(*forNode.getInit()) << " ";
        } else {
            oss << "; ";
        }
        
        if (forNode.getCondition()) {
            oss << generate(*forNode.getCondition()) << " ";
        } else {
            oss << "; ";
        }
        
        if (forNode.getIncrement()) {
            oss << generate(*forNode.getIncrement());
        }
        oss << ")";
        
        if (forNode.getBody()) {
            oss << " " << generate(*forNode.getBody());
        }
        
        return oss.str();
    }
}

std::string GenericCodeGenerator::applyTemplate(const std::string& tmpl, 
                                               const std::unordered_map<std::string, std::string>& replacements) const {
    std::string result = tmpl;
    
    for (const auto& [placeholder, value] : replacements) {
        size_t pos = 0;
        while ((pos = result.find(placeholder, pos)) != std::string::npos) {
            result.replace(pos, placeholder.length(), value);
            pos += value.length();
        }
    }
    
    return result;
}

std::string GenericCodeGenerator::mapType(const std::string& iplType) const {
    auto it = rules_.find("TypeMapping");
    if (it != rules_.end()) {
        auto mappingIt = it->second.mappings.find(iplType);
        if (mappingIt != it->second.mappings.end()) {
            return mappingIt->second;
        }
    }
    // If no mapping is found, return the original type
    return iplType;
}

std::string GenericCodeGenerator::mapKeyword(const std::string& iplKeyword) const {
    auto it = rules_.find("KeywordMapping");
    if (it != rules_.end()) {
        auto mappingIt = it->second.mappings.find(iplKeyword);
        if (mappingIt != it->second.mappings.end()) {
            return mappingIt->second;
        }
    }
    // If no mapping is found, return the original keyword
    return iplKeyword;
}

} // namespace codegen
} // namespace istudio
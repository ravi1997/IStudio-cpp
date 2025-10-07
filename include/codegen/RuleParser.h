#pragma once

#include "GenericCodeGenerator.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

namespace istudio {
namespace codegen {

class RuleParser {
public:
    static std::vector<CodeGenerationRule> parseFromFile(const std::string& filePath);
    
private:
    static void parseRuleSection(std::ifstream& file, std::vector<CodeGenerationRule>& rules);
    static std::unordered_map<std::string, std::string> parseMappings(const std::string& mappingsStr);
};

} // namespace codegen
} // namespace istudio
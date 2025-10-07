#include "RuleParser.h"
#include <iostream>
#include <algorithm>

namespace istudio {
namespace codegen {

std::vector<CodeGenerationRule> RuleParser::parseFromFile(const std::string& filePath) {
    std::vector<CodeGenerationRule> rules;
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open rule file: " << filePath << std::endl;
        return rules;
    }
    
    std::string line;
    std::string currentType;
    std::string currentTemplate;
    std::string currentMappings;
    
    while (std::getline(file, line)) {
        // Remove leading/trailing whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Check for section header [NodeType]
        if (line.length() > 2 && line[0] == '[' && line[line.length() - 1] == ']') {
            // If we were processing a previous section, save it
            if (!currentType.empty()) {
                CodeGenerationRule rule;
                rule.nodeType = currentType;
                rule.templateString = currentTemplate;
                rule.mappings = parseMappings(currentMappings);
                rules.push_back(rule);
            }
            
            // Start new section
            currentType = line.substr(1, line.length() - 2); // Remove [ and ]
            currentTemplate = "";
            currentMappings = "";
        }
        else if (line.substr(0, 8) == "template") {
            // Extract template content after "template="
            size_t eqPos = line.find('=');
            if (eqPos != std::string::npos) {
                currentTemplate = line.substr(eqPos + 1);
            }
        }
        else if (line.substr(0, 7) == "mappings") {
            // Extract mappings content after "mappings="
            size_t eqPos = line.find('=');
            if (eqPos != std::string::npos) {
                currentMappings = line.substr(eqPos + 1);
            }
        }
        else {
            // This might be a continuation of template or mappings
            if (!currentType.empty()) {
                // For now, if it's not clearly template or mappings, ignore it
                // In a more robust implementation, we'd have better handling
            }
        }
    }
    
    // Add the last rule
    if (!currentType.empty()) {
        CodeGenerationRule rule;
        rule.nodeType = currentType;
        rule.templateString = currentTemplate;
        rule.mappings = parseMappings(currentMappings);
        rules.push_back(rule);
    }
    
    return rules;
}

std::unordered_map<std::string, std::string> RuleParser::parseMappings(const std::string& mappingsStr) {
    std::unordered_map<std::string, std::string> mappings;
    
    // Parse mappings in the format: "key1=value1,key2=value2"
    std::istringstream iss(mappingsStr);
    std::string pair;
    
    while (std::getline(iss, pair, ',')) {
        size_t eqPos = pair.find('=');
        if (eqPos != std::string::npos) {
            std::string key = pair.substr(0, eqPos);
            std::string value = pair.substr(eqPos + 1);
            
            // Remove leading/trailing whitespace
            key.erase(0, key.find_first_not_of(" \t\r\n"));
            key.erase(key.find_last_not_of(" \t\r\n") + 1);
            value.erase(0, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);
            
            mappings[key] = value;
        }
    }
    
    return mappings;
}

} // namespace codegen
} // namespace istudio
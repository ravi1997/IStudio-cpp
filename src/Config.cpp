#include "../include/Config.h"
#include <fstream>
#include <sstream>

Config::Config() = default;

bool Config::loadGrammarFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Simple parsing: assume format "pattern -> action"
        size_t pos = line.find("->");
        if (pos != std::string::npos) {
            GrammarRule rule;
            rule.pattern = line.substr(0, pos);
            rule.action = line.substr(pos + 2);
            
            // Trim whitespace
            rule.pattern.erase(0, rule.pattern.find_first_not_of(" 	"));
            rule.pattern.erase(rule.pattern.find_last_not_of(" 	") + 1);
            rule.action.erase(0, rule.action.find_first_not_of(" 	"));
            rule.action.erase(rule.action.find_last_not_of(" 	") + 1);
            
            grammar_rules_.push_back(rule);
        }
    }
    
    return true;
}

bool Config::loadTranslationRules(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Simple parsing: assume format "from -> to: rule"
        size_t arrow_pos = line.find("->");
        size_t colon_pos = line.find(":");
        
        if (arrow_pos != std::string::npos && colon_pos != std::string::npos && colon_pos > arrow_pos) {
            TranslationRule rule;
            rule.from_language = line.substr(0, arrow_pos);
            rule.to_language = line.substr(arrow_pos + 2, colon_pos - arrow_pos - 2);
            rule.rule = line.substr(colon_pos + 1);
            
            // Trim whitespace
            rule.from_language.erase(0, rule.from_language.find_first_not_of(" 	"));
            rule.from_language.erase(rule.from_language.find_last_not_of(" 	") + 1);
            rule.to_language.erase(0, rule.to_language.find_first_not_of(" 	"));
            rule.to_language.erase(rule.to_language.find_last_not_of(" 	") + 1);
            rule.rule.erase(0, rule.rule.find_first_not_of(" 	"));
            rule.rule.erase(rule.rule.find_last_not_of(" 	") + 1);
            
            translation_rules_.push_back(rule);
        }
    }
    
    return true;
}

bool Config::loadSourceCode(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    source_code_ = buffer.str();
    
    return true;
}
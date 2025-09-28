#pragma once
#include <string>
#include <unordered_map>
#include <vector>

struct GrammarRule {
    std::string pattern;
    std::string action;
};

struct TranslationRule {
    std::string from_language;
    std::string to_language;
    std::string rule;
};

class Config {
public:
    Config();
    ~Config() = default;
    
    bool loadGrammarFile(const std::string& filepath);
    bool loadTranslationRules(const std::string& filepath);
    bool loadSourceCode(const std::string& filepath);
    
    const std::vector<GrammarRule>& getGrammarRules() const { return grammar_rules_; }
    const std::vector<TranslationRule>& getTranslationRules() const { return translation_rules_; }
    const std::string& getSourceCode() const { return source_code_; }
    
private:
    std::vector<GrammarRule> grammar_rules_;
    std::vector<TranslationRule> translation_rules_;
    std::string source_code_;
};
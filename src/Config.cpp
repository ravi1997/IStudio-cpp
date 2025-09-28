#include "../include/Config.h"
#include <fstream>
#include <sstream>

namespace {

std::string trim(const std::string& value)
{
    const auto first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const auto last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

} // namespace

Config::Config() = default;

bool Config::loadGrammarFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return false;
    }

    grammar_rules_.clear();

    std::string line;
    while (std::getline(file, line)) {
        const auto trimmed = trim(line);
        if (trimmed.empty() || trimmed.front() == '#') {
            continue;
        }

        const size_t pos = trimmed.find("->");
        if (pos != std::string::npos) {
            GrammarRule rule;
            rule.pattern = trim(trimmed.substr(0, pos));
            rule.action = trim(trimmed.substr(pos + 2));
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

    translation_rules_.clear();

    std::string line;
    while (std::getline(file, line)) {
        const auto trimmed = trim(line);
        if (trimmed.empty() || trimmed.front() == '#') {
            continue;
        }

        const size_t arrow_pos = trimmed.find("->");
        const size_t colon_pos = trimmed.find(":");

        if (arrow_pos != std::string::npos && colon_pos != std::string::npos && colon_pos > arrow_pos) {
            TranslationRule rule;
            rule.from_language = trim(trimmed.substr(0, arrow_pos));
            rule.to_language = trim(trimmed.substr(arrow_pos + 2, colon_pos - arrow_pos - 2));
            rule.rule = trim(trimmed.substr(colon_pos + 1));
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
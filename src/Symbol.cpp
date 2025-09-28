#include "../include/Symbol.h"
#include <iostream>
#include <string>

Symbol::Symbol(const std::string& name, SymbolType type) 
    : name_(name), type_(type), definition_("") 
{
}

Symbol::~Symbol() = default;

const std::string& Symbol::getName() const
{
    return name_;
}

SymbolType Symbol::getType() const
{
    return type_;
}

void Symbol::setDefinition(const std::string& definition)
{
    definition_ = definition;
}

const std::string& Symbol::getDefinition() const
{
    return definition_;
}

// SymbolTable implementation
SymbolTable::SymbolTable() = default;

SymbolTable::~SymbolTable() = default;

void SymbolTable::addSymbol(std::shared_ptr<Symbol> symbol)
{
    if (symbol) {
        symbols_[symbol->getName()] = symbol;
    }
}

std::shared_ptr<Symbol> SymbolTable::findSymbol(const std::string& name) const
{
    auto it = symbols_.find(name);
    if (it != symbols_.end()) {
        return it->second;
    }
    return nullptr;
}

void SymbolTable::clear()
{
    symbols_.clear();
}

std::vector<std::shared_ptr<Symbol>> SymbolTable::getAllSymbols() const
{
    std::vector<std::shared_ptr<Symbol>> results;
    results.reserve(symbols_.size());
    for (const auto& entry : symbols_) {
        results.push_back(entry.second);
    }
    return results;
}
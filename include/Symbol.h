#pragma once
#include <string>
#include <memory>

enum class SymbolType {
    Variable,
    Function,
    Class,
    Namespace
};

class Symbol
{
public:
    Symbol(const std::string& name, SymbolType type);
    ~Symbol();

    const std::string& getName() const;
    SymbolType getType() const;
    void setDefinition(const std::string& definition);
    const std::string& getDefinition() const;

private:
    std::string name_;
    SymbolType type_;
    std::string definition_;
};

// Include necessary compiler components
#include <vector>
#include <unordered_map>

class SymbolTable
{
public:
    SymbolTable();
    ~SymbolTable();

    void addSymbol(std::shared_ptr<Symbol> symbol);
    std::shared_ptr<Symbol> findSymbol(const std::string& name) const;
    void clear();
    std::vector<std::shared_ptr<Symbol>> getAllSymbols() const;

private:
    std::unordered_map<std::string, std::shared_ptr<Symbol>> symbols_;
};
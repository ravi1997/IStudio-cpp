#pragma once

#include "semantic/Type.h"
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

namespace istudio::semantic {

enum class SymbolKind {
    Variable,
    Function
};

enum class OwnershipKind {
    Unknown,
    Owned,
    Borrowed,
    Reference
};

struct Symbol {
    std::string name;
    SymbolKind kind{SymbolKind::Variable};
    TypePtr type;
    OwnershipKind ownership{OwnershipKind::Unknown};
    bool isInitialized{false};
    bool hasMoved{false};
};

class SymbolScope : public std::enable_shared_from_this<SymbolScope> {
public:
    using Ptr = std::shared_ptr<SymbolScope>;

    explicit SymbolScope(Ptr parent = {});

    Ptr createChild();

    bool declare(Symbol symbol);
    std::optional<Symbol> lookupLocal(const std::string& name) const;
    std::optional<Symbol> lookup(const std::string& name) const;
    const Ptr& parent() const noexcept { return parent_; }
    const std::unordered_map<std::string, Symbol>& symbols() const noexcept { return symbols_; }
    const std::vector<Ptr>& children() const noexcept { return children_; }

private:
    Ptr parent_;
    std::unordered_map<std::string, Symbol> symbols_;
    std::vector<Ptr> children_;
};

} // namespace istudio::semantic

#include "semantic/SymbolTable.h"

namespace istudio::semantic {

SymbolScope::SymbolScope(Ptr parent)
    : parent_(std::move(parent))
{
}

SymbolScope::Ptr SymbolScope::createChild()
{
    auto child = std::make_shared<SymbolScope>(shared_from_this());
    children_.push_back(child);
    return child;
}

bool SymbolScope::declare(Symbol symbol)
{
    auto [it, inserted] = symbols_.emplace(symbol.name, std::move(symbol));
    return inserted;
}

std::optional<Symbol> SymbolScope::lookupLocal(const std::string& name) const
{
    auto it = symbols_.find(name);
    if (it != symbols_.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::optional<Symbol> SymbolScope::lookup(const std::string& name) const
{
    const SymbolScope* current = this;
    std::shared_ptr<const SymbolScope> holder = shared_from_this();
    while (current) {
        auto it = current->symbols_.find(name);
        if (it != current->symbols_.end()) {
            return it->second;
        }
        holder = current->parent_;
        current = holder.get();
    }
    return std::nullopt;
}

} // namespace istudio::semantic

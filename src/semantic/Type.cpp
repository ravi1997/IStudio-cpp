#include "semantic/Type.h"

namespace istudio::semantic {

FunctionType::FunctionType(TypePtr returnType, std::vector<TypePtr> parameters)
    : Type(TypeKind::Function, "function"), returnType_(std::move(returnType)), parameters_(std::move(parameters))
{
}

TypeContext::TypeContext()
{
    builtins_.emplace("void", std::make_shared<BuiltinType>("void"));
    builtins_.emplace("bool", std::make_shared<BuiltinType>("bool"));
    builtins_.emplace("byte", std::make_shared<BuiltinType>("byte"));
    builtins_.emplace("number", std::make_shared<BuiltinType>("number"));
    builtins_.emplace("int", std::make_shared<BuiltinType>("int"));
    builtins_.emplace("float", std::make_shared<BuiltinType>("float"));
    builtins_.emplace("double", std::make_shared<BuiltinType>("double"));
    builtins_.emplace("char", std::make_shared<BuiltinType>("char"));
    builtins_.emplace("string", std::make_shared<BuiltinType>("string"));
    builtins_.emplace("any", std::make_shared<BuiltinType>("any"));
    builtins_.emplace("bytes", std::make_shared<BuiltinType>("bytes"));
    builtins_.emplace("list", std::make_shared<BuiltinType>("list"));
    builtins_.emplace("dict", std::make_shared<BuiltinType>("dict"));
}

TypePtr TypeContext::getBuiltin(std::string_view name) const
{
    auto it = builtins_.find(std::string{name});
    if (it != builtins_.end()) {
        return it->second;
    }
    return nullptr;
}

TypePtr TypeContext::getOrCreatePointer(TypePtr pointee, std::string decoration)
{
    for (const auto& existing : pointers_) {
        const auto* ptrType = static_cast<const PointerType*>(existing.get());
        if (ptrType->pointee() == pointee && existing->name() == decoration) {
            return existing;
        }
    }

    auto type = std::make_shared<PointerType>(std::move(pointee), std::move(decoration));
    pointers_.push_back(type);
    return type;
}

TypePtr TypeContext::getOrCreateFunction(TypePtr returnType, std::vector<TypePtr> parameters)
{
    for (const auto& existing : functions_) {
        const auto* fnType = static_cast<const FunctionType*>(existing.get());
        if (fnType->returnType() == returnType && fnType->parameters() == parameters) {
            return existing;
        }
    }

    auto type = std::make_shared<FunctionType>(std::move(returnType), std::move(parameters));
    functions_.push_back(type);
    return type;
}

} // namespace istudio::semantic

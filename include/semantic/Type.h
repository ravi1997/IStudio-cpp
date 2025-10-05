#pragma once

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace istudio::semantic {

class Type;
using TypePtr = std::shared_ptr<Type>;

enum class TypeKind {
    Builtin,
    Pointer,
    Reference,
    Optional,
    Function,
    Unknown
};

class Type {
public:
    explicit Type(TypeKind kind, std::string name) : kind_(kind), name_(std::move(name)) {}
    virtual ~Type() = default;

    [[nodiscard]] TypeKind kind() const noexcept { return kind_; }
    [[nodiscard]] const std::string& name() const noexcept { return name_; }

private:
    TypeKind kind_;
    std::string name_;
};

class BuiltinType : public Type {
public:
    explicit BuiltinType(std::string name) : Type(TypeKind::Builtin, std::move(name)) {}
};

class PointerType : public Type {
public:
    PointerType(TypePtr pointee, std::string decoration)
        : Type(TypeKind::Pointer, std::move(decoration)), pointee_(std::move(pointee)) {}

    [[nodiscard]] const TypePtr& pointee() const noexcept { return pointee_; }

private:
    TypePtr pointee_;
};

class FunctionType : public Type {
public:
    FunctionType(TypePtr returnType, std::vector<TypePtr> parameters);

    [[nodiscard]] const TypePtr& returnType() const noexcept { return returnType_; }
    [[nodiscard]] const std::vector<TypePtr>& parameters() const noexcept { return parameters_; }

private:
    TypePtr returnType_;
    std::vector<TypePtr> parameters_;
};

class TypeContext {
public:
    TypeContext();

    TypePtr getBuiltin(std::string_view name) const;
    TypePtr getOrCreatePointer(TypePtr pointee, std::string decoration);
    TypePtr getOrCreateFunction(TypePtr returnType, std::vector<TypePtr> parameters);

private:
    std::unordered_map<std::string, TypePtr> builtins_;
    std::vector<TypePtr> pointers_;
    std::vector<TypePtr> functions_;
};

} // namespace istudio::semantic

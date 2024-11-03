#pragma once

#include <IR/ir.hpp>
#include <Objc/Proxy/type.hpp>
#include <string>
#include <vector>

namespace Objc {
std::string getClassName(std::string const& cppClassFqName,
                         std::string const& moduleName);

std::string getClassName(IR::Struct const& cppClass,
                         std::string const& moduleName);

std::string getFunctionName(IR::Function const& cppFunction,
                            bool isConstructor,
                            bool isOverloaded);

// Joins a set of arguments as a string
std::string getParameterString(std::vector<IR::Argument> const& parameters);

std::string getEnumName(std::string const& qualifiedEnumName,
                        std::string const& moduleName);

std::string getContainerName(IR::Type::Container const& container,
                             std::string const& moduleName);

std::string getContainedTypeName(IR::Type const& type,
                                 std::string const& moduleName);

std::string getBaseName(IR::BaseType type);

std::string getNullableName(Objc::Proxy::Type const& type);
}    // namespace Objc

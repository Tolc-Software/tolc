#pragma once

#include <IR/ir.hpp>

namespace Pybind::Helpers {
/**
* Return UserDefined if type is a Container, otherwise nullptr
*/
IR::Type::UserDefined const* getUserDefined(IR::Type const& type);

/**
* Return Container if type is a Container, otherwise nullptr
*/
IR::Type::Container const* getContainer(IR::Type const& type);

/**
* Return true iff type is of type Container and contains ContainerType container
*/
bool isContainerType(IR::Type const& type, IR::ContainerType container);

/**
* Return true iff type is of type Value and contains BaseType base
*/
bool isBaseType(IR::Type const& type, IR::BaseType base);

/**
* Return true iff type is a function
*/
bool isFunctionType(IR::Type const& type);

/**
* Return substring without the template parameters
* Ex:
*   MyClass<int> -> MyClass
*   myFunction<int> -> myFunction
*/
std::string removeCppTemplate(std::string const& name);

// String version of the container
std::string toString(IR::ContainerType container);
}    // namespace Pybind::Helpers

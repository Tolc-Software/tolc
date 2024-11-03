#pragma once

#include "Pybind/Proxy/function.hpp"
#include <string>
#include <vector>

namespace Pybind::Helpers {
/**
* Creates a string corresponding to a trampoline class in pybind11
* For more info see:
* https://pybind11.readthedocs.io/en/stable/advanced/classes.html#overriding-virtual-functions-in-python
* Returns {trampoline class name, trampoline class definition}
*/
std::pair<std::string, std::string> getTrampolineClass(
    std::string const& className,
    std::string const& fullyQualifiedName,
    std::vector<Pybind::Proxy::Function> const& virtualFunctions,
    std::vector<Pybind::Proxy::Function> const& pureVirtualFunctions);
}

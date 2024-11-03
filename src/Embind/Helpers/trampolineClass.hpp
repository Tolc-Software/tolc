#pragma once

#include "Embind/Proxy/function.hpp"

#include <string>
#include <utility>
#include <vector>

namespace Embind::Helpers {
/**
* Creates a string corresponding to a trampoline class in embind
* For more info see:
* https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#advanced-class-concepts
* Returns {trampoline class name, trampoline class definition}
*/
std::pair<std::string, std::string> getTrampolineClass(
    std::string const& className,
    std::string const& fullyQualifiedName,
    std::vector<Embind::Proxy::Function> const& virtualFunctions);
}    // namespace Embind::Helpers

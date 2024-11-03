#pragma once

#include "Pybind/Proxy/function.hpp"
#include <string>

namespace Pybind {
std::string
returnValuePolicyToString(Pybind::Proxy::Function::return_value_policy p);
}

#include "Pybind/returnValuePolicy.hpp"
#include "Pybind/Proxy/function.hpp"
#include <string>

namespace Pybind {
std::string
returnValuePolicyToString(Pybind::Proxy::Function::return_value_policy p) {
	using rv = Pybind::Proxy::Function::return_value_policy;
	switch (p) {
		case rv::take_ownership: return "return_value_policy::take_ownership";
		case rv::copy: return "return_value_policy::copy";
		case rv::move: return "return_value_policy::move";
		case rv::reference: return "return_value_policy::reference";
		case rv::reference_internal:
			return "return_value_policy::reference_internal";
		case rv::automatic: return "return_value_policy::automatic";
		case rv::automatic_reference:
			return "return_value_policy::automatic_reference";
	}
	return "";
}
}    // namespace Pybind

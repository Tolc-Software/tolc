#pragma once

#include <set>
#include <string>

namespace Pybind::Proxy {

struct TypeInfo {
	// E.g. vector conversion requires inclusion of <pybind11/stl.h>
	// E.g: '<string>' for '#include <string>'
	// NOTE: Use <> or "" explicitly
	std::set<std::string> m_includes;

	// If a class is managed by a sharded_ptr, its fully qualified name will be here
	// A class will be marked shared if it is handled by a sharded_ptr
	std::set<std::string> m_classesMarkedShared;

	std::string m_extraFunctionsNamespace = "Tolc_";
	// If a class has virtual functions they need to have a 'trampoline' class defined.
	// This allows python classes to override the C++ classes.
	// Also allows binding of purely virtual classes
	// This is the full definition of that trampoline class.
	// https://pybind11.readthedocs.io/en/stable/advanced/classes.html#overriding-virtual-functions-in-python
	std::set<std::string> m_trampolineClasses;
};

}    // namespace Pybind::Proxy

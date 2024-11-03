#include "Pybind/Builders/functionBuilder.hpp"
#include "Pybind/Helpers/types.hpp"
#include "Pybind/Proxy/function.hpp"
#include "Pybind/Proxy/typeInfo.hpp"
#include "Pybind/checkType.hpp"
#include <optional>
#include <spdlog/spdlog.h>

namespace Pybind::Builders {

std::optional<Pybind::Proxy::Function>
buildFunction(IR::Function const& cppFunction,
              Pybind::Proxy::TypeInfo& typeInfo) {
	Pybind::Proxy::Function pyFunction(
	    Pybind::Helpers::removeCppTemplate(cppFunction.m_name),
	    cppFunction.m_representation);

	for (auto const& arg : cppFunction.m_arguments) {
		if (!Pybind::Helpers::isContainerType(arg.m_type,
		                                      IR::ContainerType::UniquePtr)) {
			Pybind::checkType(arg.m_type, typeInfo);

			pyFunction.addArgument(arg.m_type.m_representation, arg.m_name);
		} else {
			spdlog::error(
			    R"(The function {} takes a std::unique_ptr as one of its argument. Python cannot give up ownership of an object to a function. See https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html for more info.)",
			    cppFunction.m_representation);
			return std::nullopt;
		}
	}

	Pybind::checkType(cppFunction.m_returnType, typeInfo);
	pyFunction.setReturnType(cppFunction.m_returnType.m_representation);
	pyFunction.setDocumentation(cppFunction.m_documentation);

	if (cppFunction.m_returnType.m_numPointers > 0) {
		pyFunction.setReturnValuePolicy(
		    Pybind::Proxy::Function::return_value_policy::reference);
	}

	return pyFunction;
}
}    // namespace Pybind::Builders

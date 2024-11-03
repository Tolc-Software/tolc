#include "Swift/Builders/functionBuilder.hpp"
#include "ObjcSwift/Helpers/split.hpp"
#include "Swift/Builders/typeBuilder.hpp"
#include "Swift/Proxy/function.hpp"
#include "Swift/getName.hpp"
#include <fmt/format.h>
#include <optional>
#include <string>

namespace Swift::Builders {

Swift::Proxy::Function buildFunction(Objc::Proxy::Function const& objcFunction,
                                     std::string const& libraryName) {
	auto splitted =
	    Swift::splitIntoNames(objcFunction.getCppName(), libraryName);

	Swift::Proxy::Function swiftFunction(
	    splitted.m_name, objcFunction.getName(), splitted.m_swiftPrefix);

	if (objcFunction.isStatic()) {
		swiftFunction.setAsStatic();
	}

	if (objcFunction.isStandalone()) {
		swiftFunction.setAsStandalone();
		swiftFunction.setCallFrom(splitted.m_objcPrefix);
	} else {
		if (objcFunction.isConstructor()) {
			swiftFunction.setCallFrom(splitted.m_objcPrefix);
		} else {
			swiftFunction.setCallFrom(
			    objcFunction.isStatic() ? splitted.m_objcPrefix : "m_object");
		}
	}

	if (objcFunction.isConstructor()) {
		if (!objcFunction.getArguments().empty()) {
			// The first argument takes the name of
			// whatever comes after initWith lowercased
			// initWithPairStringInt -> pairStringInt
			constexpr std::string_view init = "initWith";
			auto name = objcFunction.getName();
			name.erase(0, init.size());
			if (!name.empty()) {
				name[0] = static_cast<char>(tolower(name[0]));
			}
			swiftFunction.addConstructorArgName(name);
		}
		swiftFunction.setAsConstructor();
	} else {
		swiftFunction.setReturnType(buildType(objcFunction.getReturnType()));
	}

	for (auto const& argument : objcFunction.getArguments()) {
		swiftFunction.addArgument(
		    {argument.m_name, buildType(argument.m_type)});
	}

	return swiftFunction;
}
}    // namespace Swift::Builders

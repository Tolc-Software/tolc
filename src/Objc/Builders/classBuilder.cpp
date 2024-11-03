#include "Objc/Builders/classBuilder.hpp"
#include "Objc/Builders/attributeBuilder.hpp"
#include "Objc/Builders/enumBuilder.hpp"
#include "Objc/Builders/functionBuilder.hpp"
#include "Objc/Builders/typeBuilder.hpp"
#include "Objc/Conversions/userDefined.hpp"
#include "Objc/Proxy/attribute.hpp"
#include "Objc/Proxy/function.hpp"
#include "Objc/cache.hpp"
#include "Objc/getName.hpp"
#include "ObjcSwift/Helpers/combine.hpp"
#include "ObjcSwift/Helpers/operatorNames.hpp"
#include "ObjcSwift/Helpers/typeToStringBuilder.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include "ObjcSwift/getOverloadedFunctions.hpp"
#include <IR/ir.hpp>
#include <fmt/format.h>
#include <numeric>
#include <optional>
#include <set>
#include <spdlog/spdlog.h>
#include <string>
#include <string_view>

namespace Objc::Builders {

namespace {

std::string getConversions(std::string_view objcName,
                           std::string_view cppName) {
	// No need for conversions for an object that cannot be instantiated
	auto conversions =
	    Objc::Conversions::getUserDefinedConversionNames(cppName, "");
	return fmt::format(R"(
{objcName}* {toObjcName}({cppName} const& cppClass) {{
  return [[{objcName} alloc] Tolc_initWithCppObject:cppClass];
}}

{cppName}& {toCppName}({objcName}* objcClass) {{
  return [objcClass Tolc_getCppObject];
}}
)",
	                   fmt::arg("objcName", objcName),
	                   fmt::arg("toObjcName", conversions.m_toObjc),
	                   fmt::arg("toCppName", conversions.m_toCpp),
	                   fmt::arg("cppName", cppName));
}

}    // namespace

Objc::Proxy::Class buildClass(IR::Struct const& cppClass, Objc::Cache& cache) {
	Objc::Proxy::Class objcClass(
	    Objc::getClassName(cppClass, cache.m_moduleName),
	    cppClass.m_representation);

	cache.m_cppToObjcClassNames[cppClass.m_representation] =
	    objcClass.getName();

	for (auto const& e : cppClass.m_public.m_enums) {
		objcClass.addEnum(buildEnum(e, cache));
	}

	objcClass.m_id = cppClass.m_id;

	objcClass.setDocumentation(cppClass.m_documentation);

	objcClass.setInherited(cppClass.m_public.m_inherited);

	cache.m_extraClassConversions.push_back(
	    getConversions(objcClass.getName(), cppClass.m_representation));

	// Ignore private functions
	size_t overloadIndex = 0;
	auto overloadedFunctions =
	    ObjcSwift::getOverloadedFunctions(cppClass.m_public.m_functions);
	for (auto const& function : cppClass.m_public.m_functions) {
		bool isOverloaded =
		    overloadedFunctions.find(function.m_representation) !=
		    overloadedFunctions.end();
		bool isConstructor = false;
		auto objcFunction = buildFunction(objcClass.getName(),
		                                  cppClass.m_representation,
		                                  function,
		                                  cache,
		                                  isConstructor,
		                                  isOverloaded);

		if (function.m_isStatic) {
			objcFunction.setAsStatic();
		}

		objcClass.addFunction(objcFunction);
	}

	auto overloadedOperators =
	    ObjcSwift::getOverloadedFunctions(cppClass.m_public.m_operators);
	for (auto const& [op, function] : cppClass.m_public.m_operators) {
		bool isOverloaded =
		    overloadedFunctions.find(function.m_representation) !=
		    overloadedFunctions.end();
		bool isConstructor = false;
		auto objcFunction = buildFunction(objcClass.getName(),
		                                  cppClass.m_representation,
		                                  function,
		                                  cache,
		                                  isConstructor,
		                                  isOverloaded);
		if (auto maybeName = ObjcSwift::Helpers::getOperatorName(op)) {
			if (function.m_isStatic) {
				objcFunction.setAsStatic();
			}

			objcClass.addFunction(objcFunction);
		} else {
			spdlog::warn(
			    "Unknown operator {} found while creating an Objective-C version of class {}. Since it currently does not have a translation, it's being ignored.",
			    function.m_representation,
			    objcClass.getCppClassName());
		}
	}

	for (auto const& constructor : cppClass.m_public.m_constructors) {
		auto objcFunction = buildFunction(objcClass.getName(),
		                                  cppClass.m_representation,
		                                  constructor,
		                                  cache,
		                                  true);

		if (constructor.m_isStatic) {
			objcFunction.setAsStatic();
		}

		if (cppClass.m_public.m_constructors.size() > 1) {
			objcFunction.setAsOverloaded(overloadIndex++);
		}

		objcFunction.setAsConstructor();

		objcClass.addConstructor(objcFunction);
	}

	for (auto const& variable : cppClass.m_public.m_memberVariables) {
		objcClass.addMemberVariable(Objc::Builders::buildAttribute(
		    objcClass.getName(), cppClass.m_representation, variable, cache));
	}

	// Add default constructor
	if (cppClass.m_hasImplicitDefaultConstructor) {
		auto constructor = Objc::Proxy::Function("init",
		                                         cppClass.m_name,
		                                         cppClass.m_representation +
		                                             "::" + cppClass.m_name,
		                                         objcClass.getName(),
		                                         cppClass.m_representation);
		Objc::Proxy::Type returnType;
		returnType.m_cppType = nullptr;
		returnType.m_name = []() {
			return "instancetype";
		};
		returnType.m_conversions.m_toObjc = "";
		returnType.m_conversions.m_toCpp = "";
		constructor.setReturnType(returnType);
		constructor.setAsConstructor();
		objcClass.addConstructor(constructor);
	}

	return objcClass;
}
}    // namespace Objc::Builders

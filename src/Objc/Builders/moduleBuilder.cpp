#include "Objc/Builders/moduleBuilder.hpp"
#include "Objc/Builders/attributeBuilder.hpp"
#include "Objc/Builders/classBuilder.hpp"
#include "Objc/Builders/enumBuilder.hpp"
#include "Objc/Builders/functionBuilder.hpp"
#include "Objc/Proxy/class.hpp"
#include "Objc/cache.hpp"
#include "ObjcSwift/Helpers/combine.hpp"
#include "ObjcSwift/Helpers/split.hpp"
#include "ObjcSwift/getOverloadedFunctions.hpp"
#include <IR/ir.hpp>
#include <algorithm>
#include <fmt/format.h>
#include <set>
#include <string>

namespace Objc::Builders {

namespace {
std::string getModuleName(std::string const& moduleName,
                          std::string const& fullyQualifiedName) {
	return fmt::format("{}",
	                   fmt::join(ObjcSwift::Helpers::split(
	                                 moduleName + fullyQualifiedName, "::"),
	                             ""));
}
}    // namespace

std::optional<Objc::Proxy::Class> buildModule(IR::Namespace const& ns,
                                              Objc::Cache& cache) {
	Objc::Proxy::Class objcModule(
	    getModuleName(cache.m_moduleName, ns.m_representation),
	    ns.m_representation);
	objcModule.setAsPurelyStatic();

	return objcModule;
}
}    // namespace Objc::Builders

#include "Objc/Builders/moduleFileBuilder.hpp"
#include "Objc/Builders/attributeBuilder.hpp"
#include "Objc/Builders/classBuilder.hpp"
#include "Objc/Builders/enumBuilder.hpp"
#include "Objc/Builders/functionBuilder.hpp"
#include "Objc/Builders/moduleBuilder.hpp"
#include "Objc/Proxy/moduleFile.hpp"
#include "Objc/cache.hpp"
#include "ObjcSwift/Helpers/combine.hpp"
#include "ObjcSwift/getOverloadedFunctions.hpp"
#include <IR/ir.hpp>
#include <optional>
#include <queue>
#include <spdlog/spdlog.h>
#include <string>

namespace {

bool addNamespaceObjects(Objc::Proxy::ModuleFile& moduleFile,
                         IR::Namespace const& ns,
                         std::string const& objcName,
                         Objc::Cache& cache) {
	bool hasAddedAnything = false;
	for (auto const& e : ns.m_enums) {
		moduleFile.addEnum(Objc::Builders::buildEnum(e, cache));
		hasAddedAnything = true;
	}

	for (auto const& cls : ns.m_structs) {
		moduleFile.addClass(Objc::Builders::buildClass(cls, cache));
		hasAddedAnything = true;
	}

	auto overloadedFunctions =
	    ObjcSwift::getOverloadedFunctions(ns.m_functions);
	for (auto const& function : ns.m_functions) {
		bool isConstructor = false;
		bool isOverloaded =
		    overloadedFunctions.contains(function.m_representation);
		auto f = Objc::Builders::buildFunction(objcName,
		                                       ns.m_representation,
		                                       function,
		                                       cache,
		                                       isConstructor,
		                                       isOverloaded);
		// Global functions act as static functions
		f.setAsStatic();
		f.setAsStandalone();
		moduleFile.addFunction(f);
		hasAddedAnything = true;
	}

	for (auto const& variable : ns.m_variables) {
		auto attr = Objc::Builders::buildAttribute(
		    objcName, ns.m_representation, variable, cache);
		// All global variables act as static with this interface
		// (through a class)
		attr.setAsStatic();
		attr.setAsStandalone();
		moduleFile.addAttribute(attr);
		hasAddedAnything = true;
	}

	return hasAddedAnything;
}

// This namespace will not add any symbols
bool isEmpty(IR::Namespace const& ns) {
	return ns.m_functions.empty() && ns.m_enums.empty() &&
	       ns.m_structs.empty() && ns.m_variables.empty();
}

/**
* A namespace will correspond to a Class in Objc.
* Every symbol in Objc will be prefixed by the library name.
* Ex: C++ - library = Store
*   Cart getCart();
*
* Will result in the function with the call
* Objc
*   [Store getCart];
* where Store is introduced as a class
*
* So if we add a non-empty namespace called "Store" together with "getCart"
* we will have a name clash. Since empty namespaces are not translated
* this checks either root is empty, or module exists and is non-empty
*/
bool hasNonEmptyGlobalNSAndModuleNS(IR::Namespace const& rootNamespace,
                                    std::string_view libraryName) {
	// Not creating a ns == libraryName
	// No conflicts
	if (isEmpty(rootNamespace)) {
		return false;
	}

	// Is there a ns == libraryName?
	if (auto moduleNs = std::find_if(rootNamespace.m_namespaces.begin(),
	                                 rootNamespace.m_namespaces.end(),
	                                 [libraryName](IR::Namespace const& ns) {
		                                 return ns.m_name == libraryName;
	                                 });
	    moduleNs != rootNamespace.m_namespaces.end()) {
		// There is, is it empty?
		return !isEmpty(*moduleNs);
	}
	return false;
}
}    // namespace

namespace Objc::Builders {

std::optional<Objc::Proxy::ModuleFile>
buildModuleFile(IR::Namespace const& rootNamespace,
                std::string const& libraryName) {
	if (hasNonEmptyGlobalNSAndModuleNS(rootNamespace, libraryName)) {
		spdlog::error(
		    R"(Non-empty global namespace and non-empty namespace with the same name as the library: {0}
Due to how Tolc introduces symbols when creating Objective-C bindings, it is not possible to have a non-empty global namespace while also having a non-empty namespace with the same name as the library.
Merge the global public functions/classes/enums with the functions/classes/enums under the namespace {0}.)",
		    libraryName);
		return std::nullopt;
	}
	std::unique_ptr<Objc::Cache> cache = std::make_unique<Objc::Cache>();
	cache->m_moduleName = libraryName;
	Objc::Proxy::ModuleFile moduleFile;

	std::queue<IR::Namespace const*> namespaces;
	namespaces.push(&rootNamespace);

	// If there is a namespace that has the same name as the rootModule
	while (!namespaces.empty()) {
		auto currentNamespace = namespaces.front();
		if (auto m = Objc::Builders::buildModule(*currentNamespace, *cache)) {
			if (addNamespaceObjects(moduleFile,
			                        *currentNamespace,
			                        m.value().getName(),
			                        *cache)) {
				moduleFile.addModule(m.value());
			}
		}

		// Go deeper into the nested namespaces
		for (auto const& subNamespace : currentNamespace->m_namespaces) {
			namespaces.push(&subNamespace);
		}

		// Need currentNamespace and currentModule to live this far
		namespaces.pop();
	}

	moduleFile.setCache(std::move(cache));
	return moduleFile;
}
}    // namespace Objc::Builders

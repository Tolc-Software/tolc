#include "Objc/Proxy/moduleFile.hpp"
#include "Objc/Proxy/attribute.hpp"
#include "Objc/Proxy/class.hpp"
#include "Objc/Proxy/enum.hpp"
#include "Objc/Proxy/function.hpp"
#include "Objc/cache.hpp"
#include <fmt/format.h>
#include <string>

namespace Objc::Proxy {

namespace {
std::string
joinClassDeclarations(std::vector<Objc::Proxy::Class> const& classes) {
	std::string out;

	for (auto const& cls : classes) {
		out += cls.declareCategory();
	}
	return out;
}
}    // namespace

ModuleFile::ModuleFile() : m_modules(), m_classes(), m_enums(), m_cache() {}

void ModuleFile::sortAllStructures() {
	// Is the only place m_allStructures should change
	if (!m_allStructures.empty()) {
		return;
	}
	m_allStructures.reserve(m_enums.size() + m_modules.size() +
	                        m_classes.size() + m_functions.size() +
	                        m_attributes.size());

	for (auto& e : m_enums) {
		e.m_kind = Structure::Kind::Enum;
		m_allStructures.push_back(&e);
	}

	for (auto& m : m_modules) {
		m.m_kind = Structure::Kind::Namespace;
		m_allStructures.push_back(&m);
	}

	for (auto& cls : m_classes) {
		cls.m_kind = Structure::Kind::Class;
		// Here all the functions of the interface are seen.
		// Safe to see if this class has ever been used by a shared_ptr
		if (m_cache->m_sharedPtrClasses.contains(cls.getCppClassName())) {
			cls.setAsManagedByShared();
		}
		m_allStructures.push_back(&cls);
	}

	for (auto& f : m_functions) {
		f.m_kind = Structure::Kind::Function;
		m_allStructures.push_back(&f);
	}

	for (auto& a : m_attributes) {
		a.m_kind = Structure::Kind::Attribute;
		m_allStructures.push_back(&a);
	}

	std::sort(m_allStructures.begin(),
	          m_allStructures.end(),
	          [](auto const* s0, auto const* s1) -> bool {
		          return s0->m_id < s1->m_id;
	          });
}

std::string ModuleFile::getObjcHeader() {
	sortAllStructures();
	std::string out = "#pragma once\n#import <Foundation/Foundation.h>\n";

	for (auto const& s : m_allStructures) {
		out += s->getObjcHeader();
	}

	return out;
}

std::string createExtraFunctionsSource(Objc::Cache const& cache) {
	std::string out;
	if (!cache.m_extraFunctions.empty() ||
	    !cache.m_extraClassConversions.empty()) {
		out += fmt::format(R"(
namespace {} {{
{}
{}

}})",
		                   cache.m_extraFunctionsNamespace,
		                   fmt::join(cache.m_extraClassConversions, "\n"),
		                   fmt::join(cache.m_extraFunctions, "\n"));
	}
	return out;
}

std::string ModuleFile::getObjcSource() {
	sortAllStructures();
	std::string out = fmt::format(
	    R"(
#import "{libraryName}.h"
#import <Foundation/Foundation.h>
#include <memory>
{classDeclarations}
{conversions}
)",
	    fmt::arg("classDeclarations", joinClassDeclarations(m_classes)),
	    fmt::arg("conversions", createExtraFunctionsSource(*m_cache)),
	    fmt::arg("libraryName", m_cache->m_moduleName));

	for (auto const& structure : m_allStructures) {
		out += structure->getObjcSource();
	}

	return out;
}

std::string ModuleFile::getBridgingHeader() const {
	return fmt::format("#include \"{libraryName}.h\"",
	                   fmt::arg("libraryName", m_cache->m_moduleName));
}

std::filesystem::path ModuleFile::getObjcHeaderFile() const {
	return m_cache->m_moduleName + ".h";
}

std::filesystem::path ModuleFile::getObjcSourceFile() const {
	return m_cache->m_moduleName + "_objc.mm";
}

std::filesystem::path ModuleFile::getBridgingHeaderFile() const {
	return m_cache->m_moduleName + "_objc-Bridging-Header.h";
}

void ModuleFile::setCache(std::unique_ptr<Objc::Cache> cache) {
	m_cache = std::move(cache);
}

void ModuleFile::addEnum(Objc::Proxy::Enum const& e) {
	m_enums.push_back(e);
}

void ModuleFile::addClass(Objc::Proxy::Class const& c) {
	m_classes.push_back(c);
}

void ModuleFile::addModule(Objc::Proxy::Class const& m) {
	m_modules.push_back(m);
}

void ModuleFile::addFunction(Objc::Proxy::Function const& f) {
	m_functions.push_back(f);
}

void ModuleFile::addAttribute(Objc::Proxy::Attribute const& v) {
	m_attributes.push_back(v);
}

std::vector<Structure const*> const& ModuleFile::getStructures() {
	return m_allStructures;
}

}    // namespace Objc::Proxy

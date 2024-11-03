#include "Objc/Proxy/function.hpp"
#include "Objc/Proxy/type.hpp"
#include "Objc/getName.hpp"
#include "Objc/utility.hpp"
#include "ObjcSwift/Helpers/getDocumentationParameter.hpp"
#include "ObjcSwift/Helpers/string.hpp"
#include "ObjcSwift/Helpers/wrapInFunction.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <string>

namespace Objc::Proxy {

namespace {

std::string getCallArguments(
    std::vector<Objc::Proxy::Function::Argument> const& arguments) {
	// Get the typenames of the arguments
	std::vector<std::string> names;
	std::transform(arguments.begin(),
	               arguments.end(),
	               std::back_inserter(names),
	               [&](auto const& argument) {
		               return ObjcSwift::Helpers::wrapInFunction(
		                   argument.m_name,
		                   argument.m_type.m_conversions.m_toCpp);
	               });
	return fmt::format("{}", fmt::join(names, ", "));
}

std::string getDeclarationArguments(
    std::vector<Objc::Proxy::Function::Argument> const& arguments) {
	// (int)x y:(int)y z:(int)z
	// Get the typenames of the arguments
	// The first one doesn't start with a name
	bool isFirst = true;
	std::string out;
	for (auto const& arg : arguments) {
		if (!isFirst) {
			out += arg.m_name + ':';
		}
		isFirst = false;
		out += fmt::format("({type}){name} ",
		                   fmt::arg("type", getNullableName(arg.m_type)),
		                   fmt::arg("name", arg.m_name));
	}
	if (!arguments.empty()) {
		// Remove the last space
		out.pop_back();
	}
	return out;
}
}    // namespace

std::string Function::getFunctionDeclaration() const {
	// - (int)add:(int)x y:(int)y;
	auto arguments = getDeclarationArguments(m_arguments);
	return fmt::format(
	    R"({static}({returnType}) {functionName}{arguments})",
	    fmt::arg("static", m_isStatic ? "+" : "-"),
	    fmt::arg("returnType", getNullableName(m_returnType)),
	    fmt::arg("functionName", m_name),
	    fmt::arg("arguments", arguments.empty() ? arguments : ':' + arguments));
}

std::string Function::getFunctionCall() const {
	auto arguments = getCallArguments(m_arguments);
	std::string functionCall = "";
	if (m_isStatic) {
		functionCall =
		    fmt::format(R"({dereferenced}{fullyQualifiedName}({arguments}))",
		                fmt::arg("dereferenced", m_returnType.m_dereference),
		                fmt::arg("fullyQualifiedName", m_fullyQualifiedName),
		                fmt::arg("arguments", arguments));
	} else {
		functionCall =
		    fmt::format("{dereferenced}m_object->{name}({arguments})",
		                fmt::arg("dereferenced", m_returnType.m_dereference),
		                fmt::arg("name", m_cppName),
		                fmt::arg("arguments", arguments));
	}
	return m_returnType.m_name() == "void" ?
               functionCall :
               ObjcSwift::Helpers::wrapInFunction(
	               functionCall, m_returnType.m_conversions.m_toObjc);
}

std::string Function::getFunctionBody() const {
	if (m_isConstructor) {
		return fmt::format(
		    R"(  if (self = [super init]) {{
    m_object = std::unique_ptr<{cppClass}>(new {cppClass}({arguments}));
  }}
  return self;)",
		    fmt::arg("cppClass", m_cppClass),
		    fmt::arg("arguments", getCallArguments(m_arguments)));
	}

	// Class function
	return fmt::format(
	    "  {maybeReturn}{functionCall};",
	    fmt::arg("maybeReturn",
	             m_returnType.m_name() == "void" ? "" : "return "),
	    fmt::arg("functionCall", getFunctionCall()));
}

std::string Function::getObjcSource() const {
	auto definition = fmt::format(
	    R"(
{declaration} {{
{body}
}}
)",
	    fmt::arg("declaration", getFunctionDeclaration()),
	    fmt::arg("body", getFunctionBody()));
	if (m_isStandalone) {
		definition = Objc::wrapInImplementation(
		    m_objcClass,
		    Objc::getCategoryName('f', m_objcClass, m_name, m_overloadedIndex),
		    definition);
	}
	return definition;
}

std::string Function::getObjcHeader() const {
	// E.g.
	// - (int)add:(int)x y:(int)y;
	auto declaration = Objc::getDocumentationString(m_documentation) + '\n' +
	                   getFunctionDeclaration() + ';';

	if (m_isStandalone) {
		declaration = Objc::wrapInInterface(
		    m_objcClass,
		    Objc::getCategoryName('f', m_objcClass, m_name, m_overloadedIndex),
		    declaration);
	}

	return declaration;
}

Function::Function(std::string const& name,
                   std::string const& cppName,
                   std::string const& fullyQualifiedName,
                   std::string const& objcClass,
                   std::string const& cppClass)
    : m_name(name), m_cppName(cppName),
      m_fullyQualifiedName(fullyQualifiedName), m_objcClass(objcClass),
      m_cppClass(cppClass), m_documentation(), m_returnType(),
      m_overloadedIndex(std::nullopt), m_isStatic(false),
      m_isConstructor(false), m_isStandalone(false) {}

void Function::setReturnType(Objc::Proxy::Type const& type) {
	m_returnType = type;
}

void Function::setAsStatic() {
	m_isStatic = true;
};

void Function::setAsOverloaded(size_t overloadIndex) {
	m_overloadedIndex = overloadIndex;
};

void Function::setDocumentation(std::string const& documentation) {
	m_documentation = documentation;
}

std::string Function::getName() const {
	return m_name;
}

void Function::setAsConstructor() {
	m_isConstructor = true;
}

void Function::addArgument(Argument const& arg) {
	m_arguments.push_back(arg);
}

void Function::setAsStandalone() {
	m_isStandalone = true;
}

bool Function::isStatic() const {
	return m_isStatic;
}

bool Function::isConstructor() const {
	return m_isConstructor;
}

Objc::Proxy::Type const& Function::getReturnType() const {
	return m_returnType;
}

std::string Function::getCppName() const {
	return m_fullyQualifiedName;
}

std::vector<Objc::Proxy::Function::Argument> const&
Function::getArguments() const {
	return m_arguments;
}

bool Function::isStandalone() const {
	return m_isStandalone;
}

}    // namespace Objc::Proxy

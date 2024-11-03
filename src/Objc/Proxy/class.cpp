#include "Objc/Proxy/class.hpp"
#include "Objc/utility.hpp"
#include "ObjcSwift/Helpers/getDocumentationParameter.hpp"
#include "ObjcSwift/Helpers/wrapInFunction.hpp"
#include <fmt/format.h>
#include <string>

namespace Objc::Proxy {

namespace {
std::string joinFunctions(std::vector<Objc::Proxy::Function> const& functions,
                          bool isSource) {
	std::string out;
	if (isSource) {
		for (auto const& f : functions) {
			out += f.getObjcSource();
		}
	} else {
		for (auto const& f : functions) {
			out += f.getObjcHeader();
		}
	}
	return out;
}

std::string getUnderlyingObject(std::string const& fullyQualifiedName,
                                bool isManagedByShared) {
	// NOTE: The object is actually only public for the implementation file
	//       Used for copy constructors
	return fmt::format(
	    R"( {{
  // The corresponding C++ object
  @public std::{smart_ptr}<{fullyQualifiedName}> m_object;
}}

// Copy constructor
-(instancetype) Tolc_initWithCppObject:({fullyQualifiedName} const&)cppClass {{
  if (self = [super init]) {{
    m_object = std::{smart_ptr}<{fullyQualifiedName}>(new {fullyQualifiedName}(cppClass));
  }}
  return self;
}}

-({fullyQualifiedName}&) Tolc_getCppObject {{
  return *m_object.get();
}}

-(instancetype) Tolc_initWithSmartPtr:(std::{smart_ptr}<{fullyQualifiedName}>)cppClass {{
  if (self = [super init]) {{
    m_object = {leftMove}cppClass{rightMove};
  }}
  return self;
}}

-(std::{smart_ptr}<{fullyQualifiedName}>) Tolc_getUnderlyingSmartPtr {{
  return {leftMove}m_object{rightMove};
}}
)",
	    fmt::arg("smart_ptr", isManagedByShared ? "shared_ptr" : "unique_ptr"),
	    fmt::arg("leftMove", isManagedByShared ? "" : "std::move("),
	    fmt::arg("rightMove", isManagedByShared ? "" : ")"),
	    fmt::arg("fullyQualifiedName", fullyQualifiedName));
}
}    // namespace

std::string Class::getObjcSource() const {
	bool isSource = true;
	std::string out = fmt::format(
	    R"(
@implementation {className}{underlyingObject}
{constructors}{functions}{memberVariables}
@end)",
	    fmt::arg("className", m_name),
	    fmt::arg("underlyingObject",
	             m_isPurelyStatic ? "" :
                                    getUnderlyingObject(m_fullyQualifiedName,
	                                                    m_isManagedByShared)),
	    fmt::arg("constructors", joinFunctions(m_constructors, isSource)),
	    fmt::arg("functions", joinFunctions(m_functions, isSource)),
	    fmt::arg("memberVariables", joinMemberVariables(isSource)));

	return out;
}

std::string Class::getObjcHeader() const {
	bool isSource = false;
	std::string out = fmt::format(
	    R"({documentation}
@interface {className} : NSObject
{constructors}{functions}{memberVariables}
@end
)",
	    fmt::arg("documentation",
	             Objc::getDocumentationString(m_documentation)),
	    fmt::arg("className", m_name),
	    fmt::arg("constructors", joinFunctions(m_constructors, isSource)),
	    fmt::arg("functions", joinFunctions(m_functions, isSource)),
	    fmt::arg("memberVariables", joinMemberVariables(isSource)));

	return out;
}

std::string Class::declareCategory() const {
	return fmt::format(
	    R"(
@interface {objcName}(e_{objcName}_private)
-(instancetype) Tolc_initWithCppObject:({cppName} const&)cppClass;
-(instancetype) Tolc_initWithSmartPtr:(std::{smart_ptr}<{cppName}>)cppClass;
-(std::{smart_ptr}<{cppName}>) Tolc_getUnderlyingSmartPtr;
-({cppName}&) Tolc_getCppObject;
@end
)",
	    fmt::arg("objcName", getName()),
	    fmt::arg("cppName", m_fullyQualifiedName),
	    fmt::arg("smart_ptr",
	             m_isManagedByShared ? "shared_ptr" : "unique_ptr"));
}

Class::Class(std::string const& name, std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName), m_constructors(),
      m_functions(), m_memberVariables(), m_enums(), m_isManagedByShared(false),
      m_isPurelyStatic(false) {}

void Class::addEnum(Enum const& e) {
	m_enums.push_back(e);
}

void Class::addFunction(Function const& function) {
	m_functions.push_back(function);
}

void Class::addConstructor(Function const& constructor) {
	m_constructors.push_back(constructor);
}

void Class::addMemberVariable(Attribute const& variable) {
	m_memberVariables.push_back(variable);
}

std::string const& Class::getName() const {
	return m_name;
}

void Class::setAsManagedByShared() {
	m_isManagedByShared = true;
}

void Class::setDocumentation(std::string const& documentation) {
	m_documentation = documentation;
}

void Class::setInherited(std::vector<std::string> const& inherited) {
	for (auto const& i : inherited) {
		m_inherited.push_back(i);
	}
}

std::string Class::joinMemberVariables(bool isSource) const {
	std::string out;
	if (isSource) {
		for (auto const& variable : m_memberVariables) {
			out += variable.getObjcSource();
		}
	} else {
		for (auto const& variable : m_memberVariables) {
			out += variable.getObjcHeader();
		}
	}
	return out;
}

void Class::setAsPurelyStatic() {
	m_isPurelyStatic = true;
}

std::string const& Class::getCppClassName() const {
	return m_fullyQualifiedName;
};

std::vector<Function> const& Class::getFunctions() const {
	return m_functions;
}

std::vector<Function> const& Class::getConstructors() const {
	return m_constructors;
}

std::vector<Attribute> const& Class::getAttributes() const {
	return m_memberVariables;
}

}    // namespace Objc::Proxy

#include "Swift/Builders/attributeBuilder.hpp"
#include "Swift/Builders/typeBuilder.hpp"
#include <string>

namespace Swift::Builders {

Swift::Proxy::Attribute
buildAttribute(Objc::Proxy::Attribute const& objcAttribute,
               std::string const& callFrom) {
	Swift::Proxy::Attribute attr(objcAttribute.getName(),
	                             buildType(objcAttribute.getType()));

	if (objcAttribute.isStatic()) {
		attr.setAsStatic();
	}

	std::string callPath = "";

	if (objcAttribute.isStandalone() || objcAttribute.isStatic()) {
		callPath = callFrom;
	} else {
		callPath = "m_object";
	}
	callPath += "." + objcAttribute.getName();

	attr.addGetter(callPath);

	if (!objcAttribute.isConst()) {
		attr.addSetter(callPath);
	}

	return attr;
}
}    // namespace Swift::Builders

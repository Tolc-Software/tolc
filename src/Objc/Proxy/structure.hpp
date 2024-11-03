#pragma once

#include <string>

namespace Objc::Proxy {
/*
* An Structure is any code structure
* namespace, class, function etc.
*/
struct Structure {
	virtual ~Structure();
	virtual std::string getObjcSource() const = 0;
	virtual std::string getObjcHeader() const = 0;

	enum class Kind { Class, Namespace, Function, Enum, Attribute };

	size_t m_id = 0;
	Kind m_kind;
};
}    // namespace Objc::Proxy

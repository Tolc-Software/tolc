#include "Objc/Conversions/utility.hpp"
#include "Objc/Conversions/conversion.hpp"
#include <string>

namespace Objc::Conversions {

Conversion addNamespace(Conversion c, std::string const& ns) {
	if (!ns.empty()) {
		c.m_toCpp = ns + "::" + c.m_toCpp;
		c.m_toObjc = ns + "::" + c.m_toObjc;
	}
	return c;
}

}    // namespace Objc::Conversions

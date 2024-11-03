#include "Pybind/Builders/attributeBuilder.hpp"
#include "Pybind/Proxy/typeInfo.hpp"
#include "Pybind/checkType.hpp"
#include <string>

namespace Pybind::Builders {

Pybind::Proxy::Attribute buildAttribute(std::string const& parentNamespace,
                                        IR::Variable const& v,
                                        Pybind::Proxy::TypeInfo& typeInfo) {
	Pybind::Proxy::Attribute attr(v.m_name, parentNamespace + "::" + v.m_name);
	Pybind::checkType(v.m_type, typeInfo);

	return attr;
}
}    // namespace Pybind::Builders

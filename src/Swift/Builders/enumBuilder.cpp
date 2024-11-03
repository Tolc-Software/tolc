#include "Swift/Builders/enumBuilder.hpp"
#include <string>

namespace Swift::Builders {

Swift::Proxy::Enum buildEnum(IR::Enum const& e) {
	Swift::Proxy::Enum proxyEnum(e.m_name, e.m_representation);

	proxyEnum.setScoped(e.m_isScoped);
	proxyEnum.setDocumentation(e.m_documentation);

	for (auto const& value : e.m_values) {
		proxyEnum.addValue(value);
	}

	return proxyEnum;
}
}    // namespace Swift::Builders

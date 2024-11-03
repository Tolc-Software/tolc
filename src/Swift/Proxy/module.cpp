#include "Swift/Proxy/module.hpp"
#include <fmt/format.h>
#include <string>

namespace Swift::Proxy {

std::string Module::getSwift() const {
	return fmt::format(R"(
extension {} {{
  public class {} {{}}
}}
)",
	                   m_extending,
	                   m_moduleName);
}

Module::Module(std::string const& moduleName, std::string const& extending)
    : m_moduleName(moduleName), m_extending(extending) {}

}    // namespace Swift::Proxy

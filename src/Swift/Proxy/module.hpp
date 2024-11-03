#pragma once

#include <string>

namespace Swift::Proxy {

struct Module {
	explicit Module(std::string const& moduleName,
	                std::string const& extending);

	std::string getSwift() const;

private:
	std::string m_moduleName;
	std::string m_extending;
};
}    // namespace Swift::Proxy

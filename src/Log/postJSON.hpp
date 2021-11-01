#pragma once

#include <optional>
#include <string>

namespace Log {
/**
 * Post some JSON to the host
 * Ex:
 *   postJSON("myhost.com", "8080", "/my/api/path", R"({"attr": 123})")
 */
std::optional<std::string> postJSON(std::string const& host,
                                    std::string const& port,
                                    std::string const& target,
                                    std::string const& json);
}    // namespace Log

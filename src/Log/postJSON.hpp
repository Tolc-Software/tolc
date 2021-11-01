#pragma once

#include <string>

namespace Log {
/**
 * Post some JSON to the host via HTTP POST
 * Returns true if connnection closed gracefully
 * (does not guarantee that the message was recieved)
 * Ex:
 *   postJSON("myhost.com", "8080", "/my/api/path", R"({"attr": 123})")
 */
bool postJSON(std::string const& host,
              std::string const& port,
              std::string const& target,
              std::string const& json);
}    // namespace Log

#pragma once

#include <string>

namespace TestUtil {
/*
 * Starts a server on ipAdress:port and returns the first request body it gets
 * NOTE: Blocks
 */
std::string startServer(std::string const& ipAddress, unsigned short port);
}    // namespace TestUtil

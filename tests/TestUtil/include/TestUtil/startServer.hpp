#pragma once

#include <filesystem>

namespace TestUtil {
/*
 * Creates a server that can be used in tests
 */
void startServer(std::filesystem::path serverScript);
}

#pragma once

#include <filesystem>

namespace TestUtil {
/*
 * Starts the python script stored in serverScript and passes it outFile
 */
void startServer(std::filesystem::path serverScript,
                 std::filesystem::path outFile);
}

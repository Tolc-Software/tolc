#include "TolcInternal/getExecutablePath.hpp"
#include <boost/dll/runtime_symbol_info.hpp>
#include <filesystem>

namespace TolcInternal {
std::filesystem::path getExecutablePath() {
	auto currentPath = boost::dll::program_location();
	return currentPath.c_str();
}
}    // namespace TolcInternal

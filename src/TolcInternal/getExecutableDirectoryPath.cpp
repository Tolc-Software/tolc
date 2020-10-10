#include "TolcInternal/getExecutableDirectoryPath.hpp"
#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/filesystem.hpp>
#include <filesystem>

namespace TolcInternal {
std::filesystem::path getExecutableDirectoryPath() {
	std::filesystem::path executable = boost::dll::program_location().c_str();
	return executable.parent_path();
}
}    // namespace TolcInternal

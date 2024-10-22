#include "Helpers/commandLineArgs.hpp"
#include "Helpers/Utils/combine.hpp"
#include "Parser/Windows/systemIncludeHelper.hpp"
#include "Parser/Config.hpp"
#include <string>
#include <vector>
#include <fmt/format.h>

namespace Helpers {

std::vector<std::string> getSystemIncludes() {
	// This is substituted by CMake to the correct path
	return {"-isystem/Users/simryd/code/Parser/build/_deps/llvm_entry-src/include/c++/v1", "-isystem/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include", "-isystem/Users/simryd/code/Parser/build/_deps/llvm_entry-src/lib/clang/19.1.0/include", "-isystem/Users/simryd/code/Parser/build/_deps/llvm_entry-src/lib/clang/19/include", "-isystem/usr/local/include", "-isystem/System/Library/Frameworks", "-isystem/Library/Frameworks"};
}

std::vector<std::string> getCommandLineArgs(Parser::Config const& config) {
	// -nostdinc++ - Do not search standard places after std headers
	// --language c++ - Force the input to be interpreted as C++
	auto systemIncludes = config.m_systemIncludes.empty() ?
	                          Helpers::getSystemIncludes() :
                              config.m_systemIncludes;
	return Helpers::Utils::combine(
	    systemIncludes,
	    {"-nostdinc++",
	     "--language",
	     "c++",
	     fmt::format("-std=c++{}", config.m_cppVersion),
	     "-fparse-all-comments",
	     "-Wno-everything"});
}

}    // namespace Helpers

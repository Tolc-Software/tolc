#include <IR/ir.hpp>
#include <Parser/Config.hpp>
#include <Parser/Windows/systemIncludeHelper.hpp>


namespace TestUtil {

Parser::Config getParserConfig() {
	auto config = Parser::Config();
	// This is auto filled by CMake
	config.m_systemIncludes = {"-isystem/Users/simonrydell/code/tolc/build/_deps/llvm_entry-src/include/c++/v1", "-isystem/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include", "-isystem/Users/simonrydell/code/tolc/build/_deps/llvm_entry-src/lib/clang/19.1.0/include", "-isystem/Users/simonrydell/code/tolc/build/_deps/llvm_entry-src/lib/clang/19/include", "-isystem/usr/local/include", "-isystem/System/Library/Frameworks", "-isystem/Library/Frameworks"};
	return config;
}

}

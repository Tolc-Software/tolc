#include "TolcInternal/getExecutableDirectoryPath.hpp"
#include <boost/ut.hpp>
#include <filesystem>

int main() {
	using namespace boost::ut;

	"getExecutableDirectoryPath() returns the full path to this test executable"_test =
	    [] {
		    auto exePath = TolcInternal::getExecutableDirectoryPath();
		    // We should get something
		    expect((!std::filesystem::is_empty(exePath)) >> fatal);
		    // It has to be the full path
		    expect(exePath.is_absolute());
		    // It should be a directory
		    expect(std::filesystem::is_directory(exePath));
	    };
}

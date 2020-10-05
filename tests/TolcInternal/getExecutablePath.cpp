#include "TolcInternal/getExecutablePath.hpp"
#include <boost/ut.hpp>

int main() {
	using namespace boost::ut;

	"getExecutablePath() returns the full path to this test executable"_test =
	    [] {
		    auto exePath = TolcInternal::getExecutablePath();
		    // It has to be the full path
		    expect(exePath.is_absolute());
		    // The test executable name is this files name without extensions
		    // (set in tests/CMakeLists.txt)
		    expect(exePath.stem() == "getExecutablePath");
	    };
}

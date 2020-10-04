#include <boost/ut.hpp>
#include "TolcInternal/getSystemIncludes.hpp"

int main() {
	using namespace boost::ut;

	"System includes should never return empty"_test = [] {
		auto systemIncludes =
		    TolcInternal::getSystemIncludes("/path/to/libcxx");
		expect(!systemIncludes.empty());
	};

	"All system include start with '-isystem'"_test = [] {
		auto systemIncludes =
		    TolcInternal::getSystemIncludes("/path/to/libcxx");
		expect((!systemIncludes.empty()) >> fatal);
		std::string systemFlag = "-isystem";
		for (auto const& include : systemIncludes) {
			auto start = include.find(systemFlag);
			// It must be in there
			expect((start != std::string::npos) >> fatal);
			// Should be at the beginning
			expect(start == 0);
		}
	};
}

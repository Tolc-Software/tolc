#include "TolcInternal/getSystemIncludes.hpp"
#include <catch2/catch.hpp>

TEST_CASE("System includes should never return empty", "[getSystemIncludes]") {
	auto systemIncludes = TolcInternal::getSystemIncludes("/path/to/libcxx");
	REQUIRE(!systemIncludes.empty());
};

TEST_CASE("All system include start with '-isystem'", "[getSystemIncludes]") {
	auto systemIncludes = TolcInternal::getSystemIncludes("/path/to/libcxx");
	REQUIRE(!systemIncludes.empty());
	std::string systemFlag = "-isystem";
	for (auto const& include : systemIncludes) {
		auto start = include.find(systemFlag);
		// It must be in there
		REQUIRE(start != std::string::npos);
		// Should be at the beginning
		REQUIRE(start == 0);
	}
};

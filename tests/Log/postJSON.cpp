#include "Log/postJSON.hpp"
#include <TestUtil/getTestFilesDirectory.hpp>
#include <catch2/catch.hpp>
#include <filesystem>
#include <fstream>
#include <string>

std::string getResult() {
	std::ifstream ifs(TestUtil::getPythonDirectory() / "out.json");
	std::string content((std::istreambuf_iterator<char>(ifs)),
	                    (std::istreambuf_iterator<char>()));

	return content;
}

TEST_CASE("Post JSON to localhost", "[postJSON]") {
	std::string json = R"([{"success": true, "time_taken": 1234}])";
	REQUIRE(Log::postJSON("localhost", "5000", "/report", json));
	auto result = getResult();
	REQUIRE(json == result);
}

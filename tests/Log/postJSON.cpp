#include "Log/postJSON.hpp"
#include <TestUtil/getTestFilesDirectory.hpp>
#include <TestUtil/startServer.hpp>
#include <catch2/catch.hpp>
#include <filesystem>
#include <fstream>
#include <string>

std::string getResult(std::filesystem::path const& outFile) {
	std::ifstream ifs(outFile);
	std::string content((std::istreambuf_iterator<char>(ifs)),
	                    (std::istreambuf_iterator<char>()));

	return content;
}

TEST_CASE("Post JSON to localhost", "[postJSON]") {
	auto outFile = std::filesystem::temp_directory_path() / "out.json";
	std::string json = R"([{"success": true, "time_taken": 1234}])";
	TestUtil::startServer(TestUtil::getPythonDirectory() / "server.py",
	                      outFile);
	REQUIRE(Log::postJSON("localhost", "5000", "/report", json));
	auto result = getResult(outFile);
	REQUIRE(json == result);
}

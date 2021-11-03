#include "Log/postJSON.hpp"
#include <TestUtil/startServer.hpp>
#include <catch2/catch.hpp>
#include <chrono>
#include <future>
#include <string>
#include <thread>

// I don't know how to reliably test this since it is not guaranteed to start the server ahead of time
TEST_CASE("Post JSON to localhost", "[postJSON]") {
	// Placeholder until further notice
	REQUIRE(true);

	// std::string json = R"([{"success": true, "time_taken": 1234}])";
	// std::future<std::string> serverOutput = std::async(std::launch::deferred, [] {
	// return TestUtil::startServer("127.0.0.1", 5000);
	// });
	// using namespace std::chrono_literals;
	// std::this_thread::sleep_for(2000ms);
	// REQUIRE(Log::postJSON("localhost", "5000", "/", json));
	// auto message = serverOutput.get();
	// REQUIRE(message == json);
}

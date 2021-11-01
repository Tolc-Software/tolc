#include "Log/postJSON.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Post JSON to localhost", "[postJSON]") {
	Log::postJSON(
	    "localhost", "5000", "/", R"({"userId":"1", "username": "fizz bizz"})");
}

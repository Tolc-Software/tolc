#include "Log/log.hpp"
#include "Log/postJSON.hpp"
#include <chrono>
#include <string>

namespace Log {
/**
	* Logs the time taken from start
	*/
void logTimeTaken(Log::Data data) {
	if (data.noAnalytics) {
		return;
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
	    stop - data.start);
	std::string hasSucceeded = data.success ? "true" : "false";
	std::string json =
	    R"({"success": )" + hasSucceeded + std::string(R"(, "time_taken": )") +
	    std::to_string(static_cast<int64_t>(duration.count())) + "}";
	Log::postJSON("api.tolc.io", "4000", "/report", json);
}

}    // namespace Log

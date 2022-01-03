#pragma once

#include <chrono>

namespace Log {

struct Data {
	// Start the clock on creation
	decltype(std::chrono::high_resolution_clock::now()) start =
	    std::chrono::high_resolution_clock::now();

	// Assume failure
	bool success = false;

	// Assume no analytics is to be collected
	bool noAnalytics = true;
};

/**
* Logs the data
*/
void logTimeTaken(Data data);
}

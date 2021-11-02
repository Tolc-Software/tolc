#include <TestUtil/startServer.hpp>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fmt/format.h>
#include <thread>

namespace TestUtil {

void startServer(std::filesystem::path serverScript) {
	auto venv = std::filesystem::temp_directory_path() / "venv";
	auto python = venv / "bin" / "python";
	if (!std::filesystem::exists(venv)) {
		// Setup virtualenv
		// Assumes python is in path
		std::system(fmt::format("python -m venv {}", venv.string()).c_str());
		// Install flask
		std::system(
		    fmt::format("{} -m pip install flask", python.string()).c_str());
	}

	// Start the server in a new thread (assumes it gets closed gracefully elsewhere)
	auto outFile = std::filesystem::temp_directory_path() / "out.json";
	auto serverProcess = std::thread([python, serverScript, outFile]() {
		std::system(fmt::format("{} {} {}",
		                        python.string(),
		                        serverScript.string(),
		                        outFile.string())
		                .c_str());
	});
	// Since the server cleans up after itself, this can act as a bootstrapper
	serverProcess.detach();

	// Try to ensure the server started, not the cleanest but gets the job done
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(2000ms);
}

}    // namespace TestUtil

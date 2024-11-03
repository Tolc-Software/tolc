#pragma once

/*
* Contains free functions that operate on a temporary stage (TestStage)
*/

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace TestStage {
/**
 * Adds a unittest to build directory in the stage
 */
void addPythonUnittest(std::filesystem::path const& stage,
                       std::string const& moduleName,
                       std::string const& testName,
                       std::vector<std::string> const& testBody,
                       std::vector<std::string> const& includes) {
  std::string test = "import unittest\n";
  for (auto const& include : includes) {
    test += "import " + include + '\n';
  }

  // Add the test class
  test += R"(


class Test)" +
          moduleName + R"((unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        pass
)";

  test += R"(
    def test_)" +
          testName + R"((self):
)";

  for (auto const& line : testBody) {
    test += R"(
        )" + line;
  }
  test += '\n';

  // Make it easily runnable
  test += R"(
if __name__ == "__main__":
    unittest.main()
)";

  std::filesystem::create_directories(stage / "build");

  std::string filename = "test_" + moduleName + ".py";
  std::ofstream testFile(stage / "build" / filename);
  testFile << test;
  testFile.close();
}

int runPythonUnittest(std::filesystem::path const& stage,
                      std::string const& moduleName) {
  // Save to restore after function call
  auto originalDirectory = std::filesystem::current_path();

  // Switch to stage
  std::filesystem::current_path(stage / "build");
  auto cmd = "python3 test_" + moduleName + ".py";
  std::cout << cmd << '\n';
  auto errorCode = std::system(cmd.c_str());

  std::filesystem::current_path(originalDirectory);
  return errorCode;
}
}    // namespace TestStage

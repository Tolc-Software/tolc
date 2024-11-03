#pragma once

#include "Stage/cmakeStage.hpp"

#include <IR/ir.hpp>

#include <filesystem>
#include <string>
#include <vector>

namespace TestUtil {
struct PybindStage {
  PybindStage(std::filesystem::path const& baseStage,
              std::string const& moduleName);

  /**
	* Compile and run a test
	*/
  int runPybindTest(std::string const& cppCode,
                    std::string const& pythonUnittestCode);

  /**
	* Adds the source file moduleName.hpp to the stage with provided content
	* Uses Tolc::Parser to parse the file and returns the global namespace
	*/
  IR::Namespace parseModuleFile(std::string const& content);

  /**
	* Adds a source file to the stage and imports the module file (moduleName.hpp)
	*/
  void addModuleFile(std::filesystem::path const& file,
                     std::string const& content);

  /**
	 * Exports as example. Assumes that runPybindTest has been called before
	 * (to save the cppCode and pythonUnittestCode)
	 */
  void exportAsExample(std::string const& name);

  /**
	* Injects input python code testBody into a python unittest file.
	* The content of testBody will be copied into a function and the unittest module can be used freely.
	*/
  int runPythonUnittest(std::string const& testBody);

  /**
	* Run this to keep the stage on the filesystem after the test has completed
	*/
  void keepAliveAfterTest();

  struct Code {
    std::string language;
    std::string code;
  };

  Stage::CMakeStage m_stage;
  std::string m_moduleName;
  std::vector<Code> m_exports;
};
}    // namespace TestUtil

#pragma once

#include "Stage/cmakeStage.hpp"

#include <IR/ir.hpp>

#include <array>
#include <filesystem>
#include <string>

namespace TestUtil {
struct ObjcSwiftStage {
  ObjcSwiftStage(std::filesystem::path const& baseStage,
                 std::string const& moduleName);

  /**
  * Compile and run a test
  */
  int runTest(std::string const& cppCode,
              std::string const& objCTestCode,
              std::string const& lagnuage);

  /**
   * Exports as example. Assumes that runTest has been called before
   * (to save the code)
   */
  void exportAsExample(std::string const& name);

  void addSrcFile(std::filesystem::path const& file,
                  std::string const& content);

  int runCtest(std::string const& language);

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
  Code m_cpp;
  Code m_objc;
  Code m_swift;
};
}    // namespace TestUtil

#include "TestUtil/objcSwiftStage.hpp"
#include "Frontend/Objc/frontend.hpp"
#include "Frontend/Swift/frontend.hpp"
#include "Stage/cmakeStage.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/parserConfig.hpp"

#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace TestUtil {

namespace {
/**
* Adds the source file moduleName.hpp to the stage with provided content
* Uses Tolc::Parser to parse the file and returns the parsed data
*/
IR::Namespace parseModuleFile(Stage::CMakeStage& stage,
                              std::string const& moduleName,
                              std::string const& content) {
  using path = std::filesystem::path;

  auto testFile = stage.addFile(path("include") / (moduleName + ".hpp"),
                                "#pragma once\n\n" + content);

  auto parsed = Parser::parseFile(testFile, TestUtil::getParserConfig());
  REQUIRE(parsed.has_value());
  return parsed.value();
}

void addSwiftTestBodies(Stage::CMakeStage& stage,
                        std::string const& moduleName,
                        std::string const& swift) {
  using path = std::filesystem::path;
  stage.addFile(path("tests") / "main.swift",
                fmt::format(R"(import {moduleName}

func assert(_ condition: Bool, _ message: String = "") -> Void {{
    if (!condition) {{
        fatalError(message)
    }}
}}

{code}
)",
                            fmt::arg("moduleName", moduleName),
                            fmt::arg("code", swift)));
}

void addObjcTestBodies(Stage::CMakeStage& stage,
                       std::string const& moduleName,
                       std::string const& objc) {
  using path = std::filesystem::path;
  stage.addFile(path("tests") / "main.mm",
                fmt::format(R"(
#import <{moduleName}.h>

int main() {{
  @autoreleasepool {{
{code}
  }}
}}
)",
                            fmt::arg("moduleName", moduleName),
                            fmt::arg("code", objc)));
}

std::string makeValidFileName(std::string s) {
  std::replace(s.begin(), s.end(), ' ', '_');
  std::replace(s.begin(), s.end(), ':', '_');
  return s;
}

void exportExample(std::string const& name,
                   ObjcSwiftStage::Code const& cppCode,
                   ObjcSwiftStage::Code const& code) {
  std::filesystem::path fileName =
      TestStage::getExamplesPath() / code.language /
      fmt::format("{}.md", makeValidFileName(name));

  std::string content = fmt::format(R"(
## {name} ##

```cpp
{cppCode}
```

```{language}
{code}
```

)",
                                    fmt::arg("name", name),
                                    fmt::arg("cppCode", cppCode.code),
                                    fmt::arg("language", code.language),
                                    fmt::arg("code", code.code));

  std::ofstream example(fileName);
  example << content;
  example.close();
}

bool configureAndBuild(Stage::CMakeStage& stage, std::string const& language) {
  return stage.runCommand(fmt::format(
             "cmake -S. -Bbuild-{language} -G Xcode -Dlanguage={language}",
             fmt::arg("language", language))) == 0 &&
         stage.runCommand(fmt::format("cmake --build build-{language}",
                                      fmt::arg("language", language))) == 0;
}

}    // namespace

ObjcSwiftStage::ObjcSwiftStage(std::filesystem::path const& baseStage,
                               std::string const& moduleName)
  : m_stage(baseStage, {"cmake", "tests", "CMakeLists.txt"}),
    m_moduleName(moduleName) {
  using path = std::filesystem::path;
  using std::filesystem::create_directories;
  create_directories(m_stage.m_stage / path("src-objc"));
  create_directories(m_stage.m_stage / path("src-swift"));
  create_directories(m_stage.m_stage / path("include"));
  create_directories(m_stage.m_stage / path("tests"));

  m_stage.setTargetName(m_moduleName);
}

int ObjcSwiftStage::runTest(std::string const& cppCode,
                            std::string const& testCode,
                            std::string const& language) {
  using path = std::filesystem::path;
  m_cpp = Code {"cpp", cppCode};

  if (language == "objc") {
    m_objc = Code {.language = language, .code = testCode};
    addObjcTestBodies(m_stage, m_moduleName, testCode);
  } else if (language == "swift") {
    m_swift = Code {.language = language, .code = testCode};
    addSwiftTestBodies(m_stage, m_moduleName, testCode);
  }

  auto createModule = [language](auto const& globalNS, auto const& moduleName) {
    return language == "objc" ?
               Frontend::Objc::createModule(globalNS, moduleName) :
               Frontend::Swift::createModule(globalNS, moduleName);
  };

  auto globalNS = parseModuleFile(m_stage, m_moduleName, cppCode);
  if (auto m = createModule(globalNS, m_moduleName)) {
    for (auto& [file, content] : m.value()) {
      auto ext = file.extension().string();
      if (ext == ".mm") {
        // Inject the header of the module
        content = "#import \"" + m_moduleName + ".hpp\"\n" + content;
      }

      m_stage.addFile(path("src-" + language) / file, content);
    }
    if (configureAndBuild(m_stage, language)) {
      return runCtest(language);
    }
  }

  return 1;
}

int ObjcSwiftStage::runCtest(std::string const& language) {
  return m_stage.runCommand("ctest -C Debug --output-on-failure -j1",
                            "build-" + language);
}

void ObjcSwiftStage::addSrcFile(std::filesystem::path const& file,
                                std::string const& content) {
  for (auto language : {"objc", "swift"}) {
    m_stage.addFile(
        std::filesystem::path(std::string("src-") + language) / file, content);
  }
}

void ObjcSwiftStage::exportAsExample(std::string const& name) {
  exportExample(name, m_cpp, m_objc);
  exportExample(name, m_cpp, m_swift);
}

void ObjcSwiftStage::keepAliveAfterTest() {
  m_stage.m_removeOnDestruction = false;
}
}    // namespace TestUtil

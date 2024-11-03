#include "TestUtil/embindStage.hpp"
#include "Frontend/Wasm/frontend.hpp"
#include "Stage/cmakeStage.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/parse.hpp"

#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <fmt/format.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {
// Platform independent path to jest "executable"
std::string makePathToJestBin() {
  using p = std::filesystem::path;
  return (p("node_modules") / p("jest") / p("bin") / p("jest.js")).string();
}

std::string makeValidFileName(std::string s) {
  std::replace(s.begin(), s.end(), ' ', '_');
  std::replace(s.begin(), s.end(), ':', '_');
  return s;
}

std::string
joinExports(std::vector<TestUtil::EmbindStage::Code> const& exports) {
  std::string content = "";
  for (auto const& [language, code] : exports) {
    content += fmt::format(R"(
```{language}
{code}
```

)",
                           fmt::arg("language", language),
                           fmt::arg("code", code));
  }
  return content;
}

}    // namespace

namespace TestUtil {

EmbindStage::EmbindStage(std::filesystem::path const& baseStage,
                         std::string const& moduleName)
  : m_stage(baseStage / "EmscriptenStage",
            {"cmake",
             "CMakeLists.txt",
             "configureAndBuild.bat",
             "package.json",
             "node_modules"}),
    m_moduleName(moduleName) {
  m_stage.setTargetName(m_moduleName);
  m_stage.setWindowsCMakeBuildAndConfigureScript("configureAndBuild.bat");

  m_emscripten_toolchain = baseStage / "build" / "_deps" / "emsdk_entry-src" /
                           "upstream" / "emscripten" / "cmake" / "Modules" /
                           "Platform" / "Emscripten.cmake";
}

int EmbindStage::runEmbindTest(std::string const& cppCode,
                               std::string const& jsUnittestCode,
                               std::string const& moduleName) {
  auto globalNS = parseModuleFile(cppCode);

  // Save as what has been used
  m_exports = {Code {"cpp", cppCode}, Code {"javascript", jsUnittestCode}};

  if (auto m = Frontend::Wasm::createModule(globalNS, moduleName)) {
    for (auto const& [filename, content] : m.value()) {
      if (filename.string() == "pre.js") {
        m_stage.addFile("src" / filename, content);
      } else {
        addModuleFile(filename, content);
      }
    }
    return runEmbindUnittest(jsUnittestCode);
  }

  return 1;
}

void EmbindStage::exportAsExample(std::string const& name) {
  std::filesystem::path fileName =
      TestStage::getExamplesPath() /
      fmt::format("{}.md", makeValidFileName(name));
  std::string content =
      fmt::format(R"(
## {exampleName} ##

{exampleCode}
)",
                  fmt::arg("exampleName", name),
                  fmt::arg("exampleCode", joinExports(m_exports)));

  std::ofstream example(fileName);
  example << content;
  example.close();
}

void EmbindStage::addModuleFile(std::filesystem::path const& file,
                                std::string const& content) {
  m_stage.addSourceFile(file,
                        "#include \"" + m_moduleName + ".hpp\"\n" + content);
}

IR::Namespace EmbindStage::parseModuleFile(std::string const& content) {
  auto testFile = m_stage.addSourceFile(m_moduleName + ".hpp", content);
  return TestUtil::parseFile(testFile.string());
}

int EmbindStage::runEmbindUnittest(std::string const& testBody) {
  // Make sure the toolchain file exists
  if (!std::filesystem::exists(m_emscripten_toolchain)) {
    std::cerr << "The toolchain file for Emscripen is not found: "
              << m_emscripten_toolchain.string() << '\n';
    return 1;
  }

  std::string configureCmd =
      fmt::format("cmake -S. -Bbuild -GNinja -DCMAKE_TOOLCHAIN_FILE={}",
                  m_emscripten_toolchain.string());
  std::string buildCmd = "cmake --build build";
  for (auto const& cmd : {configureCmd, buildCmd}) {
    if (auto setupError = m_stage.runCommand(cmd); setupError != 0) {
      return setupError;
    }
  }

  addJestUnitTest(testBody);

  return m_stage.runCommand("node " + makePathToJestBin());
}

void EmbindStage::addJestUnitTest(std::string const& body) {
  std::string test = fmt::format(R"(
const loadm = require('./build/defaultModule');

test('Tolc Test', () => {{
	loadm().then(m => {{
{body}
	}});
}});
	)",
                                 fmt::arg("body", body));

  std::string filename = m_moduleName + ".test.js";
  std::ofstream testFile(m_stage.m_stage / filename);
  testFile << test;
  testFile.close();
}

void EmbindStage::keepAliveAfterTest() {
  m_stage.m_removeOnDestruction = false;
}
}    // namespace TestUtil

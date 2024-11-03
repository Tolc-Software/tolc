#include "Embind/Builders/moduleFileBuilder.hpp"
#include "Embind/Proxy/module.hpp"
#include "TestUtil/string.hpp"
#include "TestUtil/types.hpp"

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Contains the correct boilerplate", "[moduleFileBuilder]") {
  IR::Namespace ns;
  IR::Function f = TestUtil::getFunction("f");
  f.m_returnType = TestUtil::getVector();
  ns.m_functions.push_back(f);

  std::string moduleName = "MyModule";

  auto file = Embind::Builders::buildModuleFile(ns, moduleName).value();
  auto path = file.getCppFilepath();
  auto embind = file.getEmbind();
  CAPTURE(path);
  CAPTURE(embind);

  REQUIRE(path == "MyModule_wasm.cpp");

  for (auto const& expectedContains : {"#include <emscripten/bind.h>",
                                       "namespace em = emscripten;",
                                       "EMSCRIPTEN_BINDINGS(MyModule)"}) {
    CAPTURE(expectedContains);
    REQUIRE(TestUtil::contains(embind, expectedContains));
  }
}

TEST_CASE(
    "Two level namespace creates two different module declarations in the preJS",
    "[moduleFileBuilder]") {
  IR::Namespace globalNS;
  IR::Namespace ns;
  std::string moduleName = "MyModule";
  ns.m_name = moduleName;
  ns.m_representation = moduleName;

  IR::Namespace nested;
  nested.m_name = "Nested";
  nested.m_representation = "MyModule::Nested";
  ns.m_namespaces.push_back(nested);
  globalNS.m_namespaces.push_back(ns);

  auto file = Embind::Builders::buildModuleFile(globalNS, moduleName).value();
  auto preJS = file.getPreJS();
  CAPTURE(preJS);

  // They seem to come in the wrong order
  for (auto const& expectedContains :
       {"Module['MyModule'] = {", "Module['MyModule']['Nested'] = {"}) {
    CAPTURE(expectedContains);
    REQUIRE(TestUtil::contains(preJS, expectedContains));
  }
}

// TEST_CASE("Two level namespace", "[moduleFileBuilder]") {
// 	IR::Namespace ns;

// 	std::vector subNamespaces = {"sub1", "sub2", "sub3"};
// 	for (auto const& subNs : subNamespaces) {
// 		IR::Namespace sub;
// 		sub.m_name = subNs;
// 		sub.m_representation = sub.m_name;
// 		ns.m_namespaces.push_back(sub);
// 	}

// 	std::string moduleName = "MyModule";
// 	auto file = Embind::Builders::buildModuleFile(ns, moduleName).value();
// 	auto path = file.getCppFilepath();
// 	auto embind = file.getEmbind();
// 	CAPTURE(path);
// 	CAPTURE(embind);

// 	REQUIRE(path == "MyModule.cpp");

// 	for (auto const& subNamespace : subNamespaces) {
// 		auto expectedContains = fmt::format(
// 		    R"(auto {moduleName}_{subNamespace} = {moduleName}.def_submodule("{subNamespace}");)",
// 		    fmt::arg("moduleName", moduleName),
// 		    fmt::arg("subNamespace", subNamespace));
// 		CAPTURE(subNamespace);
// 		CAPTURE(expectedContains);

// 		REQUIRE(TestUtil::contains(embind, expectedContains));
// 	}
// }

// TEST_CASE("Three level namespace", "[moduleFileBuilder]") {
// 	IR::Namespace ns;

// 	IR::Namespace sub;
// 	sub.m_name = "sub";
// 	sub.m_representation = sub.m_name;

// 	IR::Namespace subsub;
// 	subsub.m_name = "subsub";
// 	subsub.m_representation = sub.m_representation + "::" + subsub.m_name;

// 	// Add them in reverse order
// 	sub.m_namespaces.push_back(subsub);
// 	ns.m_namespaces.push_back(sub);

// 	std::string moduleName = "MyModule";
// 	auto file = Embind::Builders::buildModuleFile(ns, moduleName).value();
// 	auto embind = file.getEmbind();
// 	CAPTURE(embind);

// 	auto firstSub = R"(auto MyModule_sub = MyModule.def_submodule("sub");)";

// 	REQUIRE(TestUtil::contains(embind, firstSub));

// 	auto secondSub =
// 	    R"(auto MyModule_sub_subsub = MyModule_sub.def_submodule("subsub");)";
// 	REQUIRE(TestUtil::contains(embind, secondSub));
// }

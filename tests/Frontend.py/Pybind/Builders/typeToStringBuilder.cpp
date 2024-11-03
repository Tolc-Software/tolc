#include "Pybind/Builders/typeToStringBuilder.hpp"

#include <TestUtil/types.hpp>
#include <catch2/catch_test_macros.hpp>

#include <variant>

TEST_CASE("Base cases", "[typeToStringBuilder]") {
	REQUIRE(Pybind::Builders::buildTypeString(TestUtil::getType()) == "int");
	REQUIRE(Pybind::Builders::buildTypeString(TestUtil::getVector()) ==
	        "vector_int");
	REQUIRE(Pybind::Builders::buildTypeString(TestUtil::getMap()) ==
	        "map_int_int");
}

TEST_CASE("Nested cases", "[typeToStringBuilder]") {
	auto m = TestUtil::getMap();
	auto c = TestUtil::getContainer(m);
	REQUIRE(c != nullptr);
	c->m_containedTypes.back() = TestUtil::getVector();
	// map<int, vector<int>>
	REQUIRE(Pybind::Builders::buildTypeString(m) == "map_int_vector_int");
}

TEST_CASE("Multiple types", "[typeToStringBuilder]") {
	auto tuple = TestUtil::getMap();
	auto c = TestUtil::getContainer(tuple);
	REQUIRE(c != nullptr);
	c->m_container = IR::ContainerType::Tuple;
	c->m_containedTypes.push_back(TestUtil::getType());
	c->m_containedTypes.push_back(TestUtil::getType());
	// tuple<int, int, int, int>
	REQUIRE(Pybind::Builders::buildTypeString(tuple) ==
	        "tuple_int_int_int_int");
}

#include "Pybind/Builders/functionBuilder.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("function returning containers adds the correct include",
          "[functionBuilder]") {
	IR::Function f;
	IR::Type t;
	IR::Type::Container c;
	c.m_container = IR::ContainerType::Vector;
	t.m_type = c;
	f.m_returnType = t;
	Pybind::Proxy::TypeInfo typeInfo;
	auto pyFunction = Pybind::Builders::buildFunction(f, typeInfo).value();
	REQUIRE(typeInfo.m_includes.size() == 1);
	for (auto const& include : typeInfo.m_includes) {
		REQUIRE(include == "#include <pybind11/stl.h>");
	}
}

TEST_CASE("function fails on unique_ptr argument", "[functionBuilder]") {
	IR::Function f;
	f.m_representation = "MyNamespace::f";
	IR::Type t;
	IR::Type::Container c;
	c.m_container = IR::ContainerType::UniquePtr;
	t.m_type = c;
	IR::Argument v;
	v.m_type = t;
	v.m_name = "v";
	f.m_arguments.push_back(v);
	Pybind::Proxy::TypeInfo typeInfo;
	auto pyFunction = Pybind::Builders::buildFunction(f, typeInfo);
	REQUIRE(!pyFunction.has_value());
}

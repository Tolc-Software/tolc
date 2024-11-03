#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Inheritence gets parsed", "[inheritence]") {
  auto globalNS = TestUtil::parseString(R"(
class Shape {
public:
	explicit Shape(int area);

	virtual int getArea();

private:
	int area;
};

class Rectangle : public Shape {
public:
	explicit Rectangle(int _width, int _height);

	int getArea() override;

private:
	int width;
	int height;
};
)");
  auto& rectangle = TestUtil::findStruct(globalNS, "Rectangle");
  REQUIRE(!rectangle.m_public.m_inherited.empty());
  auto& base = rectangle.m_public.m_inherited.back();
  REQUIRE(base == "Shape");
}

TEST_CASE("Non-public inheritence", "[inheritence]") {
  auto globalNS = TestUtil::parseString(R"(
namespace Colors {
	class Base {
	public:
		explicit Base(int r, int g, int b);
	};

	class Green : private Base {
	public:
		explicit Green(int g);
	};
}
)");
  auto& colors = TestUtil::findNamespace(globalNS, "Colors");
  auto& green = TestUtil::findStruct(colors, "Green");
  REQUIRE(!green.m_private.m_inherited.empty());
  auto& base = green.m_private.m_inherited.back();
  REQUIRE(base == "Base");
}

TEST_CASE("Namespaced inheritence", "[inheritence]") {
  auto globalNS = TestUtil::parseString(R"(
namespace Colors {
	class Base {
	public:
		explicit Base(int r, int g, int b);
	};

	class Green : public Base {
	public:
		explicit Green(int g);
	};
}
)");
  auto& colors = TestUtil::findNamespace(globalNS, "Colors");
  auto& green = TestUtil::findStruct(colors, "Green");
  REQUIRE(!green.m_public.m_inherited.empty());
  auto& base = green.m_public.m_inherited.back();
  REQUIRE(base == "Base");
}

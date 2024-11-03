#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Basic builtin types", "[builtins]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <cstdint>

bool fbool() { return true; }

char fchar() { return 'A'; }

char16_t fchar16_t() { return 'A'; }

char32_t fchar32_t() { return 'A'; }

double fdouble() { return 1.5; }

float ffloat() { return 1.5; }

int fint() { return 42; }

int8_t fint8_t() { return 42; }

int16_t fint16_t() { return 42; }

int32_t fint32_t() { return 42; }

int64_t fint64_t() { return 42; }

long double flong_double() { return 42; }

long int flong_int() { return 42; }

long long int flong_long_int() { return 42; }

short fshort() { return 42; }

signed fsigned() { return -42; }

uint8_t fuint8_t() { return 42; }

uint16_t fuint16_t() { return 42; }

uint32_t fuint32_t() { return 42; }

uint64_t fuint64_t() { return 42; }

unsigned char funsigned_char() { return 'A'; }

unsigned int funsigned_int() { return 42; }

unsigned long int funsigned_long_int() { return 42; }

unsigned long long int funsigned_long_long_int() { return 42; }

unsigned short int funsigned_short_int() { return 42; }

wchar_t fwchar_t() { return 'A'; }
)";

  auto objcTestCode = R"(
assert([m fbool] == true);

assert([m fchar] == 'A');

assert([m fchar16_t] == 'A');

assert([m fchar32_t] == 'A');

assert([m fdouble] == 1.5);

assert([m ffloat] == 1.5);

assert([m fint] == 42);

assert([m fint8_t] == 42);

assert([m fint16_t] == 42);

assert([m fint32_t] == 42);

assert([m fint64_t] == 42);

assert([m flong_double] == 42);

assert([m flong_int] == 42);

assert([m flong_long_int] == 42);

assert([m fshort] == 42);

assert([m fsigned] == -42);

assert([m fuint8_t] == 42);

assert([m fuint16_t] == 42);

assert([m fuint32_t] == 42);

assert([m fuint64_t] == 42);

assert([m funsigned_char] == 'A');

assert([m funsigned_int] == 42);

assert([m funsigned_long_int] == 42);

assert([m funsigned_long_long_int] == 42);

assert([m funsigned_short_int] == 42);

assert([m fwchar_t] == 'A');
)";

  [[maybe_unused]] auto swiftTestCode = R"()";

  auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
  REQUIRE(errorCode == 0);
}

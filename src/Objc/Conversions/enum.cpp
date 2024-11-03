#include "Objc/Conversions/enum.hpp"
#include "Objc/Conversions/conversion.hpp"
#include "Objc/Conversions/getConversionName.hpp"
#include "Objc/Conversions/utility.hpp"
#include "Objc/Proxy/enum.hpp"
#include "Objc/cache.hpp"
#include <fmt/format.h>
#include <spdlog/spdlog.h>

namespace Objc::Conversions {
namespace {
std::string casesFromTo(std::string const& fromPrefix,
                        std::vector<std::string> const& from,
                        std::string const& toPrefix,
                        std::vector<std::string> const& to) {
	if (from.size() != to.size()) {
		spdlog::error(
		    "Something went wrong while converting Enums from C++ to Objective-C. Please open an issue.");
		return "";
	}
	std::string out;
	for (size_t i = 0; i < from.size(); ++i) {
		out += fmt::format(R"(    case {}{}:
      return {}{};
)",
		                   fromPrefix,
		                   from[i],
		                   toPrefix,
		                   to[i]);
	}

	return out;
}
}    // namespace

void setEnumConversions(IR::Enum const& cppEnum,
                        Objc::Proxy::Enum const& objcEnum,
                        Objc::Cache& cache) {
	auto names = Objc::Conversions::getEnumConversionNames(
	    cache.m_moduleName, cppEnum.m_representation);

	if (!cache.m_conversions.contains(names.m_toCpp)) {
		cache.m_conversions.insert(names.m_toCpp);

		auto valuePrefix =
		    cppEnum.m_isScoped ? cppEnum.m_representation + "::" : "";

		cache.m_extraFunctions.push_back(fmt::format(
		    R"(
{cppEnum} {toCppName}({objcEnum} e) {{
  switch (e) {{
{cases}
  }}
}})",
		    fmt::arg("cppEnum", cppEnum.m_representation),
		    fmt::arg("toCppName", names.m_toCpp),
		    fmt::arg("objcEnum", objcEnum.getName()),
		    fmt::arg(
		        "cases",
		        casesFromTo(
		            "", objcEnum.getValues(), valuePrefix, cppEnum.m_values))));

		cache.m_extraFunctions.push_back(fmt::format(
		    R"(
{objcEnum} {toObjcName}({cppEnum} e) {{
  switch (e) {{
{cases}
  }}
}})",
		    fmt::arg("objcEnum", objcEnum.getName()),
		    fmt::arg("toObjcName", names.m_toObjc),
		    fmt::arg("cppEnum", cppEnum.m_representation),
		    fmt::arg(
		        "cases",
		        casesFromTo(
		            valuePrefix, cppEnum.m_values, "", objcEnum.getValues()))));
	}
}
}    // namespace Objc::Conversions

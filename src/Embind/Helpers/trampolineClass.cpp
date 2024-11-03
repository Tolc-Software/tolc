#include "Embind/Helpers/trampolineClass.hpp"
#include "Embind/Helpers/split.hpp"
#include "Embind/Proxy/function.hpp"

#include <fmt/format.h>

#include <string>
#include <utility>
#include <vector>

namespace Embind::Helpers {

namespace {
std::string joinVirtual(std::vector<Embind::Proxy::Function> const& virtuals) {
  std::string joined;
  auto withArgumentNames = true;
  for (auto const& f : virtuals) {
    joined += fmt::format(
        R"_tolc_delimiter(
	{returnType} {name}({arguments}) {{
		return call<{returnType}>("{name}", {argumentNames});
	}}
)_tolc_delimiter",
        fmt::arg("returnType", f.getReturnType()),
        fmt::arg("name", f.getName()),
        fmt::arg("arguments", f.getArgumentTypes(withArgumentNames)),
        fmt::arg("argumentNames", f.getArgumentNames()));
  }
  return joined;
}

std::string getTrampolineClassName(std::string const& fullyQualifiedName) {
  return fmt::format(
      "Em{}", fmt::join(Embind::Helpers::split(fullyQualifiedName, "::"), "_"));
}
}    // namespace

std::pair<std::string, std::string> getTrampolineClass(
    std::string const& className,
    std::string const& fullyQualifiedName,
    std::vector<Embind::Proxy::Function> const& virtualFunctions) {
  std::string trampolineClassName = getTrampolineClassName(fullyQualifiedName);
  return std::make_pair(
      trampolineClassName,
      fmt::format(
          R"_tolc_delimiter(
class {trampolineClassName} : public em::wrapper<{fullyQualifiedName}> {{
public:
	EMSCRIPTEN_WRAPPER({trampolineClassName});

	/* Virtual trampoline functions */
{virtuals}
}};
)_tolc_delimiter",
          fmt::arg("trampolineClassName", trampolineClassName),
          fmt::arg("fullyQualifiedName", fullyQualifiedName),
          fmt::arg("className", className),
          fmt::arg("virtuals", joinVirtual(virtualFunctions))));
}
}    // namespace Embind::Helpers

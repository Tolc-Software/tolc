#include "Embind/Builders/functionBuilder.hpp"
#include "Embind/Helpers/types.hpp"
#include "Embind/Proxy/function.hpp"
#include "Embind/Proxy/typeInfo.hpp"
#include "Embind/checkType.hpp"

#include <spdlog/spdlog.h>

#include <optional>

namespace Embind::Builders {

std::optional<Embind::Proxy::Function>
buildFunction(IR::Function const& cppFunction,
              Embind::Proxy::TypeInfo& typeInfo) {
  Embind::Proxy::Function jsFunction(
      Embind::Helpers::removeCppTemplate(cppFunction.m_name),
      cppFunction.m_representation);

  for (auto const& arg : cppFunction.m_arguments) {
    if (!Embind::Helpers::isContainerType(arg.m_type,
                                          IR::ContainerType::UniquePtr)) {
      Embind::checkType(arg.m_type, typeInfo);

      jsFunction.addArgument(arg.m_type.m_representation, arg.m_name);
      if (arg.m_type.m_numPointers > 0) {
        jsFunction.allowPointers();
      }
    } else {
      spdlog::error(
          R"(The function {} takes a std::unique_ptr as one of its argument. Webassembly cannot give up ownership of an object to a function. See https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#unique-ptr for more info.)",
          cppFunction.m_representation);
      return std::nullopt;
    }
  }

  Embind::checkType(cppFunction.m_returnType, typeInfo);
  jsFunction.setReturnType(cppFunction.m_returnType.m_representation);

  if (cppFunction.m_returnType.m_numPointers > 0) {
    jsFunction.allowPointers();
  }

  return jsFunction;
}
}    // namespace Embind::Builders

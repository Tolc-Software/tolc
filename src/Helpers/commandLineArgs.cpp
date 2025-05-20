#include "Helpers/commandLineArgs.hpp"
#include "Helpers/Utils/combine.hpp"
#include "Parser/Config.hpp"
#include "Parser/Windows/systemIncludeHelper.hpp"
#include <fmt/format.h>
#include <string>
#include <vector>

namespace Helpers {

std::vector<std::string> getSystemIncludes() {
  // This is substituted by CMake to the correct path
  // clang-format off
  return ;
  // clang-format on
}

std::vector<std::string> getCommandLineArgs(Parser::Config const& config) {
  // -nostdinc++ - Do not search standard places after std headers
  // --language c++ - Force the input to be interpreted as C++
  auto systemIncludes = config.m_systemIncludes.empty() ?
                            Helpers::getSystemIncludes() :
                            config.m_systemIncludes;
  return Helpers::Utils::combine(
      systemIncludes,
      {"-nostdinc++",
       "--language",
       "c++",
       fmt::format("-std=c++{}", config.m_cppVersion),
       "-fparse-all-comments",
       "-Wno-everything"});
}

}    // namespace Helpers

#include "Pybind/Helpers/getDocumentationParameter.hpp"
#include <fmt/format.h>
#include <string>

namespace Pybind::Helpers {
std::string getDocumentationParameter(std::string const& documentation) {
  return documentation.empty() ?
             "\"\"" :
             fmt::format("R\"_tolc_docs({})_tolc_docs\"", documentation);
}
}    // namespace Pybind::Helpers

#include "Pybind/Helpers/getDocumentationParameter.hpp"
#include <string>
#include <fmt/format.h>

namespace Pybind::Helpers {
	std::string getDocumentationParameter(std::string const& documentation) {
	    return documentation.empty() ?
	               "\"\"" :
                   fmt::format("R\"_tolc_docs({})_tolc_docs\"", documentation);
    }
}

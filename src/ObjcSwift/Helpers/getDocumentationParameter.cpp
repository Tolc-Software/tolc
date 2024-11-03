#include "ObjcSwift/Helpers/getDocumentationParameter.hpp"
#include <fmt/format.h>
#include <string>

namespace ObjcSwift::Helpers {
std::string getDocumentationParameter(std::string const& documentation) {
	return documentation.empty() ?
               "\"\"" :
               fmt::format("R\"_tolc_docs({})_tolc_docs\"", documentation);
    }
    }    // namespace ObjcSwift::Helpers

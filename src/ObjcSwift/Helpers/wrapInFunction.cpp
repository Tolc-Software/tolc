#include "ObjcSwift/Helpers/wrapInFunction.hpp"
#include <string>

namespace ObjcSwift::Helpers {

std::string wrapInFunction(std::string const& arg,
                           std::string const& functionName) {
	if (functionName.empty()) {
		return arg;
	}
	return functionName + '(' + arg + ')';
}
}    // namespace ObjcSwift::Helpers


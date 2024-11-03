#pragma once
#include <string>

namespace Pybind::Helpers {
/**
* Return a valid parameter to pass to the creation of a pybind11 function
* E.g.
*   m.def("f", &f, {getDocumentationParameter('My function')})
*/
std::string getDocumentationParameter(std::string const& documentation);
}

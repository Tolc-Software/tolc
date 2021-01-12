#include <pybind11/pybind11.h>

namespace py = pybind11;

int add(int i, int j) {
	return i + j;
}

PYBIND11_MODULE(myPythonModule, myPythonModule) {
	// NOTE:    ^-- This name needs to be the same as the CMake target

	// Adding a simple function with optional help text
	// and named variables (allows python to use add(i = 5, j = 3))
	myPythonModule.def("add",
	                   &add,
	                   "A function which adds two numbers",
	                   py::arg("i"),
	                   py::arg("j"));
}

#include <functional>
// This header allows automatic conversion
// of std::function objects
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
// This header allows automatic conversion
// of many stl containers such as vector
#include <pybind11/stl.h>
#include <string>
#include <vector>

namespace py = pybind11;

int add() {
	return 0;
}

int add(int i, int j) {
	return i + j;
}

int add(int i, int j, int k) {
	return add(add(i, j), k);
}

void nothing() {}

std::vector<int> getValues(int i, int j) {
	return {i, j};
}

double functionTakingFunction(std::function<double(int)> funcArg) {
	return funcArg(5);
}

// Not supported as of yet
double cFunctionArg(double (*argumentFunction)(int)) {
	return argumentFunction(5);
}

enum class WorldlyThings { Money, Status, MeaningOfLife };

enum ThingsIWant { BetterChair, Abs, WarmSocks };

class Stuff {
public:
	Stuff(std::string something) : m_something(something) {};

	std::string m_something;
};

namespace MyNamespace {
std::string sayHello() {
	return "hello";
}
}    // namespace MyNamespace

class Animal {
public:
	virtual ~Animal() {}
	virtual std::string go(int n_times) = 0;
};

class Dog : public Animal {
public:
	std::string go(int n_times) override {
		std::string result;
		for (int i = 0; i < n_times; ++i)
			result += "woof! ";
		return result;
	}
};

std::string call_go(Animal* animal) {
	return animal->go(3);
}

// Trampoline
class PyAnimal : public Animal {
public:
	/* Inherit the constructors */
	using Animal::Animal;

	/* Trampoline (need one for each virtual function) */
	std::string go(int n_times) override {
		PYBIND11_OVERRIDE_PURE_NAME(
		    std::string, /* Return type */
		    Animal, /* Parent class */
		    "go",
		    go, /* Name of function in C++ (must match Python name) */
		    n_times /* Argument(s) */
		);
	}
};

PYBIND11_MODULE(myModule, myModule) {
	// NOTE:    ^-- This name needs to be the same as the CMake target output name

	// optional module docstring
	myModule.doc() = "pybind11 example plugin";

	// Adding a simple function with optional help text
	// and named variables (allows python to use add(i = 5, j = 3))
	myModule.def("add",
	             (int (*)(int, int)) & add,
	             "A function which adds two numbers",
	             py::arg("i"),
	             py::arg("j"));

	// Overloaded function
	myModule.def("add",
	             (int (*)(int, int, int)) & add,
	             "A function which adds three numbers",
	             py::arg("i"),
	             py::arg("j"),
	             py::arg("k"));

	// Overloaded function
	myModule.def("add", (int (*)()) & add, "A function which adds no numbers");

	// Testing void in function signature
	myModule.def(
	    "nothing", (void (*)()) & nothing, "A function which does nothing");

	myModule.def("getValues",
	             &getValues,
	             "Get the input as an array",
	             py::arg("i"),
	             py::arg("j"));

	// Function callbacks
	myModule.def("functionTakingFunction",
	             &functionTakingFunction,
	             "A function that takes a function and calls it");

	// C-Style function pointers are not supported as of yet
	// myModule.def(
	//     "cFunctionArg",
	//     [](std::function<double(int)> argumentFunction) {
	// 	    cFunctionArg(argumentFunction);
	//     },
	//     "A function that takes a C-style function and calls it");

	// NOTE: Scoped enums have 'py::arithmetic()'
	//       This adds additional convenience functions such as
	//       comparisons, and, or etc.
	py::enum_<WorldlyThings>(myModule, "WorldlyThings", py::arithmetic())
	    .value("Money", WorldlyThings::Money)
	    .value("Status", WorldlyThings::Status)
	    .value("MeaningOfLife", WorldlyThings::MeaningOfLife);

	// NOTE: Unscoped enums have 'export_values()'
	//       This exports them to the parent namespace (here myModule)
	//       This is more closely to how they are used in C++
	py::enum_<ThingsIWant>(myModule, "ThingsIWant")
	    .value("BetterChair", ThingsIWant::BetterChair)
	    .value("Abs", ThingsIWant::Abs)
	    .value("WarmSocks", ThingsIWant::WarmSocks)
	    .export_values();

	// Class with named constructor arguments
	py::class_<Stuff>(myModule, "Stuff")
	    .def(py::init<std::string>(), py::arg("something"))
	    .def_readwrite("m_something", &Stuff::m_something);

	// Adding a submodule within this module
	auto myNamespace = myModule.def_submodule("MyNamespace");
	// Adding functions within a namespace
	// is done with a fully qualified name
	myNamespace.def("sayHello", &MyNamespace::sayHello);

	py::class_<Animal, PyAnimal /* <--- trampoline*/>(myModule, "Animal")
	    .def(py::init<>())
	    .def("go", &Animal::go);

	py::class_<Dog, Animal>(myModule, "Dog").def(py::init<>());

	myModule.def("call_go", &call_go);
}

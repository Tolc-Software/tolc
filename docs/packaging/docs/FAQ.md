[TOC]

## How do I specialize a templated function/class? ##

Given a simple function, you can create a specialization on the form:

```cpp
template returnType functionName(arguments);
```

For example:

```cpp
#include <string>
#include <vector>

template <typename T>
T getSomething(T something) {
  return something;
}

template std::string getSomething(std::string);
template std::vector<std::string> getSomething(std::vector<std::string>);
```

Same for classes:

```cpp
template <typename T>
class MyClass {
public:
T fun(T type) {
	return type;
}
};

template class MyClass<int>;
```

Then from python they are available as:

```python
import MyLib

# Functions are processed as overloaded, so no name change
print(MyLib.getSomething("Tell tolc I said hi"))
print(MyLib.getSomething(something=["Or", "as", "an", "array"])

# Class specializations have type postfixes
my_class_int = MyLib.MyClass_int()
print(my_class_int.fun(42))
```

For more information about type postfixes see [the Template Naming Convention page](python/template_naming_convention.md).

## I get `ImportError: <module>: undefined symbol: _ZN15<class>1<variable>E` when I import my module ##

It might be that there is a static variable not instantiated in a source file.
Consider the example:

```cpp
class Example {
public:
	static int const i = 5;
};
```

On its own this will generate an `ImportError`. To avoid the error, instantiate the static variable inside a `.cpp` file as:

```cpp
int const Example::i;
```

For more information you can read about [static member declaration/instantiation on cppreference](https://en.cppreference.com/w/cpp/language/static), or [a discussion on this topic in the pybind repository](https://github.com/pybind/pybind11/issues/682).


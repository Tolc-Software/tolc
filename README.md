# `Tolc` #

`Tolc` is a tool for automatically creating an interface from an existing `C++` library to another language.

## Usage ##

```shell
$ ./tolc --help
```

## Architecture ##

This repository holds only the command line interface that's used in `tolc`. The rest of the functionality is separated into other libraries. This text only describes the idea behind taking user input and passing that to other support libraries. The input handling is done in some steps:

1. Parsing the arguments
2. Creating a config out of the parsed information
3. Using that config to call the support libraries

### Parsing the arguments ###

This is done by the [CLI11](https://github.com/CLIUtils/CLI11) library. Of note here is that a wrapper is put around it to be able to test the input more easily. First, the `main` function just forwards a call to a library that does the rest. The author feels a `C++` interface is easier to test programmatically and more cross platform than a command line interface.

### Creating a config out of the parsed information ###

The CLI11 library provides a way to hook into which variables are being generated when the user gives an option. E.g. if the user provides

```shell
$ tolc ... --input my_input
```

we can link the string `my_input` to some local variable in our code as:

```cpp
std::string input;
app->add_option("-i,--input", input, "Some help");
... // Parsing complete

// Will print "my_input" in this example
std::cout << input << '\n';
```

This is abstracted away by creating a `struct` that holds all the options. The `CommandLine::parse` function returns this object if the parsing was successful. This object does not have the correct types, and does not have enough information for complex error handling, so another layer is put in place to build the final `config` struct.

### Using that config to call the support libraries ###


The support libraries typically means the [`Parser`](https://github.com/srydell/Parser), or one of the frontend libraries (e.g. [`frontend.py`](https://github.com/srydell/Frontend.py)). The `Parser` is responsible for creating an `AST` of the code the user wants translated. The frontend chosen by the user then takes that `AST` and creates some glue code that can be compiled to create the final interface library.

## Full blown pipeline example ##

What happens when the user types;

```shell
$ tolc python -i include/myLib.hpp -o ./pythonBindings
```

1. `tolc` parses the command line to see that the user wants to use the `python` translator
2. `tolc` gives the file `include/myLib.hpp` to `Parser` which may return an `AST`
3. `tolc` takes the `AST` and passes it to `frontend.py` which gives back a string containing the interface code
4. `tolc` writes the interface code to the directory `./pythonBindings`

The user can then build this code to generate a library which can be used from `python`, typically like

```python
import myLib

print(myLib.do_stuff())
```



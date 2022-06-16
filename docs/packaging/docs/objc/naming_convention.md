# Naming Convention #

## Signatures which depend on a type ##

`Objective-C` does not have support for type dependent signatures so relies on each function and type signature to be unique. When `Tolc` encounters such a signature, it simply concatenates the related type into a `Type String`.

Lets clear that up with some examples:

### Overloaded Functions ###

The following `C++`:

```cpp
string sayHello() {
  return "Hi!";
}

string sayHello(string to) {
  return "Hi " + to;
}
```

Can be called from `Objective-C` as:

```objc
// Results in "Hi!"
[MyLib sayHello];
// Results in "Hi Tolc"
[MyLib sayHelloString:@"Tolc"];
```

### Templated functions ###

The following `C++`:

```cpp
template <typename T>
T getSomething(T something) {
  return something;
}

template std::string getSomething(std::string something);
```

Can be called from `Objective-C` as:

```objc
NSString* hi = [m getSomethingString:@"Hi"];
assert([hi isEqualToString:@"Hi"]);
```

### Templated classes ###

The following `C++`:

```cpp
template <typename T>
T getSomething(T something) {
  return something;
}

template std::string getSomething(std::string something);
```

Can be called from `Objective-C` as:

```objc
NSString* hi = [m getSomethingString:@"Hi"];
assert([hi isEqualToString:@"Hi"]);
```

## Type String ##

`Type String`s are used to make `C++` signatures unique on the `Objective-C` side.
The names are meant to be as predictable as possible. The rules are:

* `std::` is removed from any standard library type.
* User defined types are left untouched (i.e. the class `MyNamespace::MyClass` will result in appending `MyClass`).

| C++ type                        | Resulting name           |
|:------------------------------- |:------------------------ |
| std::array                      | Array                    |
| std::complex                    | Complex                  |
| std::deque                      | Deque                    |
| std::filesystem::path           | Path                     |
| std::forward\_list              | Forwardlist              |
| std::function                   | Function                 |
| std::list                       | List                     |
| std::map                        | Map                      |
| std::multimap                   | MultiMap                 |
| std::multiset                   | MultiSet                 |
| std::optional                   | Optional                 |
| std::pair                       | Pair                     |
| std::priority\_queue            | PriorityQueue            |
| std::queue                      | Queue                    |
| std::set                        | Set                      |
| std::shared\_ptr                | SharedPtr                |
| std::stack                      | Stack                    |
| std::tuple                      | Tuple                    |
| std::unique\_ptr                | Uniqueptr                |
| std::unordered\_map             | UnorderedMap             |
| std::unordered\_multimap        | UnorderedMultiMap        |
| std::unordered\_multiset        | UnorderedMultiSet        |
| std::unordered\_set             | Unorderedset             |
| std::valarray                   | Valarray                 |
| std::variant                    | Variant                  |
| std::vector                     | Vector                   |
| bool                            | Bool                     |
| char                            | Char                     |
| char16\_t                       | Char16t                  |
| char32\_t                       | Char32t                  |
| double                          | Double                   |
| float                           | Float                    |
| int                             | Int                      |
| Integral                        | Integral literal*        |
| long double                     | LongDouble               |
| long int                        | LongInt                  |
| long long int                   | LongLongInt              |
| short int                       | ShortInt                 |
| signed char                     | SignedChar               |
| string                          | String                   |
| string\_view                    | StringView               |
| unsigned char                   | UnsignedChar             |
| unsigned int                    | UnsignedInt              |
| unsigned long int               | UnsignedLongInt          |
| unsigned long long int          | UnsignedLongLongInt      |
| unsigned short int              | UnsignedShortInt         |
| wchar\_t                        | Wchart                   |

\* For example the `3` in `MyClass<std::array<int, 3>>` results in `MyClassArrayInt3`.

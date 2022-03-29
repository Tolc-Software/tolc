# Template Naming Convention #

When creating bindings for a templated `class` `tolc` will choose a name based on the template parameters.
For example:

```cpp
template <typename T>
class Example {
public:
T f(T type) {
	return type;
}
};

template class Example<int>;
```

The specialized `class` `Example<int>` will be available from `javascript` as `Example_int`:

```javascript
const loadMyLib = require('./build/MyLib');

loadm().then(MyLib => {
	example = new MyLib.Example_int();
	// Prints 5
	print(example.f(5))
	example.delete();
});
```

Multiple template parameters are separated with an underscore (_).
The names are meant to be as predictable as possible. The rules are:

* `std::` is removed from any standard library type.
* `_` is removed from any standard library type.
* User defined types are left untouched (i.e. the class `MyNamespace::MyClass` will result in appending `MyClass`).

## Type to string conversions ##

| C++ type                        | Resulting name           |
|:------------------------------- |:------------------------ |
| std::array                      | array                    |
| std::complex                    | complex                  |
| std::deque                      | deque                    |
| std::filesystem::path           | path                     |
| std::forward\_list              | forwardlist              |
| std::function                   | function                 |
| std::list                       | list                     |
| std::map                        | map                      |
| std::multimap                   | multimap                 |
| std::multiset                   | multiset                 |
| std::optional                   | optional                 |
| std::pair                       | pair                     |
| std::priority\_queue            | priorityqueue            |
| std::queue                      | queue                    |
| std::set                        | set                      |
| std::shared\_ptr                | sharedptr                |
| std::stack                      | stack                    |
| std::tuple                      | tuple                    |
| std::unique\_ptr                | uniqueptr                |
| std::unordered\_map             | unorderedmap             |
| std::unordered\_multimap        | unorderedmultimap        |
| std::unordered\_multiset        | unorderedmultiset        |
| std::unordered\_set             | unorderedset             |
| std::valarray                   | valarray                 |
| std::variant                    | variant                  |
| std::vector                     | vector                   |
| bool                            | bool                     |
| char                            | char                     |
| char16\_t                       | char16t                  |
| char32\_t                       | char32t                  |
| double                          | double                   |
| float                           | float                    |
| int                             | int                      |
| Integral                        | Integral literal*        |
| long double                     | longdouble               |
| long int                        | longint                  |
| long long int                   | longlongint              |
| short int                       | shortint                 |
| signed char                     | signedchar               |
| string                          | string                   |
| string\_view                    | stringview               |
| unsigned char                   | unsignedchar             |
| unsigned int                    | unsignedint              |
| unsigned long int               | unsignedlongint          |
| unsigned long long int          | unsignedlonglongint      |
| unsigned short int              | unsignedshortint         |
| wchar\_t                        | wchart                   |

\* For example the `3` in `MyClass<std::array<int, 3>>` results in `MyClass_array_int_3`.

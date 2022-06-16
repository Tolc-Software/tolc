# C++ to Objective-C conversions #

This page shows what is automatically translated and to what.
On the left is the `C++` and to the right what the corresponding interface in `Objective-C` will be.
Not converted means there will be no automatic translation to a corresponding language object.

Note that any restriction this poses only applies to the public interface of your code (e.g. your public headers).
Your internal code can use anything from C++.

## Conversion tables ##

| C++                                 | Objective-C translation                                                                                                                 |
|:----------------------------------- |:--------------------------------------------------------------------------------------------------------------------------------------- |
| Namespace                           | Prefix*                                                                                                                                 |
| Nested namespace                    | Prefix*                                                                                                                                 |
| Class                               | Class                                                                                                                                   |
| Public function                     | Class function                                                                                                                          |
| Private function                    | Not converted                                                                                                                           |
| Virtual function                    | ???                                                                                                                                     |
| Operator                            | ???                                                                                                                                     |
| Static member function              | Static class function                                                                                                                   |
| Static member variable              | Static class variable                                                                                                                   |
| Public const member variable        | Readonly property                                                                                                                       |
| Public non const member variable    | Readwrite property                                                                                                                      |
| Private member variable             | Not converted                                                                                                                           |
| Global variable                     | Static class property**                                                                                                                 |
| Global static variable              | Static class property**                                                                                                                 |
| Free function                       | Static class function**                                                                                                                 |
| Overloaded function                 | Class function***                                                                                                                       |
| Enum                                | [NS\_ENUM](https://developer.apple.com/documentation/swift/objective-c_and_c_code_customization/grouping_related_objective-c_constants) |
| Scoped enum                         | [NS\_ENUM](https://developer.apple.com/documentation/swift/objective-c_and_c_code_customization/grouping_related_objective-c_constants) |
| Templated class/function            | Not converted****                                                                                                                       |
| Specialized class template          | Class***                                                                                                                                |
| Specialized function template       | Function***                                                                                                                             |
| Documentation                       | Documentation                                                                                                                           |

\* Prefix on all classes and free functions in the namespace.

\*\* Wrapped in a class with a name that starts with the module name.

\*\*\* The naming convention for these classes and functions can be found under the [Naming Convention page](naming_convention.md).

\*\*\*\* No direct translation to Objective-C. Will not emit warning.

| C++ operator      | Objective-C operator  |
|:----------------- |:--------------------- |
| operator+         | ???                   |
| operator-         | ???                   |
| operator*         | ???                   |
| operator/         | ???                   |
| operator%         | ???                   |
| operator+=        | ???                   |
| operator-=        | ???                   |
| operator*=        | ???                   |
| operator/=        | ???                   |
| operator%=        | ???                   |
| operator==        | ???                   |
| operator!=        | ???                   |
| operator>         | ???                   |
| operator>=        | ???                   |
| operator<         | ???                   |
| operator<=        | ???                   |
| operator[]        | ???                   |
| operator()        | ???                   |
| operator=         | Not converted         |
| operator<<        | Not converted         |
| operator>>        | Not converted         |
| operator++        | Not converted         |
| operator--        | Not converted         |


| C++ Standard library class      | Objective-C translation                                                           |
|:------------------------------- |:--------------------------------------------------------------------------------- |
| std::array                      | [NSArray](https://developer.apple.com/documentation/foundation/nsarray)           |
| std::complex                    | ???                                                                               |
| std::deque                      | [NSArray](https://developer.apple.com/documentation/foundation/nsarray)           |
| std::filesystem::path           | [NSString](https://developer.apple.com/documentation/foundation/nsstring)         |
| std::forward\_list              | Not converted                                                                     |
| std::function                   | ???                                                                               |
| std::list                       | [NSArray](https://developer.apple.com/documentation/foundation/nsarray)           |
| std::map                        | [NSDictionary](https://developer.apple.com/documentation/foundation/nsdictionary) |
| std::multimap                   | Not converted                                                                     |
| std::multiset                   | Not converted                                                                     |
| std::optional                   | Value or nil                                                                      |
| std::pair                       | [NSArray](https://developer.apple.com/documentation/foundation/nsarray)           |
| std::priority\_queue            | Not converted                                                                     |
| std::queue                      | Not converted                                                                     |
| std::set                        | [NSOrderedSet](https://developer.apple.com/documentation/foundation/nsorderedset) |
| std::shared\_ptr                | Value*                                                                            |
| std::stack                      | Not converted                                                                     |
| std::string                     | [NSString](https://developer.apple.com/documentation/foundation/nsstring)         |
| std::string\_view               | [NSString](https://developer.apple.com/documentation/foundation/nsstring)         |
| std::tuple                      | [NSArray](https://developer.apple.com/documentation/foundation/nsarray)           |
| std::unique\_ptr                | Value                                                                             |
| std::unordered\_map             | [NSDictionary](https://developer.apple.com/documentation/foundation/nsdictionary) |
| std::unordered\_multimap        | Not converted                                                                     |
| std::unordered\_multiset        | Not converted                                                                     |
| std::unordered\_set             | [NSSet](https://developer.apple.com/documentation/foundation/nsset)               |
| std::valarray                   | [NSArray](https://developer.apple.com/documentation/foundation/nsarray)           |
| std::variant                    | ???                                                                               |
| std::vector                     | [NSArray](https://developer.apple.com/documentation/foundation/nsarray)           |

\* After using a `shared_ptr`, the object is now always managed by a `shared_ptr`.

| C++ builtin type           | Objective-C translation  |
|:-------------------------- |:------------------------ |
| bool                       | bool                     |
| char                       | char                     |
| char16\_t                  | char16\_t                |
| char32\_t                  | char32\_t                |
| double                     | double                   |
| float                      | float                    |
| int                        | int                      |
| int8\_t                    | int8\_t                  |
| int16\_t                   | int16\_t                 |
| int32\_t                   | int32\_t                 |
| int64\_t                   | int64\_t                 |
| long double                | long double              |
| long int                   | long int                 |
| long long int              | long long int            |
| short int                  | short int                |
| signed char                | signed char              |
| uint8\_t                   | uint8\_t                 |
| uint16\_t                  | uint16\_t                |
| uint32\_t                  | uint32\_t                |
| uint64\_t                  | uint64\_t                |
| unsigned char              | unsigned char            |
| unsigned int               | unsigned int             |
| unsigned long int          | unsigned long int        |
| unsigned long long int     | unsigned long long int   |
| unsigned short int         | unsigned short int       |
| wchar\_t                   | wchar\_t                 |
| void                       | void                     |


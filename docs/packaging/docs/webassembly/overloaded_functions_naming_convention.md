# Overloaded Functions Naming Convention #

When creating bindings for a function `f` `tolc` will choose a name based on if the function is overloaded or not.

For example:

```cpp
double f();

double f(int);
```

The two functions will be available from `javascript` as `f` and `f_int` respectively.

```python
var loadMyLib = require('./myLib.js');

loadMyLib().then(MyLib => {
	const result0 = MyLib.f();
	const result1 = MyLib.f_int();
});
```

Multiple arguments are added to the name as joined separated with an underscore (`_`).
The names are meant to be as predictable as possible. The rules are the same as for [the template naming conventions](./template_naming_convention.md).


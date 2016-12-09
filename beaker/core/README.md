
This language feature provides support for declaring variables, constants,
and functions. Note that this feature does not define any concrete types or,
expressions. The facilities here are common to every language that works
with objects and functions.

## Types

This feature exposes two types:

### Reference types

    ref t

The type `ref t` is a reference to a stored object. This is similar to a 
pointer type, except that that the set of operations on `ref t` are identical
to those of `t.

### Function types

    (t1, t2, ..., tn) -> t

The type of a function taking $n$ arguments of various type and returning
an object of type `t.

    (t1, t2, *) -> t

A variadic function type accepts an unnamed variadic argument list after its 
last declared parameter. See the variadic feature for types and expressions
that can be used to access variadic argument lists.

## Expressions

### Declaration reference

    ref d

A reference to a declared name. The type of the expression depends on the kind
of declaration (see below).

### Function call

    f(e1, e2, ..., en)

## Conversions

### Reference-to-object

A reference can be converted to an object.


## Declarations

### Constants

    const t n = e;

A constant declaration associates a name with a type and value. The declared
type of the constant `n` shall match the type of `e`.

A translation environment must evaluation the `e` and create the mapping 
between the name and its value.

### Variables

    var t n = e;

A variable declaration associates a name with a type and stored object. The
declared type of the variable `n` shall match the type of `e`.

The variable is initialized by evaluating `e` and storing that value in the
storage allocated to `n`.

### References

    ref t n = x;

A reference declaration introduces a new name for an existing object.

### Registers

    reg t n = e;

A register declaration associates a name with a type and object. The object
is required to be stored in a register. 

Note that this is not a general purpose language feature, it is simply provided
for completeness.

### Functions

    fn n(p1, p2, ..., pn) -> pr = e

A function declaration defines a mapping of input parameters to a return
parameter. A parameter can be a variable, reference, or register object.
The type of the declaration is the function type derived from the parameter
list and return value: `(t1, t2, ..., tn) -> tr` where each `t` corresponds
to that 

A variadic function is a function with a variadic function type. Note that
this information is determined entirely by the function's type, not its
sequence of declared parameters.


# Core language features

This language feature provides support for declaring and initializing variables
and defining functions and their associated operations: accessing stored values
and calling functions with arguments.

This feature is not "complete" in the sense that it cannot be used without
other language features. This feature set is primary intended to provide
support for 


## Types

This feature exposes three types:

### `void` type

The `void` type represents the type of expressions that do not compute values.
It is not an object type; it cannot be used to declare variables or parameters.
This is because there is no `void` value that can be stored in an object.

### Reference types

    t&

The type `t&` is a reference to a stored object.

### Function types

    (t1, t2, ..., tn) -> t

The type of a function taking $n$ arguments of various type and returning
a value type `t`. The parameters and return types may be references.


## Expressions

This feature defines a number of expressions related to the types above.

### `void` expressions

    `void e`

Evaluates and and discards the value of `e`. The type of the expression is 
`void`.


### Declaration reference

    `ref d`

A reference to a declaration. The type and value of the expression depend on
the type of declaration.

When `d` is the name of an object, the value of this expression is a reference
to the declared type of `v`. When `

A reference to a declared name. The type of the expression depends on the kind
of declaration (see below).

### Function call

    f(e1, e2, ..., en)

## Conversions

### Reference-to-object

A reference can be converted to an object.


## Initializers

Initializers are expressions that invoke an initialization procedure (i.e., a
constructor). Objects are produced by construction.

### Trivial initialization

Invokes a trivial constructor, which does not modify the contents of memory.
Trivially constructed objects are partially formed. in C++, these have
indeterminate value. Reading from an object with indeterminate values is
undefined behavior.

### Zero initialization

Inovkes a constructor that assigns 0 to all bits in the representation of an
object. This is defined for objects of all types except reference types.

### Copy initialization

Assigns the value of an expression to an object.

### Reference initialization

Binds a reference to the object computed by an expression.


## Declarations

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

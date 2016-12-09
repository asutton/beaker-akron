
This language feature provides support for working with variadic argument
lists. 

## Types

The type `va.list` is the type of a variadic argument list. It's layout
depends on the target operating system. This type can be used as the type
of variables (and as such) function parameters.

## Expressions

This feature adds support four expressions:

    `va.start(list)`

This indicates the start of variadic argument list processing. The operand 
shall be a reference to an object of type `va.list`. The type of the expression
is that of the operand.

    `va.end(list)`

This indicates the end of variadic argument list processing. The operand shall
be a reference to an object of type `va.list`. The type of the expression is
that of the operand.

    `va.arg(type, list)`

This returns the current argument, interpreted as an object of the given 
`type`. The operand `list` shall be a reference to an object of type `va.list`,
and the type of the expression is `type.`

    `va.copy(dst, src)`

This copies the source variadic argument list to the destination list. The
types of `dst` and `list` shall be references to objects of type `va.list`.
The type of the expression is that of `dst`.

## Issues

In general, the type `va.list` depends on the target architecture. However,
LLVM assumes that it is essentially an `i8*`. This can be managed by bit
casting. I suspect that we'll just have to generally insert that bit cast
prior to generating operands for the various expressions.

## Ideas

Design an algorithm that detects illegal operations on variadic argument lists.
There is a fairly simple state machine defining the legal operations on objects
of these types.

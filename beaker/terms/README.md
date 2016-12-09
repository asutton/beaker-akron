
This subdirectory defines subsets of languages. Each subset includes all of
the functionality needed to construct, validate, evaluate, and generate
code for a set of expressions and declarations.

The contents of each "language pack" are:

## logic

A small language dealing with terms in predicate logic.

Types:
  - bool
Expressions:
  - and
  - or
  - not
  - implication
  - equivalence

TODO: Consider adding universal and existential quantification? Those don't
really have observable behavior, but they could be useful for other analyses.

## numeric

A language subset dealing with arithmetic terms.

Dependencies:
  - logical -- relational operators return boolean values.

Types:
  - natN - unsigned integers with overflow
  - intN - signed integers with overflow
  - modN - unsigned integers without overflow
  - floatN - IEEE 754 floating point
Expressions:
  - equal
  - not equal
  - less than
  - greater than
  - less equal
  - grater equal
  - addition
  - subtraction
  - multiplication
  - division
  - remainder
  - negation
  - reciprocal

This is missing conversions and bitwise operations.

## object

A small language subset that supports declarations of common kinds of
objects and functions. These are reused by higher-level language features
that need basic declarative support.

Dependencies:
  - logic or numeric

Declarations:
  - variables -- A name/storage binding
  - constants -- A name/value binding
  - functions -- A name/mapping binding
  - parameters -- A set of name/entity bindings for function arguments.

Expressions:
  - ref  -- References to a variable, constant, function, or parameter
  - call -- Apply a sequence of arguments to a function

Types:
  - function type

TODO: With functions come issues of parameter passing. We'll need an extended
set of types in order deal with passing by value, by reference. More advanced
parameter passing include in, out, mut, fwd, and move. Possibly others.

TODO: Consider adding macros: name/term bindings (i.e., macros).

## Functional

Language support for typed lambda calculus.

Dependencies:
  - object - We don't support uninterpreted bases.

Expressions:
  - lambda
  - let
  - call
  - seq

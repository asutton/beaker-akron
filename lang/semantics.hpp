// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BPL_SEMANTICS_HPP
#define BPL_SEMANTICS_HPP

#include "lang.hpp"
#include "lexer.hpp"


namespace bpl {

/// Represents a lexical error.
///
/// \todo This is exactly the same as lex and parse errors.
struct type_error : std::runtime_error
{
  type_error(location, const char*);

  location get_location() const;

  location loc;
};

inline 
type_error::type_error(location loc, const char* msg)
  : std::runtime_error(msg), loc(loc)
{ }

inline location type_error::get_location() const { return loc; }


/// The semantic actions of the parser. This defines the meaning of a syntactic
/// expression by mapping it onto an abstract syntax with precise meaning.
///
/// The semantic context is initialized over a default AST builder for the
/// module being parsed. This provides access to that module, and indirectly
/// the source language.
struct semantics
{
  semantics(builder& b) : build(b) { }

  const language& get_language() const;
  language& get_language();

  name& on_identifier(token);

  expr& on_condition(expr&, token, expr&, token, expr&);
  expr& on_logical_or(expr&, token, expr&);
  expr& on_logical_and(expr&, token, expr&);
  expr& on_bitwise_or(expr&, token, expr&);
  expr& on_bitwise_xor(expr&, token, expr&);
  expr& on_bitwise_and(expr&, token, expr&);
  expr& on_equal(expr&, token, expr&);
  expr& on_not_equal(expr&, token, expr&);
  expr& on_less(expr&, token, expr&);
  expr& on_greater(expr&, token, expr&);
  expr& on_less_equal(expr&, token, expr&);
  expr& on_greater_equal(expr&, token, expr&);
  expr& on_addition(expr&, token, expr&);
  expr& on_subtraction(expr&, token, expr&);
  expr& on_multiplication(expr&, token, expr&);
  expr& on_division(expr&, token, expr&);
  expr& on_remainder(expr&, token, expr&);
  expr& on_negation(token, expr&);
  expr& on_bitwise_not(token, expr&);
  expr& on_logical_not(token, expr&);
  expr& on_bool(token);
  expr& on_int(token);

  void check_bool(expr&);
  void check_bool(expr&, expr&);
  void check_int(expr&);
  void check_int(expr&, expr&);
  void check_same(expr&, expr&);

  builder& build;
};

/// Returns the language for the semantics.
inline const language& semantics::get_language() const { return build.get_language(); }

/// Returns the language for the semantics.
inline language& semantics::get_language() { return build.get_language(); }

} // namespace bpl

#endif


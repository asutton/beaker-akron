// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BPL_SEMANTICS_HPP
#define BPL_SEMANTICS_HPP

#include "lang.hpp"
#include "lexer.hpp"


namespace bpl {

using beaker::scope;
using beaker::scope_stack;
using beaker::dc;


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
  semantics(module&);

  const language& get_language() const;
  language& get_language();

  name& on_identifier(token);

  // Types
  type& on_void_type(location);
  type& on_bool_type(location);
  type& on_int_type(location);

  // Expressions
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

  decl& on_start_module();
  decl& on_finish_module();
  
  decl& on_start_function(name&, decl_seq&&, type&, locations<4>);
  decl& on_finish_function(location);
  decl& on_finish_function(stmt&);

  stmt& on_block_statement(stmt_seq&&, locations<2>);

  void check_bool(expr&);
  void check_bool(expr&, expr&);
  void check_int(expr&);
  void check_int(expr&, expr&);
  void check_same(expr&, expr&);

  // Resources
  module& mod;
  beaker::sys_void::builder& build_void;
  beaker::sys_bool::builder& build_bool;
  beaker::sys_int::builder& build_int;
  beaker::sys_name::builder& build_name;
  beaker::sys_var::builder& build_var;
  beaker::sys_fn::builder& build_fn;

  // Scope management
  void enter_scope(int k);
  void leave_scope();
  const scope& current_scope() const;
  scope& current_scope();
  const decl& current_context() const;
  decl& current_context();

  scope_stack scopes;
  dc cur_cxt;
};

/// Returns the language for the semantics.
inline const language& 
semantics::get_language() const { return mod.get_language(); }

/// Returns the language for the semantics.
inline language& semantics::get_language() { return mod.get_language(); }

/// Pushes a new scope onto the scope stack.
inline void semantics::enter_scope(int k) { scopes.push(k); }

/// Pops the current scope from the scope stack.
inline void semantics::leave_scope() { scopes.pop(); }

/// Returns the current scope.
inline const scope& semantics::current_scope() const { return scopes.top(); }

/// Returns the current scope.
inline scope& semantics::current_scope() { return scopes.top(); }

/// Returns the current declaration context.
inline const decl& semantics::current_context() const { return *cur_cxt.get_context(); }

/// Returns the current declaration context.
inline decl& semantics::current_context() { return *cur_cxt.get_context(); }


// -------------------------------------------------------------------------- //
// Scopes
//
// TODO: Factor this into a separate module.

enum {
  module_scope,
  function_parameter_scope,
  function_scope,
  block_scope,
};


// -------------------------------------------------------------------------- //
// Declarative regions

/// A declarative region is a portion of program text in which names can
/// be declared as particular kinds of entities.
///
/// Entering a declarative region corresponds to entering a new scope.
///
/// \todo Support different kinds of scopes with different lookup and
/// declaration rules?
struct declarative_region
{
  declarative_region(semantics&, decl&);
  declarative_region(semantics&, int);
  ~declarative_region();

  semantics& sema;
  dc prev_dc;
};


} // namespace bpl

#endif


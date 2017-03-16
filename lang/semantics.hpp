// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BPL_SEMANTICS_HPP
#define BPL_SEMANTICS_HPP

#include "lang.hpp"
#include "lexer.hpp"

#include <beaker/base/scope.hpp>


namespace bpl {

using beaker::dc;
using beaker::scope;
using beaker::lexical_environment;


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


/// Represents a declaration error.
struct decl_error : std::runtime_error
{
  decl_error(location, const char*);

  location get_location() const;

  location loc;
};

inline 
decl_error::decl_error(location loc, const char* msg)
  : std::runtime_error(msg), loc(loc)
{ }

inline location decl_error::get_location() const { return loc; }


/// A converted pair is a pair of converted expressions, which have the
/// same type.
struct conv_pair : expr_pair
{
  using expr_pair::expr_pair;

  const type& get_type() const;
  type& get_type();
};

/// Returns the type of the converted expression pair.
inline const type& conv_pair::get_type() const { return first.get_type(); }

/// Returns the type of the converted expression pair.
inline type& conv_pair::get_type() { return first.get_type(); }


bool is_boolean(const conv_pair&);
bool is_integral(const conv_pair&);


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
  expr& on_assignment(expr&, expr&, location);
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
  expr& on_id(const symbol&, location);

  decl& on_start_module();
  decl& on_finish_module();
  
  decl& on_start_function(name&, decl_seq&&, type&, locations<4>);
  decl& on_finish_function(decl&, location);
  decl& on_finish_function(decl&, stmt&);
  decl& on_function_parameter(type&, name&);
  decl& on_start_variable(type&, name&, location);
  decl& on_finish_variable(decl&, expr&, locations<2>);

  stmt& on_expression_statement(expr&, location);
  stmt& on_declaration_statement(decl&);
  stmt& on_return_statement(locations<2>);
  stmt& on_return_statement(expr&, locations<2>);
  stmt& on_block_statement(stmt_seq&&, locations<2>);

  // Resources
  module& mod;
  sys_void::builder& build_void;
  sys_bool::builder& build_bool;
  sys_int::builder& build_int;
  sys_name::builder& build_name;
  sys_var::builder& build_var;
  sys_fn::builder& build_fn;

  // Scope management
  void enter_scope(int k);
  void leave_scope();
  const scope& current_scope() const;
  scope& current_scope();

  // Context management
  const decl& current_context() const;
  decl& current_context();
  const sys_fn::fn_decl& current_function() const;
  sys_fn::fn_decl& current_function();

  // Declarations
  decl& declare(decl&);
  decl& declare(scope&, decl&);

  // Typing
  expr& check_bool(expr&);
  void check_bool(expr&, expr&);
  expr& check_int(expr&);
  void check_int(expr&, expr&);
  void check_same(expr&, expr&);

  // Conversions
  expr& standard_conversion(expr&, type&);
  expr& boolean_conversion(expr&);
  conv_pair boolean_conversion(expr&, expr&);
  expr& arithmetic_conversion(expr&);
  conv_pair arithmetic_conversion(expr&, expr&);
  conv_pair common_conversion(expr&, expr&);

  // Initialization
  void copy_initialize(value_decl&, expr&);
  void object_initialize(value_decl&, expr&);
  void reference_initialize(value_decl&, expr&);

  lexical_environment env;
  dc cur_cxt;
};

/// Returns the language for the semantics.
inline const language& 
semantics::get_language() const { return mod.get_language(); }

/// Returns the language for the semantics.
inline language& semantics::get_language() { return mod.get_language(); }

/// Pushes a new scope onto the scope stack.
inline void semantics::enter_scope(int k) { env.enter_scope(k); }

/// Pops the current scope from the scope stack.
inline void semantics::leave_scope() { env.leave_scope(); }

/// Returns the current scope.
inline const scope& semantics::current_scope() const { return env.current_scope(); }

/// Returns the current scope.
inline scope& semantics::current_scope() { return env.current_scope(); }

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


// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef TEST_AST_STMT_HPP
#define TEST_AST_STMT_HPP

#include "build.hpp"

#include <iostream>

// -------------------------------------------------------------------------- //
// Statement builder

/// A helper class that can be used to create statements. This class is not
/// constructed directly, but rather through the `add_stmts()` function.
///
/// FIXME: The 'var' constructor is fundamentally broken because the usual
/// composition mechanism does not guarantee left to right evaluation.
struct stmt_builder
{
  stmt_builder(decl& d)
    : cxt(d), stmts(get_body(d))
  { }

  stmt_builder& run(expr&);

  stmt_builder& var(out<decl>&, name&, type&, expr&);
  stmt_builder& var(out<decl>&, const char*, type&, expr&);
  stmt_builder& check(expr&);
  stmt_builder& ret(expr&);
  
  decl& cxt;
  stmt_seq& stmts;
};


/// Add an expression statement that evaluates e and discards the value.
inline stmt_builder& 
stmt_builder::run(expr& e)
{
  stmt& s = global_builder::get().make_expr_stmt(e);
  stmts.push_back(s);
  return *this;
}

/// Create a statement that declares a local variable and adds that to the 
/// builder's list of statements.  Saves a reference to the variable in `out`.
inline stmt_builder& 
stmt_builder::var(out<decl>& v, name& n, type& t, expr& e)
{ 
  v = global_builder::get().make_local_var_decl(cxt, n, t, e);
  sys_fn::decl_stmt& stmt = global_builder::get().make_decl_stmt(v);
  stmts.push_back(stmt);
  return *this;
}

/// Create a statement that declares a local variable and adds that to the 
/// builder's list of statements.  Saves a reference to the variable in `out`.
inline stmt_builder& 
stmt_builder::var(out<decl>& v, const char* n, type& t, expr& e)
{ 
  return var(v, global_builder::get().get_name(n), t, e);
}

inline stmt_builder& 
stmt_builder::check(expr& e)
{
  sys_bool::assert_decl& decl = global_builder::get().make_assert_decl(cxt, e);
  sys_fn::decl_stmt& stmt = global_builder::get().make_decl_stmt(decl);
  stmts.push_back(stmt);
  return *this;
}

inline stmt_builder&
stmt_builder::ret(expr& e)
{
  stmt& s = global_builder::get().make_ret_stmt(e);
  stmts.push_back(s);
  return *this;
}


/// Returns a statement builder for the given function.
inline stmt_builder add_stmts(decl& d) { return {d}; }

#endif

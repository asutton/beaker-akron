// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_FN_BUILD_HPP
#define BEAKER_SYS_FN_BUILD_HPP

#include <beaker/sys.fn/fwd.hpp>
#include <beaker/sys.name/fwd.hpp>

#include <beaker/base/build.hpp>


namespace beaker {
namespace sys_fn {

/// Provides access to resources needed to construct, validate, and
/// evaluate core terms.
struct builder : factory
{
  builder(module&);

  // Canonical types
  fn_type& get_fn_type(const type_seq&, type&);
  fn_type& get_fn_type(type_seq&&, type&);
  fn_type& get_fn_type(decl_seq&, decl&);

  // Expressions
  call_expr& make_call_expr(expr&, const expr_seq&);
  call_expr& make_call_expr(expr&, expr_seq&&);
  eq_expr& make_eq_expr(expr&, expr&);
  ne_expr& make_ne_expr(expr&, expr&);

  // Function declarations
  fn_decl& make_fn_decl(dc, name&, type&, const decl_seq&, decl&);
  fn_decl& make_fn_decl(dc, const char*, type&, const decl_seq&, decl&);
  fn_decl& make_fn_decl(dc, name&, type&, decl_seq&&, decl&);
  fn_decl& make_fn_decl(dc, const char*, type&, decl_seq&&, decl&);

  // Function definitions
  fn_decl& make_fn_decl(dc, name&, type&, const decl_seq&, decl&, stmt&);
  fn_decl& make_fn_decl(dc, const char*, type&, const decl_seq&, decl&, stmt&);
  fn_decl& make_fn_decl(dc, name&, type&, decl_seq&&, decl&, stmt&);
  fn_decl& make_fn_decl(dc, const char*, type&, decl_seq&&, decl&, stmt&);
  
  // Parameters
  parm_decl& make_parm_decl(type&);
  parm_decl& make_parm_decl(name&, type&);
  parm_decl& make_parm_decl(const char*, type&);

  // Variables
  var_decl& make_var_decl(dc, name&, type&, expr&);
  var_decl& make_var_decl(dc, const char*, type&, expr&);

  // Statements
  block_stmt& make_block_stmt();
  block_stmt& make_block_stmt(const stmt_seq&);
  block_stmt& make_block_stmt(stmt_seq&&);
  expr_stmt& make_expr_stmt(expr&);
  decl_stmt& make_decl_stmt(decl&);
  ret_stmt& make_ret_stmt();
  ret_stmt& make_ret_stmt(expr&);

  canonical_term_set<fn_type>* fn_;
};

} // namespace sys_fn
} // namespace beaker


#endif

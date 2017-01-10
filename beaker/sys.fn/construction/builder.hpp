// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_FN_CONSTRUCTION_BUILDER_HPP
#define BEAKER_SYS_FN_CONSTRUCTION_BUILDER_HPP

#include <beaker/base/construction/builder.hpp>
#include <beaker/base/type.hpp>
#include <beaker/base/expr.hpp>
#include <beaker/base/decl.hpp>
#include <beaker/base/stmt.hpp>


namespace beaker {

namespace sys_name {

struct basic_name;
struct internal_name;

} // namespace

namespace sys_fn {

struct fn_type;

struct call_expr;
struct eq_expr;
struct ne_expr;

struct fn_decl;
struct parm_decl;

struct block_stmt;
struct expr_stmt;
struct decl_stmt;
struct ret_stmt;

/// Provides access to resources needed to construct, validate, and
/// evaluate core terms.
struct builder : basic_builder<sys_fn_lang>
{
  builder(module&);

  // Names
  sys_name::basic_name& get_name(const char*);
  sys_name::internal_name& get_name();

  // Canonical types
  fn_type& get_fn_type(const type_seq&, type&);
  fn_type& get_fn_type(type_seq&&, type&);
  fn_type& get_fn_type(decl_seq&, decl&);

  // Expressions
  call_expr& make_call_expr(expr&, const expr_seq&);
  call_expr& make_call_expr(expr&, expr_seq&&);
  eq_expr& make_eq_expr(expr&, expr&);
  ne_expr& make_ne_expr(expr&, expr&);

  // Declarations
  fn_decl& make_fn_decl(name&, type&, const decl_seq&, decl&);
  fn_decl& make_fn_decl(const char*, type&, const decl_seq&, decl&);
  fn_decl& make_fn_decl(name&, type&, decl_seq&&, decl&);
  fn_decl& make_fn_decl(const char*, type&, decl_seq&&, decl&);
  fn_decl& make_fn_decl(name&, type&, const decl_seq&, decl&, expr&);
  fn_decl& make_fn_decl(const char*, type&, const decl_seq&, decl&, expr&);
  fn_decl& make_fn_decl(name&, type&, decl_seq&&, decl&, expr&);
  fn_decl& make_fn_decl(const char*, type&, decl_seq&&, decl&, expr&);
  fn_decl& make_fn_decl(name&, type&, const decl_seq&, decl&, stmt&);
  fn_decl& make_fn_decl(const char*, type&, const decl_seq&, decl&, stmt&);
  fn_decl& make_fn_decl(name&, type&, decl_seq&&, decl&, stmt&);
  fn_decl& make_fn_decl(const char*, type&, decl_seq&&, decl&, stmt&);
  
  parm_decl& make_parm_decl(name&, type&);
  parm_decl& make_parm_decl(const char*, type&);

  // Statements
  block_stmt& make_block_stmt(const stmt_seq&);
  block_stmt& make_block_stmt(stmt_seq&&);
  expr_stmt& make_expr_stmt(expr&);
  decl_stmt& make_decl_stmt(decl&);
  ret_stmt& make_ret_stmt(expr&);

  canonical_set<fn_type> fn_;
};

} // namespace sys_fn
} // namespace beaker


#endif

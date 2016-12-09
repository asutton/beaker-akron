// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_CORE_CONSTRUCTION_BUILDER_HPP
#define BEAKER_CORE_CONSTRUCTION_BUILDER_HPP

#include <beaker/base/construction/builder.hpp>


namespace beaker {

struct module;

namespace core {

struct basic_name;
struct internal_name;

struct void_type;
struct ref_type;
struct fn_type;

struct nop_expr;
struct void_expr;
struct ref_expr;
struct temp_expr;
struct copy_expr;
struct call_expr;
struct deref_expr;

struct zero_init;
struct copy_init;
struct call_init;

struct var_decl;
struct ref_decl;
struct reg_decl;
struct const_decl;
struct fn_decl;

struct block_stmt;
struct expr_stmt;
struct decl_stmt;
struct ret_stmt;

/// Provides access to resources needed to construct, validate, and
/// evaluate core terms.
struct builder : builder_base
{
  static constexpr int lang = core_lang;

  builder(module&);

  // Names
  basic_name& get_name(const char*);
  internal_name& get_name();

  // Declared types
  type& get_decl_type(const decl&);
  type& get_decl_type(decl&);

  // Canonical types
  void_type& get_void_type();
  ref_type& get_ref_type(type&);
  fn_type& get_fn_type(const type_seq&, type&, int = 0);
  fn_type& get_fn_type(type_seq&&, type&, int = 0);
  fn_type& get_fn_type(const decl_seq&, decl&, int = 0);

  // General types
  ref_type& make_ref_type(type&);
  fn_type& make_fn_type(type&);

  // Expressions
  nop_expr& make_nop_expr(type&);
  void_expr& make_void_expr(type&, expr&);
  ref_expr& make_ref_expr(type&, decl&);
  temp_expr& make_temp_expr(type&);
  copy_expr& make_copy_expr(type&, expr&, expr&);
  call_expr& make_call_expr(type&, expr&, const expr_seq&);
  call_expr& make_call_expr(type&, expr&, expr_seq&&);
  deref_expr& make_deref_expr(type&, expr&);

  zero_init& make_zero_init();
  copy_init& make_copy_init(expr&);
  call_init& make_call_init(decl&, const expr_seq&);
  call_init& make_call_init(decl&, expr_seq&&);

  // Declarations
  var_decl& make_var_decl(name&, type&);
  var_decl& make_var_decl(name&, type&, expr&);
  ref_decl& make_ref_decl(name&, type&);
  ref_decl& make_ref_decl(name&, type&, expr&);
  reg_decl& make_reg_decl(name&, type&);
  reg_decl& make_reg_decl(name&, type&, expr&);
  const_decl& make_const_decl(name&, type&);
  const_decl& make_const_decl(name&, type&, expr&);
  fn_decl& make_fn_decl(name&, type&, const decl_seq&, decl&);
  fn_decl& make_fn_decl(name&, type&, decl_seq&&, decl&);
  fn_decl& make_fn_decl(name&, type&, const decl_seq&, decl&, expr&);
  fn_decl& make_fn_decl(name&, type&, decl_seq&&, decl&, expr&);
  fn_decl& make_fn_decl(name&, type&, const decl_seq&, decl&, stmt&);
  fn_decl& make_fn_decl(name&, type&, decl_seq&&, decl&, stmt&);

  // Statements
  block_stmt& make_block_stmt(const stmt_seq&);
  block_stmt& make_block_stmt(stmt_seq&&);
  expr_stmt& make_expr_stmt(expr&);
  decl_stmt& make_decl_stmt(decl&);
  ret_stmt& make_ret_stmt(expr&);

  int current_id_;
  canonical_set<basic_name> names_;
  singleton_set<void_type> void_;
  canonical_set<ref_type> refs_;
  canonical_set<fn_type> fns_;
};

/// Returns the canonical type `void`.
inline void_type& builder::get_void_type() { return void_.get(); }

} // namespace core
} // namespace beaker


#endif

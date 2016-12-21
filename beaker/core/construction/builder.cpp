// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "builder.hpp"
#include <beaker/core/name.hpp>
#include <beaker/core/type.hpp>
#include <beaker/core/expr.hpp>
#include <beaker/core/decl.hpp>
#include <beaker/core/stmt.hpp>
#include <beaker/base/module.hpp>
#include <beaker/base/comparison/eq.hpp>
#include <beaker/util/symbol_table.hpp>


namespace beaker {
namespace core {

builder::builder(module& m)
  : basic_builder<core_lang>(m), current_id_()
{ }

/// Returns the name corresponding to str.
basic_name& 
builder::get_name(const char* str) 
{
  symbol_table& syms = mod_->get_symbol_table();
  const symbol& sym = syms.insert(str, 0);
  return names_.get(sym);
}

/// Returns a unique internal name.
internal_name& 
builder::get_name() 
{
  return make<internal_name>(current_id_++);
}

type&
builder::get_decl_type(const decl& t)
{
  return get_decl_type(modifiable(t));
}

type&
builder::get_decl_type(decl& t)
{
  return t.as_typed()->get_type();
}

/// Returns the canonical type `void`.
void_type& 
builder::get_void_type()
{ 
  return void_.get(); 
}

ref_type&
builder::get_ref_type(type& t)
{
  return refs_.get(t);
}

in_type&
builder::get_in_type(type& t)
{
  return ins_.get(t);
}

out_type&
builder::get_out_type(type& t)
{
  return outs_.get(t);
}

fn_type&
builder::get_fn_type(const type_seq& p, type& r)
{
  return fns_.get(p, r);
}

fn_type&
builder::get_fn_type(type_seq&& p, type& r)
{
  return fns_.get(std::move(p), r);
}

fn_type&
builder::get_fn_type(const decl_seq& p, decl& r)
{
  type_seq parms;
  for (const decl& d : p)
    parms.push_back(get_decl_type(d));
  type& ret = get_decl_type(r);
  return get_fn_type(std::move(parms), ret);
}

/// Returns a new expression `nop`.
nop_expr&
builder::make_nop_expr(type& t)
{
  assert(is<void_type>(t));
  return make<nop_expr>(t);
}

/// Returns a new expression `void(e)`.
void_expr&
builder::make_void_expr(type& t, expr& e)
{
  assert(is<void_type>(t));
  return make<void_expr>(t, e);
}

/// Returns a new express that refers to a declaration.
ref_expr&
builder::make_ref_expr(type& t, decl& d)
{
  return make<ref_expr>(t, d);
}

/// Returns a new temporary expression.
temp_expr&
builder::make_temp_expr(type& t)
{
  assert(is_object_type(t));
  return make<temp_expr>(get_ref_type(t), t);
}

/// Returns a copy expression.
copy_expr&
builder::make_copy_expr(type& t, expr& e1, expr& e2)
{
  assert(is<ref_type>(e1.get_type()));
  assert(equivalent(get_object_type(e1.get_type()), e2.get_type()));
  assert(equivalent(e1.get_type(), t));
  return make<copy_expr>(t, e1, e2);
}

call_expr&
builder::make_call_expr(type& t, expr& f, const expr_seq& a)
{
  return make<call_expr>(t, f, a);
}

call_expr&
builder::make_call_expr(type& t, expr& f, expr_seq&& a)
{
  return make<call_expr>(t, f, std::move(a));
}

/// Returns a new expression `deref(t)`.
deref_expr&
builder::make_deref_expr(type& t, expr& e)
{
  assert(is<ref_type>(e.get_type()));
  assert(equivalent(get_object_type(e.get_type()), t));
  return make<deref_expr>(t, e);
}

/// Returns a zero initializer for the boolean type.
zero_init&
builder::make_zero_init()
{
  return make<zero_init>(get_void_type());
}

/// Returns a copy initializer for the expression e.
copy_init& 
builder::make_copy_init(expr& e)
{
  return make<copy_init>(get_void_type(), e);
}

/// Returns a call initializer for the function f and arguments a.
call_init& 
builder::make_call_init(decl& f, const expr_seq& a)
{
  return make<call_init>(get_void_type(), f, a);
}

/// Returns a call initializer for the function f and arguments a.
call_init& 
builder::make_call_init(decl& f, expr_seq&& a)
{
  return make<call_init>(get_void_type(), f, std::move(a));
}


var_decl&
builder::make_var_decl(name& n, type& t)
{
  return make<var_decl>(n, t);
}

var_decl&
builder::make_var_decl(name& n, type& t, expr& e)
{
  return make<var_decl>(n, t, e);
}

/// Returns a new function that has no definition.
fn_decl&
builder::make_fn_decl(name& n, type& t, const decl_seq& p, decl& r)
{
  return make<fn_decl>(n, t, p, r);
}

/// Returns a new function that has no definition.
fn_decl&
builder::make_fn_decl(name& n, type& t, decl_seq&& p, decl& r)
{
  return make<fn_decl>(n, t, std::move(p), r);
}

/// Returns a new function defined by an expression e.
fn_decl&
builder::make_fn_decl(name& n, type& t, const decl_seq& p, decl& r, expr& e)
{
  return make<fn_decl>(n, t, p, r, e);
}

/// Returns a new function defined by an expression e.
fn_decl&
builder::make_fn_decl(name& n, type& t, decl_seq&& p, decl& r, expr& e)
{
  return make<fn_decl>(n, t, std::move(p), r, e);
}

/// Returns a new function defined by a block statement s.
fn_decl&
builder::make_fn_decl(name& n, type& t, const decl_seq& p, decl& r, stmt& s)
{
  return make<fn_decl>(n, t, p, r, s);
}

/// Returns a new function defined by a block statement s.
fn_decl&
builder::make_fn_decl(name& n, type& t, decl_seq&& p, decl& r, stmt& s)
{
  return make<fn_decl>(n, t, std::move(p), r, s);
}

/// Returns a new parameter.
parm_decl&
builder::make_parm_decl(name& n, type& t)
{
  return make<parm_decl>(n, t);
}


/// Returns a new block statement with statements s.
block_stmt& 
builder::make_block_stmt(const stmt_seq& s)
{
  return make<block_stmt>(s);
}

/// Returns a new block statement with statements s.
block_stmt& 
builder::make_block_stmt(stmt_seq&& s)
{
  return make<block_stmt>(std::move(s));
}

/// Returns a new expression statement.
expr_stmt& 
builder::make_expr_stmt(expr& e)
{
  return make<expr_stmt>(e);
}

/// Returns a new declaration statement.
decl_stmt& 
builder::make_decl_stmt(decl& d)
{
  return make<decl_stmt>(d);
}

/// \todo Verify that `e` is either an initializer or a `nop`. For now,
/// a simple check for void expressions will help.
ret_stmt& 
builder::make_ret_stmt(expr& e)
{
  return make<ret_stmt>(e);
}

} // namespace core
} // namespace beaker


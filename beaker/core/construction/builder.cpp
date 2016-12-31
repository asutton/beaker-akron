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

/// Returns the canonical type `void`.
void_type& 
builder::get_void_type()
{ 
  return void_.get(); 
}

/// Returns the canonical type `t&`.
ref_type&
builder::get_ref_type(type& t)
{
  assert(is_object_type(t));
  return refs_.get(t);
}

// Check that no parameters have void type.
static inline bool
has_void_parm(const type_seq& ts)
{
  return std::any_of(ts.begin(), ts.end(), is_void_type);
}

/// Returns the canonical type `(p) -> r`. Note that `p` is a sequence of types.
fn_type&
builder::get_fn_type(const type_seq& p, type& r)
{
  assert(!has_void_parm(p));
  return fns_.get(p, r);
}

/// Returns the canonical type `(p) -> r`. Note that `p` is a sequence of types.
fn_type&
builder::get_fn_type(type_seq&& p, type& r)
{
  assert(!has_void_parm(p));
  return fns_.get(std::move(p), r);
}

/// Returns the canonical type `(a) -> b` where `a` is the sequence of types
/// of the parameters in `p`, and `b` is the type of the return parameter `r`.
/// This requires that each declaration in `p` is a parameter.
fn_type&
builder::get_fn_type(decl_seq& p, decl& r)
{
  type_seq parms;
  for (decl& d : p) {
    assert(is_parameter(d));
    parms.push_back(get_declared_type(d));
  }
  assert(is_parameter(r));
  type& ret = get_declared_type(r);
  return get_fn_type(std::move(parms), ret);
}


/// Returns a new expression `nop`.
nop_expr&
builder::make_nop_expr()
{
  return make<nop_expr>(get_void_type());
}

/// Returns a new expression `void(e)`.
void_expr&
builder::make_void_expr(expr& e)
{
  return make<void_expr>(get_void_type(), e);
}

/// Returns a new express that refers to a declaration.
ref_expr&
builder::make_ref_expr(decl& d)
{
  type& t = get_declared_type(d);
  if (is_object_type(t))
    // References to objects have reference types.
    return make<ref_expr>(get_ref_type(t), d);
  else if (is_reference_type(t))
    // References to references simply collapse.
    return make<ref_expr>(t, d);
  else if (is_function_type(t))
    // References to functions are simply functions.
    return make<ref_expr>(t, d);
  else
    assert(false && "reference to unknown type category");
}

/// Returns a new expression `deref(t)`.
deref_expr&
builder::make_deref_expr(expr& e)
{
  type& t = e.get_type();
  if (ref_type* ref = as<ref_type>(&t))
    // Dereferencing a reference yields an object type.
    return make<deref_expr>(ref->get_object_type(), e);
  else if (is_function_type(t))
    // Dereferencing a function is a no-op.
    return make<deref_expr>(t, e);
  else if (is_void_type(t))
    // Can't dereference voids.
    assert(false && "dereference of void expression");
  else
    assert(false && "dereference of unknown type category");
}

/// Returns a copy expression. The type of the expression is the same as the
/// type of e1.
assign_expr&
builder::make_assign_expr(expr& e1, expr& e2)
{
  assert(is_reference_expression(e1));
  assert(is_object_expression(e2));
  assert(equivalent(get_object_type(e1.get_type()), e2.get_type()));
  return make<assign_expr>(e1.get_type(), e1, e2);
}


/// FIXME: Guarantee that arguments match parameters.
call_expr&
builder::make_call_expr(expr& f, const expr_seq& a)
{
  type& t = f.get_type();
  return make<call_expr>(get_return_type(t), f, a);
}

/// FIXME: Guarantee that arguments match parameters.
call_expr&
builder::make_call_expr(expr& f, expr_seq&& a)
{
  type& t = f.get_type();
  return make<call_expr>(get_return_type(t), f, std::move(a));
}

/// Returns a trivial initializer for `t`.
nop_init&
builder::make_nop_init(type& t)
{
  return make<nop_init>(t);
}

/// Returns a zero initializer for `t`.
zero_init&
builder::make_zero_init(type& t)
{
  return make<zero_init>(t);
}

/// Returns a copy initializer for the expression `e`. This assumes that
/// the type of the initialized object is the same as the type of `e`.
copy_init& 
builder::make_copy_init(expr& e)
{
  assert(is_object_type(e.get_type()));
  return make<copy_init>(e.get_type(), e);
}

/// Returns a reference initializer for the expression `e`. This assumes that
/// the type of the initialized reference is the same as the type of `e`.
ref_init& 
builder::make_ref_init(expr& e)
{
  assert(is_reference_type(e.get_type()));
  return make<ref_init>(e.get_type(), e);
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


/// Returns a new variable `var t n`. The declaration is not defined.
///
/// FIXME: Does this mean the variable has external linkage?
var_decl&
builder::make_var_decl(name& n, type& t)
{
  return make<var_decl>(n, t);
}

/// Returns a new variable `var t n`. The declaration is not defined.
///
/// FIXME: Does this mean the variable has external linkage?
var_decl&
builder::make_var_decl(const char* n, type& t)
{
  return make_var_decl(get_name(n), t);
}

/// Returns a new variable `var t n = e`.
///
/// If e is not an initializer and we can infer which initialization would be
/// required, a proper initializer is created.
var_decl&
builder::make_var_decl(name& n, type& t, expr& e)
{
  assert(equivalent(t, e.get_type()));
  if (is_object_type(t)) {
    // Initializing an object requires copy, zero, or trivial initialization.
    // This cannot be initialized by a reference initializer. If any other 
    // expression is given, assume that copy initialization was meant.
    assert(!is<ref_init>(e));
    if (is<copy_init>(e) || is<zero_init>(e) || is<nop_init>(e))
      return make<var_decl>(n, t, e);
    else
      return make<var_decl>(n, t, make_copy_init(e));
  }
  else if(is_reference_type(t)) {
    // Initializing a reference requires reference initialization. Copy, zero,
    // and trivial initialization is not allowed for references. If any other
    // expression is given, assume that reference initialization was meant.
    assert(!is<copy_init>(e) && !is<zero_init>(e) && !is<nop_init>(e));
    if (is<ref_init>(e))
      return make<var_decl>(n, t, e);
    else
      return make<var_decl>(n, t, make_ref_init(e));
  }
  else if (is_function_type(t)) {
    assert(false && "function variable initialization not implemented");
  }
  else {
    assert(false && "variable of unknown category");
  }
}

/// Returns a new variable `var t n = e`.
var_decl&
builder::make_var_decl(const char* n, type& t, expr& e)
{
  return make_var_decl(get_name(n), t, e);
}

/// Returns a new function that has no definition.
///
/// TODO: Should this have external linkage by default?
fn_decl&
builder::make_fn_decl(name& n, type& t, const decl_seq& p, decl& r)
{
  return make<fn_decl>(n, t, p, r);
}

/// Returns a new function that has no definition.
fn_decl&
builder::make_fn_decl(const char* n, type& t, const decl_seq& p, decl& r)
{
  return make_fn_decl(get_name(n), t, p, r);
}

/// Returns a new function that has no definition.
///
/// TODO: Should this have external linkage by default?
fn_decl&
builder::make_fn_decl(name& n, type& t, decl_seq&& p, decl& r)
{
  return make<fn_decl>(n, t, std::move(p), r);
}

/// Returns a new function that has no definition.
fn_decl&
builder::make_fn_decl(const char* n, type& t, decl_seq&& p, decl& r)
{
  return make_fn_decl(get_name(n), t, std::move(p), r);
}

/// Returns a new function defined by an expression e.
fn_decl&
builder::make_fn_decl(name& n, type& t, const decl_seq& p, decl& r, expr& e)
{
  return make<fn_decl>(n, t, p, r, e);
}

/// Returns a new function defined by an expression e.
fn_decl&
builder::make_fn_decl(const char* n, type& t, const decl_seq& p, decl& r, expr& e)
{
  return make_fn_decl(get_name(n), t, p, r, e);
}

/// Returns a new function defined by an expression e.
fn_decl&
builder::make_fn_decl(name& n, type& t, decl_seq&& p, decl& r, expr& e)
{
  return make<fn_decl>(n, t, std::move(p), r, e);
}

/// Returns a new function defined by an expression e.
fn_decl&
builder::make_fn_decl(const char* n, type& t, decl_seq&& p, decl& r, expr& e)
{
  return make_fn_decl(get_name(n), t, std::move(p), r, e);
}

/// Returns a new function defined by a block statement s.
fn_decl&
builder::make_fn_decl(name& n, type& t, const decl_seq& p, decl& r, stmt& s)
{
  return make<fn_decl>(n, t, p, r, s);
}

/// Returns a new function defined by a block statement s.
fn_decl&
builder::make_fn_decl(const char* n, type& t, const decl_seq& p, decl& r, stmt& s)
{
  return make_fn_decl(get_name(n), t, p, r, s);
}

/// Returns a new function defined by a block statement s.
fn_decl&
builder::make_fn_decl(name& n, type& t, decl_seq&& p, decl& r, stmt& s)
{
  return make<fn_decl>(n, t, std::move(p), r, s);
}

/// Returns a new function defined by a block statement s.
fn_decl&
builder::make_fn_decl(const char* n, type& t, decl_seq&& p, decl& r, stmt& s)
{
  return make_fn_decl(get_name(n), t, std::move(p), r, s);
}

/// Returns a new parameter.
parm_decl&
builder::make_parm_decl(name& n, type& t)
{
  return make<parm_decl>(n, t);
}

/// Returns a new parameter `t n`.
parm_decl&
builder::make_parm_decl(const char* n, type& t)
{
  return make_parm_decl(get_name(n), t);
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

/// Returns a new statement `return e`.
///
/// This function will insert an initializer if one is needed.
///
/// Note that type of the returned expression must match the return type of 
/// the function. Failure to do so will result in undefined behavior, which
/// could happen at compile time or runtime.
ret_stmt& 
builder::make_ret_stmt(expr& e)
{
  type& t = e.get_type();
  if (is_void_type(t)) {
    // Void returns do not initialize a return value. We could insert a
    // trivial initializer, just for completion.
    return make<ret_stmt>(e);
  }
  else if (is_object_type(t)) {
    // Returning a value requires copy initialization. Reference, zero, and
    // trivial initialization are not valid in this context. If any other
    // expression is given, assume copy initialization was meant.
    assert(!is<ref_init>(e) && !is<zero_init>(e) && !is<nop_init>(e));
    if (is<copy_init>(e))
      return make<ret_stmt>(e);
    else
      return make<ret_stmt>(make_copy_init(e));
  }
  else if(is_reference_type(t)) {
    // Returning a reference requires reference initialization. Copy, zero,
    // trivial initialization are not valid in this context. If any other
    // expression is given, assume reference initialization was meant.
    assert(!is<copy_init>(e) && !is<zero_init>(e) && !is<nop_init>(e));
    if (is<ref_init>(e))
      return make<ret_stmt>(e);
    else
      return make<ret_stmt>(make_ref_init(e));
  }
  else if (is_function_type(t)) {
    assert(false && "function return types not implemented");
  }
  else {
    assert(false && "unknown return category");
  }
}

} // namespace core
} // namespace beaker


// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "builder.hpp"
#include "../type.hpp"
#include "../expr.hpp"
#include "../decl.hpp"
#include <beaker/base/module.hpp>
#include <beaker/sys.name/name.hpp>
#include <beaker/sys.name/construction/builder.hpp>


namespace beaker {
namespace sys_var {

static inline sys_name::builder& 
get_name_builder(builder& b)
{
  return b.get_module().get_builder<sys_name::builder>();
}


builder::builder(module& m)
  : basic_builder<sys_var_lang>(m),
    ref_(&get_language().make_canonical_set<ref_type>())
{ }

/// Returns the name corresponding to str.
sys_name::basic_name& 
builder::get_name(const char* str) 
{
  return get_name_builder(*this).get_name(str);
}

/// Returns a unique internal name.
sys_name::internal_name& 
builder::get_name() 
{
  return get_name_builder(*this).get_name();
}

/// Returns the canonical type `t&`.
ref_type&
builder::get_ref_type(type& t)
{
  assert(is_object_type(t));
  return ref_->get(t);
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
  else if (is_object_type(t))
    assert(false && "dereference of object type");
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

} // namespace sys_var
} // namespace beaker


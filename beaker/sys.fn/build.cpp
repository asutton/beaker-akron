// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "build.hpp"
#include "type.hpp"
#include "expr.hpp"
#include "decl.hpp"
#include "stmt.hpp"

#include <beaker/sys.void/ast.hpp>
#include <beaker/sys.bool/ast.hpp>
#include <beaker/sys.name/ast.hpp>
#include <beaker/sys.var/ast.hpp>

#include <algorithm>


namespace beaker {
namespace sys_fn {

/// Returns the module's void builder.
static inline sys_void::builder& 
get_void_builder(builder& b)
{
  return b.get_module().get_builder<sys_void::feature>();
}

/// Retrus the module's bool builder.
static inline sys_bool::builder& 
get_bool_builder(builder& b)
{
  return b.get_module().get_builder<sys_bool::feature>();
}

/// Returns the `bool` type.
static inline type&
get_bool_type(builder& b)
{
  return get_bool_builder(b).get_bool_type();
}

/// Returns the builder for names.
static inline sys_name::builder& 
get_name_builder(builder& b)
{
  return b.get_module().get_builder<sys_name::feature>();
}

/// Returns a new name with the given spelling.
static inline name&
get_name(builder& b, const char* str)
{
  return get_name_builder(b).get_name(str);
}

/// Returns a new internal name.
static inline name&
get_name(builder& b)
{
  return get_name_builder(b).get_name();
}


builder::builder(module& m)
  : factory(m),
    fn_(&make_canonical_set<fn_type>(get_language_allocator()))
{ }

// Check that no parameters have void type.
static inline bool
has_void_parm(const type_seq& ts)
{
  return std::any_of(ts.begin(), ts.end(), sys_void::is_void_type);
}

/// Returns the canonical type `(p) -> r`. Note that `p` is a sequence of types.
fn_type&
builder::get_fn_type(const type_seq& p, type& r)
{
  assert(!has_void_parm(p));
  return fn_->get(p, r);
}

/// Returns the canonical type `(p) -> r`. Note that `p` is a sequence of types.
fn_type&
builder::get_fn_type(type_seq&& p, type& r)
{
  assert(!has_void_parm(p));
  return fn_->get(std::move(p), r);
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

/// Returns the expression `e1 == e2` where e1 and e2 have function type.
eq_expr&
builder::make_eq_expr(expr& e1, expr& e2)
{
  assert(equal(e1.get_type(), e2.get_type()));
  assert(is_function_expression(e1));
  type& t = get_bool_type(*this);
  return make<eq_expr>(t, e1, e2);
}

/// Returns the expression `e1 != e2` where e1 and e2 have function type.
ne_expr&
builder::make_ne_expr(expr& e1, expr& e2)
{
  assert(equal(e1.get_type(), e2.get_type()));
  assert(is_function_expression(e1));
  type& t = get_bool_type(*this);
  return make<ne_expr>(t, e1, e2);
}


/// Returns a new function that has no definition. By default, a function with 
/// no definition has external linkage.
fn_decl&
builder::make_fn_decl(dc cxt, name& n, type& t, const decl_seq& p, decl& r)
{
  return make<fn_decl>(generate_id(), cxt, n, t, p, r);
}

/// Returns a new function that has no definition. By default, a function with 
/// no definition has external linkage.
fn_decl&
builder::make_fn_decl(dc cxt, const char* n, type& t, const decl_seq& p, decl& r)
{
  return make_fn_decl(cxt, get_name(*this, n), t, p, r);
}

/// Returns a new function that has no definition. By default, a function with 
/// no definition has external linkage.
fn_decl&
builder::make_fn_decl(dc cxt, name& n, type& t, decl_seq&& p, decl& r)
{
  return make<fn_decl>(generate_id(), cxt, n, t, std::move(p), r);
}

/// Returns a new function that has no definition. By default, a function with 
/// no definition has external linkage.
fn_decl&
builder::make_fn_decl(dc cxt, const char* n, type& t, decl_seq&& p, decl& r)
{
  return make_fn_decl(cxt, get_name(*this, n), t, std::move(p), r);
}

/// Returns a new function with external linkage and defined by a block 
/// statement.
fn_decl&
builder::make_fn_decl(dc cxt, name& n, type& t, const decl_seq& p, decl& r, stmt& s)
{
  return make<fn_decl>(generate_id(), cxt, external_link, n, t, p, r, s);
}

/// Returns a new function with external linkage and defined by a block 
/// statement.
fn_decl&
builder::make_fn_decl(dc cxt, const char* n, type& t, const decl_seq& p, decl& r, stmt& s)
{
  return make_fn_decl(cxt, get_name(*this, n), t, p, r, s);
}

/// Returns a new function with external linkage and defined by a block 
/// statement.
fn_decl&
builder::make_fn_decl(dc cxt, name& n, type& t, decl_seq&& p, decl& r, stmt& s)
{
  return make<fn_decl>(generate_id(), cxt, external_link, n, t, std::move(p), r, s);
}

/// Returns a new function with external linkage and defined by a block 
/// statement.
fn_decl&
builder::make_fn_decl(dc cxt, const char* n, type& t, decl_seq&& p, decl& r, stmt& s)
{
  return make_fn_decl(cxt, get_name(*this, n), t, std::move(p), r, s);
}

/// Returns an unnamed parameter with the given type.
parm_decl&
builder::make_parm_decl(type& t)
{
  return make_parm_decl(get_name(*this), t);
}

/// Returns a new parameter `t n`. Note that the declaration context of
/// the parameter is determined when it is added to its function. 
parm_decl&
builder::make_parm_decl(name& n, type& t)
{
  return make<parm_decl>(generate_id(), n, t);
}

/// Returns a new parameter `t n`. 
parm_decl&
builder::make_parm_decl(const char* n, type& t)
{
  return make_parm_decl(get_name(*this, n), t);
}

/// Returns a new partially constructed variable. No initializer is given.
var_decl&
builder::make_var_decl(dc cxt, name& n, type& t)
{
  return make<var_decl>(generate_id(), cxt, n, t);
}

/// Returns a new partially constructed variable. No initializer is given.
var_decl&
builder::make_var_decl(dc cxt, const char* n, type& t)
{
  return make_var_decl(cxt, get_name(*this, n), t);
}

/// Returns a new local variable of the form `var t n = e`.
///
/// If e is not an initializer and we can infer which initialization would be
/// required, a proper initializer is created.
var_decl&
builder::make_var_decl(dc cxt, name& n, type& t, expr& e)
{
  assert(equal(t, e.get_type()));
  return make<var_decl>(generate_id(), cxt, n, t, e);
  
  #if 0
  assert(equivalent(t, e.get_type()));
  if (is_object_type(t)) {
    // Initializing an object requires copy, zero, or trivial initialization.
    // This cannot be initialized by a reference initializer. If any other 
    // expression is given, assume that copy initialization was meant.
    assert(!is<ref_init>(e));
    if (is<copy_init>(e) || is<zero_init>(e) || is<nop_init>(e))
      return make<var_decl>(generate_id(), cxt, l, s, n, t, e);
    else
      return make<var_decl>(generate_id(), cxt, l, s, n, t, make_copy_init(e));
  }
  else if(is_reference_type(t)) {
    // Initializing a reference requires reference initialization. Copy, zero,
    // and trivial initialization is not allowed for references. If any other
    // expression is given, assume that reference initialization was meant.
    assert(!is<copy_init>(e) && !is<zero_init>(e) && !is<nop_init>(e));
    if (is<ref_init>(e))
      return make<var_decl>(generate_id(), cxt, l, s, n, t, e);
    else
      return make<var_decl>(generate_id(), cxt, l, s, n, t, make_ref_init(e));
  }
  else if (is_function_type(t)) {
    assert(false && "function variable initialization not implemented");
  }
  else {
    assert(false && "variable of unknown category");
  }
  #endif
}

/// Returns a new variable `var t n = e`.
var_decl&
builder::make_var_decl(dc cxt, const char* n, type& t, expr& e)
{
  return make_var_decl(cxt, get_name(*this, n), t, e);
}

/// Returns a new block statement with statements s.
block_stmt& 
builder::make_block_stmt()
{
  return make<block_stmt>(stmt_seq{});
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
  return make<ret_stmt>(e);
  
  #if 0
  auto& vb = get_var_builder(*this);
  type& t = e.get_type();
  if (sys_void::is_void_type(t)) {
    // Void returns do not initialize a return value. We could insert a
    // trivial initializer, just for completion.
    return make<ret_stmt>(e);
  }
  else if (is_object_type(t)) {
    // Returning a value requires copy initialization. Reference, zero, and
    // trivial initialization are not valid in this context. If any other
    // expression is given, assume copy initialization was meant.
    assert(!is<sys_var::ref_init>(e) && 
           !is<sys_var::zero_init>(e) && 
           !is<sys_var::nop_init>(e));
    if (is<sys_var::copy_init>(e))
      return make<ret_stmt>(e);
    else
      return make<ret_stmt>(vb.make_copy_init(e));
  }
  else if(is_reference_type(t)) {
    // Returning a reference requires reference initialization. Copy, zero,
    // trivial initialization are not valid in this context. If any other
    // expression is given, assume reference initialization was meant.
    assert(!is<sys_var::copy_init>(e) && 
           !is<sys_var::zero_init>(e) && 
           !is<sys_var::nop_init>(e));
    if (is<sys_var::ref_init>(e))
      return make<ret_stmt>(e);
    else
      return make<ret_stmt>(vb.make_ref_init(e));
  }
  else if (is_function_type(t)) {
    assert(false && "function return types not implemented");
  }
  else {
    assert(false && "unknown return category");
  }
  #endif
}

/// Returns the statement `return nop`.
ret_stmt& 
builder::make_ret_stmt()
{
  return make<ret_stmt>(get_void_builder(*this).make_nop_expr());
}

} // namespace sys_fn
} // namespace beaker


// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "build.hpp"
#include "type.hpp"
#include "expr.hpp"
#include "decl.hpp"
#include "stmt.hpp"

#include <beaker/sys.void/type.hpp>
#include <beaker/sys.bool/type.hpp>
#include <beaker/sys.bool/build.hpp>
#include <beaker/sys.name/name.hpp>
#include <beaker/sys.name/build.hpp>
#include <beaker/sys.var/expr.hpp>
#include <beaker/sys.var/build.hpp>

#include <algorithm>


namespace beaker {
namespace sys_fn {

static inline sys_bool::builder& 
get_bool_builder(builder& b)
{
  return b.get_module().get_builder<sys_bool::builder>();
}

static inline sys_name::builder& 
get_name_builder(builder& b)
{
  return b.get_module().get_builder<sys_name::builder>();
}

static inline sys_var::builder& 
get_var_builder(builder& b)
{
  return b.get_module().get_builder<sys_var::builder>();
}


builder::builder(module& m)
  : basic_builder<sys_fn_lang>(m),
    fn_(get_language().make_canonical_set<fn_type>())
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
  return fn_.get(p, r);
}

/// Returns the canonical type `(p) -> r`. Note that `p` is a sequence of types.
fn_type&
builder::get_fn_type(type_seq&& p, type& r)
{
  assert(!has_void_parm(p));
  return fn_.get(std::move(p), r);
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

eq_expr&
builder::make_eq_expr(expr& e1, expr& e2)
{
  assert(equivalent(e1.get_type(), e2.get_type()));
  assert(is<fn_type>(e1.get_type()));
  type& t = get_bool_builder(*this).get_bool_type();
  return make<eq_expr>(t, e1, e2);
}

ne_expr&
builder::make_ne_expr(expr& e1, expr& e2)
{
  assert(equivalent(e1.get_type(), e2.get_type()));
  assert(is<fn_type>(e1.get_type()));
  type& t = get_bool_builder(*this).get_bool_type();
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
  return make_fn_decl(cxt, get_name(n), t, p, r);
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
  return make_fn_decl(cxt, get_name(n), t, std::move(p), r);
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
  return make_fn_decl(cxt, get_name(n), t, p, r, s);
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
  return make_fn_decl(cxt, get_name(n), t, std::move(p), r, s);
}

/// Returns a new function with the given linkage and defined by a block 
/// statement.
fn_decl&
builder::make_fn_decl(dc cxt, linkage l, name& n, type& t, const decl_seq& p, decl& r, stmt& s)
{
  return make<fn_decl>(generate_id(), cxt, l, n, t, p, r, s);
}

/// Returns a new function with the given linkage and defined by a block 
/// statement.
fn_decl&
builder::make_fn_decl(dc cxt, linkage l, const char* n, type& t, const decl_seq& p, decl& r, stmt& s)
{
  return make_fn_decl(cxt, l, get_name(n), t, p, r, s);
}

/// Returns a new function with the given linkage and defined by a block 
/// statement.
fn_decl&
builder::make_fn_decl(dc cxt, linkage l, name& n, type& t, decl_seq&& p, decl& r, stmt& s)
{
  return make<fn_decl>(generate_id(), cxt, l, n, t, std::move(p), r, s);
}

/// Returns a new function with the given linkage and defined by a block 
/// statement.
fn_decl&
builder::make_fn_decl(dc cxt, linkage l, const char* n, type& t, decl_seq&& p, decl& r, stmt& s)
{
  return make_fn_decl(cxt, l, get_name(n), t, std::move(p), r, s);
}

/// Returns a new parameter `t n`. Note that the declaration context of
/// the parameter is determined when it is added to its function. 
parm_decl&
builder::make_parm_decl(name& n, type& t)
{
  return make<parm_decl>(generate_id(), n, t);
}

/// Returns a new parameter `t n`. Note that the declaration context of
/// the parameter is determined when it is added to its function. 
parm_decl&
builder::make_parm_decl(const char* n, type& t)
{
  return make_parm_decl(get_name(n), t);
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
}

} // namespace sys_fn
} // namespace beaker


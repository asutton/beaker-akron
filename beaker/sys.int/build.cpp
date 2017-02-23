// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "build.hpp"
#include "type.hpp"
#include "expr.hpp"

#include <beaker/sys.bool/ast.hpp>


namespace beaker {
namespace sys_int {

static inline sys_bool::builder& 
get_logic_builder(module& m)
{
  return m.get_builder<sys_bool::feature>();
}

builder::builder(module& m)
  : factory(m),
    nat_(&make_canonical_set<nat_type>(get_language_allocator())),
    int_(&make_canonical_set<int_type>(get_language_allocator())),
    mod_(&make_canonical_set<mod_type>(get_language_allocator()))
{ }

// Returns true when p is an acceptable integer type.
//
// FIXME: This isn't generally correct. We have to verify that p is one of the
// allowable integer types on the system ('int' could map to int12, for
// example).
static inline bool
check_precision(int p)
{
  switch (p) {
    case 8:
    case 16:
    case 32:
    case 64:
    case 128:
      return true;
    default:
      return false;
  } 
}

/// Returns the canonical type `natp` with `p` bits of precision.
nat_type&
builder::get_nat_type(int p) 
{
  assert(check_precision(p));
  return nat_->get(p);
}

/// Returns the canonical type `intp` with `p` bits of precision.
int_type&
builder::get_int_type(int p) 
{
  assert(check_precision(p));
  int_type& ret = int_->get(p);
  return ret;
}

/// Returns the canonical type `modp` with `p` bits of precision.
mod_type&
builder::get_mod_type(int p) 
{
  assert(check_precision(p));
  return mod_->get(p);
}

static inline bool
check_nat_value(const nat_type& t, std::uintmax_t n)
{
  return t.min() <= n && n <= t.max();
}

static inline bool
check_int_value(const int_type& t, std::intmax_t n)
{
  return t.min() <= n && n <= t.max();
}

// FIXME: If the value exceeds the representation, why don't we just apply
// the modulus to make it wrap?
static inline bool
check_mod_value(const mod_type& t, std::uintmax_t n)
{
  return t.min() <= n && n <= t.max();
}

static bool
check_value(const type& t, const value& v)
{
  switch (t.get_kind()) {
    case nat_type_kind:
      return check_nat_value(cast<nat_type>(t), v.get_int());
    case int_type_kind:
      return check_int_value(cast<int_type>(t), v.get_int());
    case mod_type_kind:
      return check_mod_value(cast<mod_type>(t), v.get_int());
  }
  assert(false && "not an integer type");
}

int_expr&
builder::make_int_expr(type& t, const value& v)
{
  assert(is_integral_type(t));
  assert(v.is_integer());
  assert(check_value(t, v));
  return make<int_expr>(t, v);
}

int_expr&
builder::make_int_expr(type& t, value&& v)
{
  assert(is_integral_type(t));
  assert(v.is_integer());
  assert(check_value(t, v));
  return make<int_expr>(t, std::move(v));
}

/// Returns the integer literal `n` with type t. The type of the expression 
/// shall be an integral type.
int_expr& 
builder::make_int_expr(type& t, int n) 
{
  assert(is_integral_type(t));
  return make_int_expr(t, value(n)); 
}

/// Returns the expression `e1 == e2`.
///
/// The operands `e1` and `e2` shall have the same type `t`, and `t` shall be 
/// an arithmetic type. The type of the expression shall be `bool`.
eq_expr&
builder::make_eq_expr(expr& e1, expr& e2)
{
  assert(equal(e1.get_type(), e2.get_type()));
  assert(is_integral_expression(e1));
  sys_bool::builder& b = get_logic_builder(get_module());
  return make<eq_expr>(b.get_bool_type(), e1, e2);
}

/// Returns the expression `e1 != e2`.
///
/// The operands `e1` and `e2` shall have the same type `t`, and `t` shall be 
/// an arithmetic type. The type of the expression shall be `bool`.
ne_expr&
builder::make_ne_expr(expr& e1, expr& e2)
{
  assert(equal(e1.get_type(), e2.get_type()));
  assert(is_integral_expression(e1));
  sys_bool::builder& b = get_logic_builder(get_module());
  return make<ne_expr>(b.get_bool_type(), e1, e2);
}

/// Returns the expression `e1 < e2`.
///
/// The operands `e1` and `e2` shall have the same type `t`, and `t` shall be 
/// an arithmetic type. The type of the expression shall be `bool`.
lt_expr&
builder::make_lt_expr(expr& e1, expr& e2)
{
  assert(equal(e1.get_type(), e2.get_type()));
  assert(is_integral_expression(e1));
  sys_bool::builder& b = get_logic_builder(get_module());
  return make<lt_expr>(b.get_bool_type(), e1, e2);
}

/// Returns the expression `e1 > e2`.
///
/// The operands `e1` and `e2` shall have the same type `t`, and `t` shall be 
/// an arithmetic type. The type of the expression shall be `bool`.
gt_expr&
builder::make_gt_expr(expr& e1, expr& e2)
{
  assert(equal(e1.get_type(), e2.get_type()));
  assert(is_integral_expression(e1));
  sys_bool::builder& b = get_logic_builder(get_module());
  return make<gt_expr>(b.get_bool_type(), e1, e2);
}

/// Returns the expression `e1 <= e2`.
///
/// The operands `e1` and `e2` shall have the same type `t`, and `t` shall be 
/// an arithmetic type. The type of the expression shall be `bool`.
le_expr&
builder::make_le_expr(expr& e1, expr& e2)
{
  assert(equal(e1.get_type(), e2.get_type()));
  assert(is_integral_expression(e1));
  sys_bool::builder& b = get_logic_builder(get_module());
  return make<le_expr>(b.get_bool_type(), e1, e2);
}

/// Returns the expression `e1 >= e2`.
///
/// The operands `e1` and `e2` shall have the same type `t`, and `t` shall be 
/// an arithmetic type. The type of the expression shall be `bool`.
ge_expr&
builder::make_ge_expr(expr& e1, expr& e2)
{
  assert(equal(e1.get_type(), e2.get_type()));
  assert(is_integral_expression(e1));
  sys_bool::builder& b = get_logic_builder(get_module());
  return make<ge_expr>(b.get_bool_type(), e1, e2);
}

add_expr&
builder::make_add_expr(expr& e1, expr& e2)
{
  assert(equal(e1.get_type(), e2.get_type()));
  assert(is_integral_expression(e1));
  return make<add_expr>(e1.get_type(), e1, e2);
}

sub_expr&
builder::make_sub_expr(expr& e1, expr& e2)
{
  assert(equal(e1.get_type(), e2.get_type()));
  assert(is_integral_expression(e1));
  return make<sub_expr>(e1.get_type(), e1, e2);
}

mul_expr&
builder::make_mul_expr(expr& e1, expr& e2)
{
  assert(equal(e1.get_type(), e2.get_type()));
  assert(is_integral_expression(e1));
  return make<mul_expr>(e1.get_type(), e1, e2);
}

quo_expr&
builder::make_quo_expr(expr& e1, expr& e2)
{
  assert(equal(e1.get_type(), e2.get_type()));
  assert(is_integral_expression(e1));
  return make<quo_expr>(e1.get_type(), e1, e2);
}

rem_expr&
builder::make_rem_expr(expr& e1, expr& e2)
{
  assert(equal(e1.get_type(), e2.get_type()));
  assert(is_integral_expression(e1));
  return make<rem_expr>(e1.get_type(), e1, e2);
}

neg_expr&
builder::make_neg_expr(expr& e)
{
  assert(is_integral_expression(e));
  return make<neg_expr>(e.get_type(), e);
}

rec_expr&
builder::make_rec_expr(expr& e)
{
  assert(is_integral_expression(e));
  return make<rec_expr>(e.get_type(), e);
}

} // namespace sys_int
} // namespace beaker

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "builder.hpp"
#include "../type.hpp"
#include "../expr.hpp"

#include <beaker/sys.bool/type.hpp>
#include <beaker/sys.bool/construction/builder.hpp>

#include <beaker/base/module.hpp>


namespace beaker {
namespace sys_float {

static inline sys_bool::builder& 
get_logic_builder(module& m)
{
  return m.get_builder<sys_bool::builder>();
}

builder::builder(module& m)
  : basic_builder<sys_float_lang>(m),
    float_(&get_language().make_canonical_set<float_type>())
{ }

/// Returns the canonical type `floatp` with `p` bits of precision.
float_type&
builder::get_float_type(int p) 
{
  return float_->get(p);
}

float_expr&
builder::make_float_expr(type& t, const value& v)
{
  assert(is_floating_point_type(t));
  assert(v.is_float());
  return make<float_expr>(t, v);
}

float_expr&
builder::make_float_expr(type& t, value&& v)
{
  assert(is_floating_point_type(t));
  assert(v.is_float());
  return make<float_expr>(t, std::move(v));
}

/// Returns the floating point literal `n` with type t. The type of the 
/// expression shall be a floating point type.
inline float_expr&
builder::make_float_expr(type& t, double n) 
{
  assert(is_floating_point_type(t));
  return make_float_expr(t, value(n));
}

eq_expr&
builder::make_eq_expr(expr& e1, expr& e2)
{
  assert(equivalent(e1.get_type(), e2.get_type()));
  assert(is_floating_point_expression(e1));
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
  assert(equivalent(e1.get_type(), e2.get_type()));
  assert(is_floating_point_expression(e1));
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
  assert(equivalent(e1.get_type(), e2.get_type()));
  assert(is_floating_point_expression(e1));
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
  assert(equivalent(e1.get_type(), e2.get_type()));
  assert(is_floating_point_expression(e1));
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
  assert(equivalent(e1.get_type(), e2.get_type()));
  assert(is_floating_point_expression(e1));
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
  assert(equivalent(e1.get_type(), e2.get_type()));
  assert(is_floating_point_expression(e1));
  sys_bool::builder& b = get_logic_builder(get_module());
  return make<ge_expr>(b.get_bool_type(), e1, e2);
}

add_expr&
builder::make_add_expr(type& t, expr& e1, expr& e2)
{
  assert(equivalent(t, e1.get_type()));
  assert(equivalent(t, e2.get_type()));
  assert(is_floating_point_type(t));
  return make<add_expr>(t, e1, e2);
}

sub_expr&
builder::make_sub_expr(type& t, expr& e1, expr& e2)
{
  assert(equivalent(t, e1.get_type()));
  assert(equivalent(t, e2.get_type()));
  assert(is_floating_point_type(t));
  return make<sub_expr>(t, e1, e2);
}

mul_expr&
builder::make_mul_expr(type& t, expr& e1, expr& e2)
{
  assert(equivalent(t, e1.get_type()));
  assert(equivalent(t, e2.get_type()));
  assert(is_floating_point_type(t));
  return make<mul_expr>(t, e1, e2);
}

div_expr&
builder::make_div_expr(type& t, expr& e1, expr& e2)
{
  assert(equivalent(t, e1.get_type()));
  assert(equivalent(t, e2.get_type()));
  assert(is_floating_point_type(t));
  return make<div_expr>(t, e1, e2);
}

neg_expr&
builder::make_neg_expr(type& t, expr& e)
{
  assert(equivalent(t, e.get_type()));
  assert(is_floating_point_type(t));
  return make<neg_expr>(t, e);
}

rec_expr&
builder::make_rec_expr(type& t, expr& e)
{
  assert(equivalent(t, e.get_type()));
  assert(is_floating_point_type(t));
  return make<rec_expr>(t, e);
}

} // namespace sys_float
} // namespace beaker

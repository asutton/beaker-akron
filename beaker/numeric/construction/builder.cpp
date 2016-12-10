// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "builder.hpp"

#include <beaker/numeric/type.hpp>
#include <beaker/numeric/expr.hpp>
#include <beaker/logic/type.hpp>


namespace beaker {
namespace numeric {

builder::builder(module& m)
  : basic_builder<numeric_lang>(m)
{ }

/// Returns the canonical type `charp` with `p` bits of precision.
char_type&
builder::get_char_type(int p) 
{
  return chars_.get(p);
}

/// Returns the canonical type `natp` with `p` bits of precision.
nat_type&
builder::get_nat_type(int p) 
{
  return nats_.get(p);
}

/// Returns the canonical type `intp` with `p` bits of precision.
int_type&
builder::get_int_type(int p) 
{
  return ints_.get(p);
}

/// Returns the canonical type `modp` with `p` bits of precision.
mod_type&
builder::get_mod_type(int p) 
{
  return mods_.get(p);
}

/// Returns the canonical type `floatp` with `p` bits of precision.
float_type&
builder::get_float_type(int p) 
{
  return floats_.get(p);
}

/// Returns the expression `e1 == e2`.
///
/// The operands `e1` and `e2` shall have the same type `t`, and `t` shall be 
/// an arithmetic type. The type of the expression shall be `bool`.
eq_expr&
builder::make_eq_expr(type& t, expr& e1, expr& e2)
{
  assert(equivalent(e1.get_type(), e2.get_type()));
  assert(is_arithmetic_expression(e1));
  assert(logic::is_boolean_type(t));
  return make<eq_expr>(t, e1, e2);
}

/// Returns the expression `e1 != e2`.
///
/// The operands `e1` and `e2` shall have the same type `t`, and `t` shall be 
/// an arithmetic type. The type of the expression shall be `bool`.
ne_expr&
builder::make_ne_expr(type& t, expr& e1, expr& e2)
{
  assert(equivalent(e1.get_type(), e2.get_type()));
  assert(is_arithmetic_expression(e1));
  assert(logic::is_boolean_type(t));
  return make<ne_expr>(t, e1, e2);
}

/// Returns the expression `e1 < e2`.
///
/// The operands `e1` and `e2` shall have the same type `t`, and `t` shall be 
/// an arithmetic type. The type of the expression shall be `bool`.
lt_expr&
builder::make_lt_expr(type& t, expr& e1, expr& e2)
{
  assert(equivalent(e1.get_type(), e2.get_type()));
  assert(is_arithmetic_expression(e1));
  assert(logic::is_boolean_type(t));
  return make<lt_expr>(t, e1, e2);
}

/// Returns the expression `e1 > e2`.
///
/// The operands `e1` and `e2` shall have the same type `t`, and `t` shall be 
/// an arithmetic type. The type of the expression shall be `bool`.
gt_expr&
builder::make_gt_expr(type& t, expr& e1, expr& e2)
{
  assert(equivalent(e1.get_type(), e2.get_type()));
  assert(is_arithmetic_expression(e1));
  assert(logic::is_boolean_type(t));
  return make<gt_expr>(t, e1, e2);
}

/// Returns the expression `e1 <= e2`.
///
/// The operands `e1` and `e2` shall have the same type `t`, and `t` shall be 
/// an arithmetic type. The type of the expression shall be `bool`.
le_expr&
builder::make_le_expr(type& t, expr& e1, expr& e2)
{
  assert(equivalent(e1.get_type(), e2.get_type()));
  assert(is_arithmetic_expression(e1));
  assert(logic::is_boolean_type(t));
  return make<le_expr>(t, e1, e2);
}

/// Returns the expression `e1 >= e2`.
///
/// The operands `e1` and `e2` shall have the same type `t`, and `t` shall be 
/// an arithmetic type. The type of the expression shall be `bool`.
ge_expr&
builder::make_ge_expr(type& t, expr& e1, expr& e2)
{
  assert(equivalent(e1.get_type(), e2.get_type()));
  assert(is_arithmetic_expression(e1));
  assert(logic::is_boolean_type(t));
  return make<ge_expr>(t, e1, e2);
}

int_expr&
builder::make_int_expr(type& t, const value& v)
{
  assert(is_integral_type(t));
  assert(v.is_integer());
  return make<int_expr>(t, v);
}

int_expr&
builder::make_int_expr(type& t, value&& v)
{
  assert(is_integral_type(t));
  assert(v.is_integer());
  return make<int_expr>(t, std::move(v));
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

add_expr&
builder::make_add_expr(type& t, expr& e1, expr& e2)
{
  assert(equivalent(t, e1.get_type()));
  assert(equivalent(t, e2.get_type()));
  assert(is_arithmetic_type(t));
  return make<add_expr>(t, e1, e2);
}

sub_expr&
builder::make_sub_expr(type& t, expr& e1, expr& e2)
{
  assert(equivalent(t, e1.get_type()));
  assert(equivalent(t, e2.get_type()));
  assert(is_arithmetic_type(t));
  return make<sub_expr>(t, e1, e2);
}

mul_expr&
builder::make_mul_expr(type& t, expr& e1, expr& e2)
{
  assert(equivalent(t, e1.get_type()));
  assert(equivalent(t, e2.get_type()));
  assert(is_arithmetic_type(t));
  return make<mul_expr>(t, e1, e2);
}

div_expr&
builder::make_div_expr(type& t, expr& e1, expr& e2)
{
  assert(equivalent(t, e1.get_type()));
  assert(equivalent(t, e2.get_type()));
  assert(is_arithmetic_type(t));
  return make<div_expr>(t, e1, e2);
}

rem_expr&
builder::make_rem_expr(type& t, expr& e1, expr& e2)
{
  assert(equivalent(t, e1.get_type()));
  assert(equivalent(t, e2.get_type()));
  assert(is_arithmetic_type(t));
  return make<rem_expr>(t, e1, e2);
}

neg_expr&
builder::make_neg_expr(type& t, expr& e)
{
  assert(equivalent(t, e.get_type()));
  assert(is_arithmetic_type(t));
  return make<neg_expr>(t, e);
}

rec_expr&
builder::make_rec_expr(type& t, expr& e)
{
  assert(equivalent(t, e.get_type()));
  assert(is_arithmetic_type(t));
  return make<rec_expr>(t, e);
}

} // namespace numeric
} // namespace beaker

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "semantics.hpp"

#include <beaker/base/printing/print.hpp>

#include <iostream>
#include <stdexcept>


namespace icalc {

/// Returns true if `e` has type `bool`.
static inline bool
is_boolean(const expr& e)
{
  return beaker::sys_bool::is_boolean_expression(e);
}

/// Returns true if both `e1` and `e2` have type `bool`.
static inline bool
are_boolean(const expr& e1, const expr& e2)
{
  return is_boolean(e1) && is_boolean(e2);
}

/// Returns true if `e` has type `int`.
static inline bool
is_integral(const expr& e)
{
  return beaker::sys_int::is_integral_expression(e);
}

/// Returns true if both `e1` and `e2` have type `int`.
static inline bool
are_integral(const expr& e1, const expr& e2)
{
  return is_integral(e1) && is_integral(e2);
}

/// Returns true if both `e1` and `e2` have the same type.
static inline bool
same_typed(const expr& e1, const expr& e2)
{
  return equal(e1.get_type(), e2.get_type());
}

/// Check that `e1` has type bool.
void
semantics::check_bool(expr& e1)
{
  if (!is_boolean(e1))
    throw type_error(e1.get_location(), "operand does not have type 'bool'");
}

/// Check that `e1` and `e2` both have type bool.
void
semantics::check_bool(expr& e1, expr& e2)
{
  check_bool(e1);
  check_bool(e2);
}

/// Check that `e1` has type int.
void
semantics::check_int(expr& e1)
{
  if (!is_integral(e1))
    throw type_error(e1.get_location(), "operand does not have type 'int'");
}

/// Check that `e1` and `e2` both have type int.
void
semantics::check_int(expr& e1, expr& e2)
{
  check_int(e1);
  check_int(e2);
}

/// Check that `e1` and `e2` have the same type.
void
semantics::check_same(expr& e1, expr& e2)
{
  if (!same_typed(e1, e2))
    throw type_error(e1.get_location(), "operands have different types");
}

expr&
semantics::on_condition(expr& e1, token q, expr& e2, token c, expr& e3)
{
  check_bool(e1);
  check_same(e2, e3);
  expr& ret = build.make_if_expr(e1, e2, e3);
  ret.set_location(e1.get_location());
  return ret;
}

expr&
semantics::on_logical_or(expr& e1, token tok, expr& e2)
{
  check_bool(e1, e2);
  expr& ret = build.make_or_else_expr(e1, e2);
  ret.set_location(e1.get_location());
  return ret;
}

expr&
semantics::on_logical_and(expr& e1, token tok, expr& e2)
{
  check_bool(e1, e2);
  expr& ret = build.make_and_then_expr(e1, e2);
  ret.set_location(e1.get_location());
  return ret;
}

// FIXME: Provide better diagnostics.
expr&
semantics::on_bitwise_or(expr& e1, token tok, expr& e2)
{
  if (are_boolean(e1, e2)) {
    beaker::sys_bool::builder& bb = build;
    return bb.make_or_expr(e1, e2);
  }
  else if (are_integral(e1, e2)) {
    throw type_error(location(), "bitwise-or not implemented");
  }
  throw type_error(e1.get_location(), "operands have different types");
}

// FIXME: Provide better diagnostics.
expr&
semantics::on_bitwise_xor(expr& e1, token tok, expr& e2)
{
  if (are_boolean(e1, e2)) {
    beaker::sys_bool::builder& bb = build;
    return bb.make_xor_expr(e1, e2);
  }
  else if (are_integral(e1, e2)) {
    throw type_error(location(), "bitwise-xor not implemented");
  }
  throw type_error(e1.get_location(), "operands have different types");
}

// FIXME: Provide better diagnostics.
expr&
semantics::on_bitwise_and(expr& e1, token tok, expr& e2)
{
  if (are_boolean(e1, e2)) {
    beaker::sys_bool::builder& bb = build;
    return bb.make_and_expr(e1, e2);
  }
  else if (are_integral(e1, e2)) {
    throw type_error(location(), "bitwise-and not implemented");
  }
  throw type_error(e1.get_location(), "operands have different types");
}

expr&
semantics::on_equal(expr& e1, token tok, expr& e2)
{
  if (are_boolean(e1, e2)) {
    beaker::sys_bool::builder& bb = build;
    return bb.make_eq_expr(e1, e2);
  }
  else if (are_integral(e1, e2)) {
    beaker::sys_int::builder& ib = build;
    return ib.make_eq_expr(e1, e2);
  }
  throw type_error(e1.get_location(), "operands have different types");
}

expr&
semantics::on_not_equal(expr& e1, token tok, expr& e2)
{
  if (are_boolean(e1, e2)) {
    beaker::sys_bool::builder& bb = build;
    return bb.make_xor_expr(e1, e2);
  }
  else if (are_integral(e1, e2)) {
    beaker::sys_int::builder& ib = build;
    return ib.make_ne_expr(e1, e2);
  }
  throw type_error(e1.get_location(), "operands have different types");
}

expr&
semantics::on_less(expr& e1, token tok, expr& e2)
{
  check_int(e1, e2);
  return build.make_lt_expr(e1, e2);
}

expr&
semantics::on_greater(expr& e1, token tok, expr& e2)
{
  check_int(e1, e2);
  return build.make_gt_expr(e1, e2);
}

expr&
semantics::on_less_equal(expr& e1, token tok, expr& e2)
{
  check_int(e1, e2);
  return build.make_le_expr(e1, e2);
}

expr&
semantics::on_greater_equal(expr& e1, token tok, expr& e2)
{
  check_int(e1, e2);
  return build.make_ge_expr(e1, e2);
}

expr&
semantics::on_addition(expr& e1, token tok, expr& e2)
{
  check_int(e1, e2);
  return build.make_add_expr(e1, e2);
}

expr&
semantics::on_subtraction(expr& e1, token tok, expr& e2)
{
  check_int(e1, e2);
  return build.make_sub_expr(e1, e2);
}

expr&
semantics::on_multiplication(expr& e1, token tok, expr& e2)
{
  check_int(e1, e2);
  return build.make_mul_expr(e1, e2);
}

expr&
semantics::on_division(expr& e1, token tok, expr& e2)
{
  check_int(e1, e2);
  return build.make_quo_expr(e1, e2);
}

expr&
semantics::on_remainder(expr& e1, token tok, expr& e2)
{
  check_int(e1, e2);
  return build.make_rem_expr(e1, e2);
}

expr&
semantics::on_negation(token tok, expr& e1)
{
  check_int(e1);
  return build.make_neg_expr(e1);
}

expr&
semantics::on_bitwise_not(token tok, expr& e1)
{
  throw type_error(tok.get_location(), "bitwise-not not implemented");
}

expr&
semantics::on_logical_not(token tok, expr& e1)
{
  check_bool(e1);
  return build.make_not_expr(e1);
}

/// Returns a new boolean expression.
expr&
semantics::on_bool(token tok)
{
  const beaker::bool_attr& attr = get_bool_attribute(tok);
  expr* ret;
  if (attr.get_value())
    ret = &build.make_true_expr();
  else
    ret = &build.make_false_expr();
  ret->set_location(tok.get_location());
  return *ret;
}

expr&
semantics::on_int(token tok)
{
  const beaker::int_attr& attr = get_int_attribute(tok);
  expr& ret = build.make_int_expr(build.get_int32_type(), attr.get_value());
  ret.set_location(tok.get_location());
  return ret;
}

} // namespace icalc

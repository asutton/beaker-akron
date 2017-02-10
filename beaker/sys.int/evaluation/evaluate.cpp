// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "evaluate.hpp"
#include "../type.hpp"
#include "../expr.hpp"
#include <beaker/base/printing/print.hpp>


namespace beaker {
namespace sys_int {

/// Evaluate the literal expression `e`.
value
evaluate_int_expr(evaluator& eval, const int_expr& e)
{
  return e.get_value();
}

value
evaluate_eq_expr(evaluator& eval, const eq_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() == v2.get_int());
}

value
evaluate_ne_expr(evaluator& eval, const ne_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() != v2.get_int());
}

value
evaluate_lt_expr(evaluator& eval, const lt_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() < v2.get_int());
}

value
evaluate_gt_expr(evaluator& eval, const gt_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() > v2.get_int());
}

value
evaluate_le_expr(evaluator& eval, const le_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() <= v2.get_int());
}

value
evaluate_ge_expr(evaluator& eval, const ge_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() >= v2.get_int());
}


template<typename T>
static value
add_int(evaluator& eval, const add_expr& e, const T& t)
{
  std::uintmax_t a = evaluate(eval, e.get_lhs()).get_int();
  std::uintmax_t b = evaluate(eval, e.get_rhs()).get_int();
  if (a > t.max() - b)
    throw overflow_error(e);
  return value(a + b);
}

static value
add_mod(evaluator& eval, const add_expr& e, const mod_type& t)
{
  std::uintmax_t a = evaluate(eval, e.get_lhs()).get_int();
  std::uintmax_t b = evaluate(eval, e.get_rhs()).get_int();
  return value((a + b) % t.mod());
}

value
evaluate_add_expr(evaluator& eval, const add_expr& e)
{
  const type& t = e.get_type();
  switch (t.get_kind()) {
    case nat_type_kind:
      return add_int(eval, e, cast<nat_type>(t));
    case int_type_kind:
      return add_int(eval, e, cast<int_type>(t));
    case mod_type_kind:
      return add_mod(eval, e, cast<mod_type>(t));
  }
  assert(false && "not an integer expression");
}

template<typename T>
static value
sub_int(evaluator& eval, const sub_expr& e, const T& t)
{
  std::uintmax_t a = evaluate(eval, e.get_lhs()).get_int();
  std::uintmax_t b = evaluate(eval, e.get_rhs()).get_int();
  if (a < t.min() + b)
    throw overflow_error(e);
  return value(a - b);
}

static value
sub_mod(evaluator& eval, const sub_expr& e, const mod_type& t)
{
  std::uintmax_t a = evaluate(eval, e.get_lhs()).get_int();
  std::uintmax_t b = evaluate(eval, e.get_rhs()).get_int();
  return value((a - b) % t.mod());
}

value
evaluate_sub_expr(evaluator& eval, const sub_expr& e)
{
  const type& t = e.get_type();
  switch (t.get_kind()) {
    case nat_type_kind:
      return sub_int(eval, e, cast<nat_type>(t));
    case int_type_kind:
      return sub_int(eval, e, cast<int_type>(t));
    case mod_type_kind:
      return sub_mod(eval, e, cast<mod_type>(t));
  }
  assert(false && "not an integer expression");
}

// FIXME: Handle overflow.
value
evaluate_mul_expr(evaluator& eval, const mul_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() * v2.get_int());
}

// FIXME: Handle overflow.
//
// FIXME: Handle division by 0.
value
evaluate_div_expr(evaluator& eval, const div_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() / v2.get_int());
}

// FIXME: Handle overflow.
//
// FIXME: Handle division by 0.
value
evaluate_rem_expr(evaluator& eval, const rem_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() % v2.get_int());
}

value
evaluate_neg_expr(evaluator& eval, const neg_expr& e)
{
  value v = evaluate(eval, e.get_operand());
  return value(-v.get_int());
}

// FIXME: Handle overflow.
//
// FIXME: Handle division by 0.
value
evaluate_rec_expr(evaluator& eval, const rec_expr& e)
{
  value v = evaluate(eval, e.get_operand());
  return value(1 / v.get_int());
}

} // namespace sys_int
} // namespace beaker

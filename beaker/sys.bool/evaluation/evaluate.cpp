// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "evaluate.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_bool {

// Evaluate the expressions `true` and `false`.
value
evaluate_bool_expr(evaluator& eval, const bool_expr& e)
{
  return e.get_value();  
}

// Evaluate the expression `e1 & e2`.
value
evaluate_and_expr(evaluator& eval, const and_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() & v2.get_int());
}

// Evaluate the expression `e1 | e2`.
value
evaluate_or_expr(evaluator& eval, const or_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() | v2.get_int());
}

/// Evaluates the expression `e1 ^ e2`.
value
evaluate_xor_expr(evaluator& eval, const xor_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() ^ v2.get_int());
}

// Evaluate the expression `!e`.
value
evaluate_not_expr(evaluator& eval, const not_expr& e)
{
  value v = evaluate(eval, e.get_operand());
  return value(!v.get_int());
}

// Evaluate the expression `e1 => e2`.
value
evaluate_imp_expr(evaluator& eval, const imp_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value((!v1.get_int()) | v2.get_int());
}

// Evaluate the expression `e1 <=> e2`.
value
evaluate_eq_expr(evaluator& eval, const eq_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() == v2.get_int());
}

value
evaluate_if_expr(evaluator& eval, const if_expr& e)
{
  value p = evaluate(eval, e.get_condition());
  if (p.get_int())
    return evaluate(eval, e.get_true_value());
  else
    return evaluate(eval, e.get_false_value());
}

value
evaluate_and_then_expr(evaluator& eval, const and_then_expr& e)
{
  value v = evaluate(eval, e.get_lhs());
  if (v.get_int())
    return evaluate(eval, e.get_rhs());
  else
    return value(0); // false
}

value
evaluate_or_else_expr(evaluator& eval, const or_else_expr& e)
{
  value v = evaluate(eval, e.get_lhs());
  if (v.get_int())
    return value(1); // true
  else
    return evaluate(eval, e.get_rhs());
}

value
evaluate_assert_expr(evaluator& eval, const assert_expr& e)
{
  value v = evaluate(eval, e.get_operand());
  if (v.get_int())
    return value(1);
  else
    throw assertion_error(e);
}

} // namespace sys_bool
} // namespace beaker

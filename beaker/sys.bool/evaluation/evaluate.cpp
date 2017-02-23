// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "evaluate.hpp"
#include "../expr.hpp"


namespace beaker {

// Evaluate the expressions `true` and `false`.
value
evaluate_expr(evaluator& eval, const sys_bool::bool_expr& e)
{
  return e.get_value();  
}

// Evaluate the expression `e1 & e2`.
value
evaluate_expr(evaluator& eval, const sys_bool::and_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() & v2.get_int());
}

// Evaluate the expression `e1 | e2`.
value
evaluate_expr(evaluator& eval, const sys_bool::or_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() | v2.get_int());
}

/// Evaluates the expression `e1 ^ e2`.
value
evaluate_expr(evaluator& eval, const sys_bool::xor_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() ^ v2.get_int());
}

// Evaluate the expression `!e`.
value
evaluate_expr(evaluator& eval, const sys_bool::not_expr& e)
{
  value v = evaluate(eval, e.get_operand());
  return value(!v.get_int());
}

// Evaluate the expression `e1 => e2`.
value
evaluate_expr(evaluator& eval, const sys_bool::imp_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value((!v1.get_int()) | v2.get_int());
}

// Evaluate the expression `e1 <=> e2`.
value
evaluate_expr(evaluator& eval, const sys_bool::eq_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() == v2.get_int());
}

value
evaluate_expr(evaluator& eval, const sys_bool::if_expr& e)
{
  value p = evaluate(eval, e.get_condition());
  if (p.get_int())
    return evaluate(eval, e.get_true_value());
  else
    return evaluate(eval, e.get_false_value());
}

value
evaluate_expr(evaluator& eval, const sys_bool::and_then_expr& e)
{
  value v = evaluate(eval, e.get_lhs());
  if (v.get_int())
    return evaluate(eval, e.get_rhs());
  else
    return value(0); // false
}

value
evaluate_expr(evaluator& eval, const sys_bool::or_else_expr& e)
{
  value v = evaluate(eval, e.get_lhs());
  if (v.get_int())
    return value(1); // true
  else
    return evaluate(eval, e.get_rhs());
}

value
evaluate_expr(evaluator& eval, const sys_bool::assert_expr& e)
{
  value v = evaluate(eval, e.get_operand());
  if (v.get_int())
    return value(1);
  else
    throw sys_bool::assertion_error(e);
}

} // namespace beaker

// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "evaluation.hpp"
#include "expression.hpp"


namespace beaker {
namespace logic {

// Evaluate the expressions `true` and `false`.
static inline value
evaluate_expr(evaluator& eval, const bool_expr& e)
{
  return e.get_value();  
}

// Evaluate the expression `e1 && e2`.
static value
evaluate_expr(evaluator& eval, const and_expr& e)
{
  value v = evaluate(eval, e.get_left());
  if (!v.get_integer()) // v == 0
    return value(0);    // short circuit to false
  else
    return evaluate(eval, e.get_right());
}

// Evaluate the expression `e1 || e2`.
static value
evaluate_expr(evaluator& eval, const or_expr& e)
{
  value v = evaluate(eval, e.get_left());
  if (v.get_integer()) // v != 0
    return value(1);   // short circuit to true
  else
    return evaluate(eval, e.get_right());
}

// Evaluate the expression `!e`.
static value
evaluate_expr(evaluator& eval, const not_expr& e)
{
  value v = evaluate(eval, e.get_operand());
  if (v.get_integer()) // v != 0
    return value(0);
  else                 // v == 0
    return value(1);
}

// Evaluate the expression `e1 => e2`.
static value
evaluate_expr(evaluator& eval, const imp_expr& e)
{
  value v = evaluate(eval,e.get_left());
  if (!v.get_integer()) // v == 0
    return value(1);    // short circuit true
  else
    return evaluate(eval, e.get_right());
}

// Evaluate the expression `e1 <=> e2`.
static value
evaluate_expr(evaluator& eval, const eq_expr& e)
{
  value v1 = evaluate(eval, e.get_left());
  value v2 = evaluate(eval, e.get_left());
  return value(v1.get_integer() == v2.get_integer());
}

// Evaluate the expression e.
value
evaluate_expr(evaluator& eval, const expr& e) 
{
  switch (e.get_kind()) {
    case bool_expr_kind:
      return evaluate_expr(eval, cast<bool_expr>(e));
    case and_expr_kind:
      return evaluate_expr(eval, cast<and_expr>(e));
    case or_expr_kind: 
      return evaluate_expr(eval, cast<or_expr>(e));
    case not_expr_kind:
      return evaluate_expr(eval, cast<not_expr>(e));
    case imp_expr_kind:
      return evaluate_expr(eval, cast<imp_expr>(e));
    case eq_expr_kind: 
      return evaluate_expr(eval, cast<eq_expr>(e));
    default:
      break;
  }
  assert(false && "not a logic expression");
}

} // namespace logic
} // namespace beaker

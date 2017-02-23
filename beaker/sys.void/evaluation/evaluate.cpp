// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "evaluate.hpp"
#include "../expr.hpp"


namespace beaker {

/// Returns the void value.
value
evaluate_expr(evaluator& eval, const sys_void::nop_expr& e)
{
  return {};
}

/// Evaluates the operand and returns the void value.
value
evaluate_expr(evaluator& eval, const sys_void::void_expr& e)
{
  evaluate(eval, e.get_operand());
  return {};
}

/// Evaluation fails with a trap exception.
value
evaluate_expr(evaluator& eval, const sys_void::trap_expr& e)
{
  throw sys_void::trap_error(e);
}


} // namespace beaker

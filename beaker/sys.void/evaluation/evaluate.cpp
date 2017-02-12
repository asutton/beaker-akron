// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "evaluate.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_void {

/// Returns the void value.
value
evaluate(evaluator& eval, const nop_expr& e)
{
  return {};
}

/// Evaluates the operand and returns the void value.
value
evaluate(evaluator& eval, const void_expr& e)
{
  evaluate(eval, e.get_operand());
  return {};
}

/// Evaluation fails with a trap exception.
value
evaluate(evaluator& eval, const trap_expr& e)
{
  throw trap_error(e);
}


} // namespace sys_void
} // namespace beaker

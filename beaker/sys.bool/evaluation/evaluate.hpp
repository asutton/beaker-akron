// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_BOOL_EVALUATION_EVALUATE_HPP
#define BEAKER_SYS_BOOL_EVALUATION_EVALUATE_HPP

#include <beaker/sys.bool/fwd.hpp>

#include <beaker/base/evaluation/evaluate.hpp>

namespace beaker {
namespace sys_bool {

// -------------------------------------------------------------------------- //
// Errors

/// A trap error occures when the operand of an assertion is false.
struct assertion_error : evaluation_error
{
  assertion_error(const expr&);

  const expr* expr_;
};

inline 
assertion_error::assertion_error(const expr& e)
  : evaluation_error("assert"), expr_(&e) 
{ }


// -------------------------------------------------------------------------- //
// Overrides

value evaluate_bool_expr(evaluator&, const bool_expr&);
value evaluate_and_expr(evaluator&, const and_expr&);
value evaluate_or_expr(evaluator&, const or_expr&);
value evaluate_xor_expr(evaluator&, const xor_expr&);
value evaluate_not_expr(evaluator&, const not_expr&);
value evaluate_imp_expr(evaluator&, const imp_expr&);
value evaluate_eq_expr(evaluator&, const eq_expr&);
value evaluate_if_expr(evaluator&, const if_expr&);
value evaluate_and_then_expr(evaluator&, const and_then_expr&);
value evaluate_or_else_expr(evaluator&, const or_else_expr&);
value evaluate_assert_expr(evaluator&, const assert_expr&);

} // namespace sys_bool
} // namespace beaker


#endif

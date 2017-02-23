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

/// An assertion error occures when the operand of an assertion is false.
struct assertion_error : evaluation_error
{
  assertion_error(const expr&);

  const expr* expr_;
};

inline 
assertion_error::assertion_error(const expr& e)
  : evaluation_error("assert"), expr_(&e) 
{ }

} // namespace sys_bool


// -------------------------------------------------------------------------- //
// Overrides

value evaluate_expr(evaluator&, const sys_bool::bool_expr&);
value evaluate_expr(evaluator&, const sys_bool::and_expr&);
value evaluate_expr(evaluator&, const sys_bool::or_expr&);
value evaluate_expr(evaluator&, const sys_bool::xor_expr&);
value evaluate_expr(evaluator&, const sys_bool::not_expr&);
value evaluate_expr(evaluator&, const sys_bool::imp_expr&);
value evaluate_expr(evaluator&, const sys_bool::eq_expr&);
value evaluate_expr(evaluator&, const sys_bool::if_expr&);
value evaluate_expr(evaluator&, const sys_bool::and_then_expr&);
value evaluate_expr(evaluator&, const sys_bool::or_else_expr&);
value evaluate_expr(evaluator&, const sys_bool::assert_expr&);

} // namespace beaker


#endif

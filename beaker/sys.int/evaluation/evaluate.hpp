// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_INT_EVALUATION_EVALUATE_HPP
#define BEAKER_SYS_INT_EVALUATION_EVALUATE_HPP

#include <beaker/sys.int/fwd.hpp>

#include <beaker/base/evaluation/evaluate.hpp>


namespace beaker {
namespace sys_int {

// -------------------------------------------------------------------------- //
// Errors

/// An overflow error occurs when the result of an arithmetic operation cannot
/// be represented by the type.
struct overflow_error : evaluation_error
{
  overflow_error(const expr&);

  const expr* expr_;
};

inline 
overflow_error::overflow_error(const expr& e)
  : evaluation_error("overflow"), expr_(&e) 
{ }


/// A division error occurs when dividing by 0.
struct division_error : evaluation_error
{
  division_error(const expr&);

  const expr* expr_;
};

inline 
division_error::division_error(const expr& e)
  : evaluation_error("division by zero"), expr_(&e) 
{ }

} // namespace sys_int


// -------------------------------------------------------------------------- //
// Overrides

value evaluate_expr(evaluator&, const sys_int::int_expr&);
value evaluate_expr(evaluator&, const sys_int::eq_expr&);
value evaluate_expr(evaluator&, const sys_int::ne_expr&);
value evaluate_expr(evaluator&, const sys_int::lt_expr&);
value evaluate_expr(evaluator&, const sys_int::gt_expr&);
value evaluate_expr(evaluator&, const sys_int::le_expr&);
value evaluate_expr(evaluator&, const sys_int::ge_expr&);
value evaluate_expr(evaluator&, const sys_int::add_expr&);
value evaluate_expr(evaluator&, const sys_int::sub_expr&);
value evaluate_expr(evaluator&, const sys_int::mul_expr&);
value evaluate_expr(evaluator&, const sys_int::quo_expr&);
value evaluate_expr(evaluator&, const sys_int::rem_expr&);
value evaluate_expr(evaluator&, const sys_int::neg_expr&);
value evaluate_expr(evaluator&, const sys_int::rec_expr&);

} // namespace beaker


#endif

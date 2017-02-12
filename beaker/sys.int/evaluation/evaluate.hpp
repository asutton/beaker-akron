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


// -------------------------------------------------------------------------- //
// Overrides

value evaluate(evaluator&, const int_expr&);
value evaluate(evaluator&, const eq_expr&);
value evaluate(evaluator&, const ne_expr&);
value evaluate(evaluator&, const lt_expr&);
value evaluate(evaluator&, const gt_expr&);
value evaluate(evaluator&, const le_expr&);
value evaluate(evaluator&, const ge_expr&);
value evaluate(evaluator&, const add_expr&);
value evaluate(evaluator&, const sub_expr&);
value evaluate(evaluator&, const mul_expr&);
value evaluate(evaluator&, const quo_expr&);
value evaluate(evaluator&, const rem_expr&);
value evaluate(evaluator&, const neg_expr&);
value evaluate(evaluator&, const rec_expr&);

} // namespace sys_int
} // namespace beaker


#endif

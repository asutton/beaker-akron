// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VOID_EVALUATION_EVALUATE_HPP
#define BEAKER_SYS_VOID_EVALUATION_EVALUATE_HPP

#include <beaker/sys.void/fwd.hpp>
#include <beaker/base/evaluation/evaluate.hpp>


namespace beaker {
namespace sys_void {

// -------------------------------------------------------------------------- //
// Errors

/// A trap error occures when the trap expression is evaluated.
struct trap_error : evaluation_error
{
  trap_error(const expr&);

  const expr* expr_;
};

inline 
trap_error::trap_error(const expr& e)
  : evaluation_error("trap"), expr_(&e) 
{ }


// -------------------------------------------------------------------------- //
// Overrides

value evaluate(evaluator&, const nop_expr&);
value evaluate(evaluator&, const void_expr&);
value evaluate(evaluator&, const trap_expr&);

} // namespace sys_void
} // namespace beaker

#endif

// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "evaluation.hpp"
#include "expression.hpp"

#include <beaker/terms/logic/evaluation.hpp>

namespace beaker {

value 
evaluate(evaluator& eval, const expr& e)
{
  switch (e.get_pack()) {
    case logic_lang:
      return logic::evaluate_expr(eval, e);
    case numeric_lang:
      // return numeric::evaluate_expr(eval, e);
    case functional_lang:
      // return funtional::evaluate_expr(eval, e);
    default:
      break;
  }
  assert(false && "expression not supported");
}

} // namespace beaker


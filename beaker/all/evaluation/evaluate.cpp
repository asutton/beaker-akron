// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "evaluate.hpp"
#include "../ast.hpp"


namespace beaker {

/// \todo Should _init functions get special treatment?
value
evaluate(evaluator& eval, const expr& e)
{
  switch (e.get_kind()) {
#define def_expr(NS, E) \
    case NS::E ## _expr_kind: \
      return evaluate_expr(eval, cast<NS::E ## _expr>(e));
#define def_init(NS, E) \
    case NS::E ## _init_kind: \
      return evaluate_expr(eval, cast<NS::E ## _init>(e));
#include <beaker/all/expr.def>
  }
  assert(false && "invalid expression");
}


} // namespace beaker
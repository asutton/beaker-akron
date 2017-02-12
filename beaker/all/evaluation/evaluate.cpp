// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include <beaker/base/evaluation/evaluate.hpp>

#include "../ast.hpp"


namespace beaker {

value
evaluate(evaluator& eval, const expr& e)
{
  switch (e.get_kind()) {
#define def_expr(NS, E, B) \
    case NS::E ## _expr_kind: \
      return evaluate(eval, cast<NS::E ## _expr>(e));
#include <beaker/all/expr.def>
  }
  assert(false && "invalid expression");
}


} // namespace beaker
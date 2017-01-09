// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "evaluation.hpp"
#include "expression.hpp"


namespace beaker {
namespace numeric {

// Returns the value of a literal expression.
template<typename T>
static inline value
get_literal(const expr& e) 
{
  return cast<T>(e).get_value();
}

// Evaluate the expression e.
//
// TODO: Actually implement me.
value
evaluate(evaluator& eval, const expr& e) 
{
  switch (e.get_kind()) {
    case int_expr_kind:
      return get_literal<int_expr>(e);
    case float_expr_kind:
      return get_literal<float_expr>(e);
    default:
      break;
  }
  assert(false && "not a numeric expression");
}

} // namespace numeric
} // namespace beaker

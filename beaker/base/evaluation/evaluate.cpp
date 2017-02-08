// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "evaluate.hpp"
#include "../expr.hpp"

namespace beaker {

// Returns the equality algorithm associated with the node t.
static inline const evaluate_algorithm&
get_algorithm(const language& lang)
{
  return lang.get_algorithm<evaluate_algorithm>();
}

// Generates the dispatch table for the algorithm
evaluate_algorithm::evaluate_algorithm(language& lang)
  : exprs(new expr_table(lang.get_expressions()))
{ }

/// Evaluate the expression `e`.
value
evaluate(evaluator& eval, const expr& e)
{
  const auto& tab = *get_algorithm(eval.lang).exprs;
  auto fn = tab.get_overrider(e);
  return fn(eval, e);
}

} // namespace beaker


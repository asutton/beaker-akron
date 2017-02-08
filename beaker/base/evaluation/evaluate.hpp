// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_EVALUATION_EVALUATE_HPP
#define BEAKER_EVALUATION_EVALUATE_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/error.hpp>
#include <beaker/base/value.hpp>


namespace beaker {

/// Represents an error that occurred during evaluation. These exceptions can
/// be caught and suppressed to support compile-time evaluation.
///
/// \todo: Allow errors to indicate declarations and statements also.
struct evaluation_error : error
{
  using error::error;
};


/// The evaluator maintains the evaluation context for the evaluation of the
/// expressions, declarations, and statements.
struct evaluator
{
  evaluator(const language&);
  
  const language& lang;
};

inline evaluator::evaluator(const language& lang) : lang(lang) { }


/// The evaluation algorithm computes values and side effects for expressions,
/// statements, and declarations.
///
/// \todo Implement statement evaluation (flow control) and declaration
/// elaboration.
struct evaluate_algorithm : algorithm
{
  using expr_table = dispatch_table<value(evaluator&, const expr&)>;

  evaluate_algorithm(language&);
  
  std::unique_ptr<expr_table> exprs;
};

value evaluate(evaluator&, const expr&);
value evaluate(evaluator&, const decl&);
value evaluate(evaluator&, const stmt&);

} // namespace beaker


#endif

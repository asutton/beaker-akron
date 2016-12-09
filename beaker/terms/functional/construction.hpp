// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_TERM_FUNCTIONAL_CONTEXT_HPP
#define BEAKER_TERM_FUNCTIONAL_CONTEXT_HPP

#include <beaker/terms/functional/expression.hpp>

#include <beaker/common/construction.hpp>


namespace beaker {
namespace functional {

// Provides access to resources needed to construct, validate, and
// evaluate functional terms.
struct builder
{
  builder(allocator&);

  // Expressions
  lambda_expr& make_lambda_expr(type&, decl&);
  let_expr& make_let_expr(type&, decl&, expr&);
  seq_expr& make_seq_expr(type&, expr&, expr&);

  allocator* alloc_;
};

} // namespace functional
} // namespace


#endif

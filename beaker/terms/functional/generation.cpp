// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "generation.hpp"
#include "expression.hpp"

#include <llvm/IR/IRBuilder.h>

#include <iostream>


namespace beaker {
namespace functional {

// Generate a lambda expression.
//
// This is a reference to the anonymous function.
static llvm::Value*
generate_lambda_expr(generator& gen, const lambda_expr& e)
{
  generator sub(gen);
  return generate(sub, e.get_function());
}

static llvm::Value*
generate_let_expr(generator& gen, const let_expr& e)
{
  generator::decl_context_guard guard(gen);
  generate(gen, e.get_variable());
  llvm::Value* ret = generate(gen, e.get_expression());
  return ret;
}

// Generate a sequencing of expressions.
//
// The left operand is evaluated first and the result is discarded.
static llvm::Value*
generate_seq_expr(generator& gen, const seq_expr& e)
{
  generate(gen, e.get_left());
  return generate(gen, e.get_right());
}


llvm::Value* 
generate_expr(generator& gen, const expr& e) 
{
  switch (e.get_kind()) {
    case lambda_expr_kind:
      return generate_lambda_expr(gen, cast<lambda_expr>(e));
    case let_expr_kind:
      return generate_let_expr(gen, cast<let_expr>(e));
    case seq_expr_kind:
      return generate_seq_expr(gen, cast<seq_expr>(e));
    default:
      break;
  }
  assert(false && "not a functional expression");
}

} // namespace functional
} // namespace beaker


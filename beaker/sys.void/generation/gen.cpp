// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "gen.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_void {

cg::type
gen_algo::operator()(generator& gen, const type& t) const
{
  assert(is_void_type(t));
  return llvm::Type::getVoidTy(gen.get_context());
}

/// Generate the code for the expression e and return nullptr.
static cg::value
generate_void_expr(generator& gen, const void_expr& e)
{
  generate(gen, e.get_operand());
  return nullptr;
}

cg::value
gen_algo::operator()(generator& gen, const expr& e) const
{
  switch (e.get_kind()) {
    case nop_expr_kind:
      return nullptr;
    case void_expr_kind:
      return generate_void_expr(gen, cast<void_expr>(e));
    default:
      break;
  }
  assert(false && "not a void epxression");
}

} // namespace sys_void
} // namespace beaker


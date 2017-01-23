// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "gen.hpp"
#include "../type.hpp"
#include "../expr.hpp"

#include <llvm/IR/Intrinsics.h>


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

// Emits a trap instruction.
//
// FIXME: Emit a debug trap when debugging is enabled.
static cg::value
generate_trap_expr(generator& gen, const trap_expr& e)
{
  llvm::Module& mod = gen.get_module();
  llvm::Builder ir(gen.get_current_block());
  cg::value op = llvm::Intrinsic::getDeclaration(&mod, llvm::Intrinsic::trap);
  std::vector<llvm::Value*> args;
  ir.CreateCall(op, args);
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
    case trap_expr_kind:
      return generate_trap_expr(gen, cast<trap_expr>(e));
    default:
      break;
  }
  assert(false && "not a void epxression");
}

} // namespace sys_void
} // namespace beaker


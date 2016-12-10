// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/variadic/generation/gen.hpp>
#include <beaker/variadic/type.hpp>
#include <beaker/variadic/expr.hpp>

#include <llvm/IR/Function.h>
#include <llvm/IR/Intrinsics.h>


namespace beaker {
namespace variadic {

/// \todo The actual type of this object depends on the architecture. We need 
/// to start propagating architectural information through the code generator.
cg::type
gen_algo::operator()(generator& gen, const type& t) const
{
  assert(t.get_kind() == list_type_kind);
  return llvm::Type::getInt8Ty(gen.get_context());
}

/// \todo Figure out how to make a call to @llvm.va_start.
cg::value
generate_start_expr(generator& gen, const start_expr& e)
{
  llvm::Module& mod = gen.get_module();
  llvm::Value* args[] { 
    generate(gen, e.get_list())
  };
  cg::value op = llvm::Intrinsic::getDeclaration(&mod, llvm::Intrinsic::vastart);
  llvm::Builder ir(gen.get_current_block());
  return ir.CreateCall(op, args);
}

cg::value
generate_end_expr(generator& gen, const end_expr& e)
{
  llvm::Module& mod = gen.get_module();
  llvm::Value* args[] {
    generate(gen, e.get_list())
  };
  cg::value op = llvm::Intrinsic::getDeclaration(&mod, llvm::Intrinsic::vaend);
  llvm::Builder ir(gen.get_current_block());
  return ir.CreateCall(op, args);

}

cg::value
generate_arg_expr(generator& gen, const arg_expr& e)
{
  return nullptr; 
}

/// \todo Figure out how to make a call t
cg::value
generate_copy_expr(generator& gen, const copy_expr& e)
{
  llvm::Module& mod = gen.get_module();
  llvm::Value* args[] { 
    generate(gen, e.get_destination()),
    generate(gen, e.get_source())
  };
  cg::value op = llvm::Intrinsic::getDeclaration(&mod, llvm::Intrinsic::vacopy);
  llvm::Builder ir(gen.get_current_block());
  return ir.CreateCall(op, args);

}

cg::value
gen_algo::operator()(generator& gen, const expr& e) const
{
  switch (e.get_kind()) {
    case start_expr_kind:
      return generate_start_expr(gen, cast<start_expr>(e));
    case end_expr_kind:
      return generate_end_expr(gen, cast<end_expr>(e));
    case arg_expr_kind:
      return generate_arg_expr(gen, cast<arg_expr>(e));
    case copy_expr_kind:
      return generate_copy_expr(gen, cast<copy_expr>(e));
    default:
      break;
  }
  assert(false && "not a variadic expression");
}

} // namespace variadic
} // namespace beaker

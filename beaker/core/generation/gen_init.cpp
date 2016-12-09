// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/core/type.hpp>
#include <beaker/core/expr.hpp>
#include <beaker/core/decl.hpp>
#include <beaker/base/generation/generation.hpp>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>


namespace beaker {
namespace core {

// Initialize the current object with zero.
cg::value
generate_zero_init(generator& gen, const zero_init& e)
{
  llvm::Builder ir(gen.get_current_block());
  cg::value ptr = gen.get_initialized_object();
  cg::value zero = llvm::Constant::getNullValue(ptr->getType());
  ir.CreateStore(ptr, zero);
  return nullptr;
}

// Generate the value of e, and store its result in the object pointed at
// by ptr.
//
// TODO: Consider what happens with indirect objects:
//
//    var big x = make_big();
//
// We materialize a temporary x to store the result of make_big(). The
// result of that expression will be a pointer to that object. We can then
// memcpy the big object into the stored object.
cg::value
generate_copy_init(generator& gen, const copy_init& e)
{
  llvm::Builder ir(gen.get_current_block());
  cg::value ptr = gen.get_initialized_object();
  cg::value val = generate(gen, e.get_operand());
  ir.CreateStore(ptr, val);
  return nullptr;
}

cg::value
generate_call_init(generator& gen, const call_init& e)
{
  assert(false && "not implemented");
  return nullptr;
}

} // namespace core
} // namespace beaker

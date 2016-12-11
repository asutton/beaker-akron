// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

namespace beaker {
namespace core {

// Initialize the current object with zero.
static cg::value
generate_zero_init(generator& gen, const zero_init& e)
{
  cg::value ptr = gen.get_initialized_object();
  cg::value zero = llvm::Constant::getNullValue(ptr->getType());
  llvm::Builder ir(gen.get_current_block());
  ir.CreateStore(zero, ptr);
  return zero;
}

// Generate the value of e and store the result in the current initialization
// target. 
static cg::value
generate_copy_init(generator& gen, const copy_init& e)
{
  cg::value ptr = gen.get_initialized_object();
  cg::value val = generate(gen, e.get_operand());
  llvm::Builder ir(gen.get_current_block());
  ir.CreateStore(val, ptr);
  return nullptr;
}

static cg::value
generate_call_init(generator& gen, const call_init& e)
{
  assert(false && "not implemented");
  return nullptr;
}

} // namespace core
} // namespace beaker

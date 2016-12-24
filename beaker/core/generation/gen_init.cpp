// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/base/printing/print.hpp>

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
// target. Returns the address of the initialized object.
//
// When we are initializing function arguments and return values whose types
// are passed directly, then the current initialization is null. In this case,
// we generate and return the initial value, assuming that it will be provided
// directly to the appropriate context.
static cg::value
generate_copy_init(generator& gen, const copy_init& e)
{
  cg::value val = generate(gen, e.get_operand());
  if (cg::value ptr = gen.get_initialized_object()) {
    // Copying the value depends on whether the is direct or indirect.
    // Note that the copy initializer operand must have the same type as
    // the initialized object.
    cg::type type = generate(gen, e.get_operand().get_type());
    llvm::Builder ir(gen.get_current_block());
    if (type.is_direct()) {
      // For direct values, we always need to store.
      ir.CreateStore(val, ptr);
    }
    else {
      // For indirect values, we may need to memcpy (we require no overlap
      // between the memory of the source and destination).
      //
      // If the value and pointer are the same, then we've already performed
      // some initialization. Only copy when source and destination differ.
      if (ptr != val) {
        // FIXME: Correctly compute the alignment of the type. Also, correctly
        // compute the size of the type based on the target architecture.
        //
        // We'll need to integrate data layout into the generator and evaluator
        // (since compile-time evaluation may necessitate target-dependent info).
        llvm::Constant* size = llvm::ConstantExpr::getSizeOf(type);
        ir.CreateMemCpy(ptr, val, size, 0);
      }
    }
    return ptr;
  }
  return val;
}

static cg::value
generate_call_init(generator& gen, const call_init& e)
{
  assert(false && "not implemented");
  return nullptr;
}

} // namespace core
} // namespace beaker

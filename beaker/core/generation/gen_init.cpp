// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

namespace beaker {
namespace core {

/// Trivially initialize an object. This simply returns the address of the
/// initialized object, without performing initialization.
static cg::value
generate_nop_init(generator& gen, const nop_init& e)
{
  return gen.get_initialized_object(); 
}

/// Initialize the current object with zero values. This returns the address
/// of the initialized object.
static cg::value
generate_zero_init(generator& gen, const zero_init& e)
{
  llvm::Builder ir(gen.get_current_block());

  // Get the initialized object. There must be an initialization context.
  cg::value ptr = gen.get_initialized_object();
  assert(ptr);

  // Get the type of object being initialized.
  cg::type type = generate(gen, e.get_object_type());
  if (type.is_direct()) {
    // When the type is direct, we can simply store a null value.
    cg::value zero = llvm::Constant::getNullValue(type);
    ir.CreateStore(zero, ptr);
  }
  else {
    // When the type is indirect, we need to use memset.
    //
    // TODO: See notes in generate_copy_init about the size and alignment
    // of types.
    cg::value zero = ir.getInt8(0);
    llvm::Constant* size = llvm::ConstantExpr::getSizeOf(type);
    ir.CreateMemSet(ptr, zero, size, 0);
  }
  return ptr;
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
  cg::value val = generate(gen, e.get_expression());
  if (cg::value ptr = gen.get_initialized_object()) {
    // Copying the value depends on whether the is direct or indirect.
    // Note that the copy initializer operand must have the same type as
    // the initialized object.
    cg::type type = generate(gen, e.get_object_type());
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
        // (since compile-time evaluation may necessitate target-dependent 
        // info). Note that we could jam that info in to cg::type.
        llvm::Constant* size = llvm::ConstantExpr::getSizeOf(type);
        ir.CreateMemCpy(ptr, val, size, 0);
      }
    }
    return ptr;
  } else {
    // When there is no initialized object, just return the value. This
    // typically happens when directly initializing a function argument.
    return val;
  }
}

// Generate the object referred to by e and use its address to initialize
// an object or argument.
//
// References are simply pointers and, as such, always passed directly.
static cg::value
generate_ref_init(generator& gen, const ref_init& e)
{
  cg::value val = generate(gen, e.get_expression());
  if (cg::value ptr = gen.get_initialized_object()) {
    // Store the pointer value and return the address of the stored object.
    llvm::Builder ir(gen.get_current_block());
    ir.CreateStore(val, ptr);
    return ptr;
  }
  else {
    // When there is no initialized object, just return the value. This
    // typically happens when directly initializing a function argument.
    return val;
  }
}

static cg::value
generate_call_init(generator& gen, const call_init& e)
{
  assert(false && "not implemented");
  return nullptr;
}

} // namespace core
} // namespace beaker

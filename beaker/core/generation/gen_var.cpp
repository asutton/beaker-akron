// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

namespace beaker {
namespace core {

/// Generate the declaration and initializer.
///
/// \todo Initialization should be an expression that invokes its own
/// store operation (i.e., for copy). It could also be a call to a 
/// constructor, or a memset, or a memcpy, depending on operands.
static cg::value
generate_local_var(generator& gen, const var_decl& d)
{
  // Generate storage in the entry block and bind to it.
  llvm::Builder ir(gen.get_entry_block());
  llvm::Type* type = generate(gen, d.get_type());
  cg::value ptr = ir.CreateAlloca(type);
  gen.put_value(d, ptr);

  // Generate the initialization at the current instruction.
  generator::init_guard guard(gen, ptr);
  assert(d.has_initializer());
  generate(gen, d.get_initializer().get_as<expr>());
  return ptr;
}

/// Generate an LLVM function from d.
///
/// \todo Implement generation of global variables.
static cg::value
generate_var_decl(generator& gen, const var_decl& d)
{
  assert(d.has_storage());

  if (d.has_static_storage()) {
    // Generate a global variable.
  }

  if (d.has_automatic_storage()) {
    return generate_local_var(gen, d);
  }

  // TODO: Generate variables with other storage classes.
  assert(false && "variable not supported");
}

} // namespace core
} // namespace beaker

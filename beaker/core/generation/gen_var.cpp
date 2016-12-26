// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

namespace beaker {
namespace core {

/// A locally declared object requires storage.
static cg::value
generate_local_object_var(generator& gen, const var_decl& d)
{
  // Generate storage in the entry block and bind to it.
  llvm::Builder ir(gen.get_entry_block());
  cg::type type = generate(gen, d.get_type());
  std::string name = generate(gen, d.get_name());
  cg::value ptr = ir.CreateAlloca(type, nullptr, name);
  gen.put_value(d, ptr);

  // Generate the initialization at the current instruction.
  assert(d.has_initializer());
  generator::init_guard guard(gen, ptr);
  generate(gen, d.get_initializer().get_as<expr>());
  return ptr;
}

/// A locally declared reference is an alias to its referenced object.
static cg::value
generate_local_reference_var(generator& gen, const var_decl& d)
{
  // Just evaluate the initializer (without an object, we'll get the value
  // back directly) and bind the declaration to that address.
  assert(d.has_initializer());
  const expr& def = d.get_initializer().get_as<expr>();
  generator::init_guard guard(gen, nullptr);
  cg::value ptr = generate(gen, def);
  gen.put_value(d, ptr);
  return ptr;
}

/// Generate the declaration and initializer.
///
/// \todo Support local function variables.
static cg::value
generate_local_var(generator& gen, const var_decl& d)
{
  const type& t = d.get_type();
  if (is_object_type(t))
    return generate_local_object_var(gen, d);
  else if (is_reference_type(t))
    return generate_local_reference_var(gen, d);
  else
    assert(false && "unsupported declaration");
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

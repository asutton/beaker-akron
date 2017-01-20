// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

namespace beaker {
namespace sys_var {

/// A locally declared object requires storage.
static cg::value
generate_local_object(generator& gen, const var_decl& d)
{
  // Generate storage in the entry block and bind to it.
  cg::type type = generate(gen, d.get_type());
  std::string name = generate(gen, d.get_name());
  cg::value ptr = gen.make_alloca(type, name);
  gen.put_value(d, ptr);

  // Generate the initialization at the current instruction.
  assert(d.has_initializer());
  generator::init_guard guard(gen, ptr);
  generate(gen, d.get_initializer());
  return ptr;
}

/// A locally declared reference is an alias to its referenced object.
///
/// Just evaluate the initializer (without an object, we'll get the value
/// back directly) and bind the declaration to that address.
static cg::value
generate_local_reference(generator& gen, const var_decl& d)
{
  assert(d.has_initializer());
  const expr& def = d.get_initializer();
  generator::init_guard guard(gen, nullptr);
  cg::value ptr = generate(gen, def);
  gen.put_value(d, ptr);
  return ptr;
}

/// Generate the declaration and initializer.
static cg::value
generate_local_var(generator& gen, const var_decl& d)
{
  const type& t = d.get_type();
  if (is_object_type(t))
    return generate_local_object(gen, d);
  else if (is_reference_type(t))
    return generate_local_reference(gen, d);
  else
    assert(false && "unsupported declaration");
}

/// Generate an LLVM function from d.
///
/// \todo Implement generation of global variables.
static cg::value
generate_var_decl(generator& gen, const var_decl& d)
{
  if (d.has_automatic_storage()) {
    return generate_local_var(gen, d);
  }
  else if (d.has_static_storage()) {
    assert(false && "global variables not implemented");
  }

  // TODO: Generate variables with other storage classes.
  assert(false && "unknown storage class");
}

} // namespace sys_var
} // namespace beaker

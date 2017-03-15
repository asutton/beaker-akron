// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

namespace bpl {

/// Determine the initializer for a declaration by an expression.
///
/// \todo Actually implement initialization semantics. 
///
/// \todo Move this into a new module.
void
semantics::copy_initialize(value_decl& d, expr& e)
{
  type& t = d.get_type();
  if (is_object_type(t))
    return object_initialize(d, e);
  if (is_reference_type(t))
    return reference_initialize(d, e);
  assert(false && "invalid object type");
}

/// Try to find an initialization of the object d by the expression e.
///
/// \todo Search for conversions.
void
semantics::object_initialize(value_decl& d, expr& e)
{
  if (!equal(d.get_type(), e.get_type()))
    throw type_error(location(), "copy initialization from expression with wrong type");

  // FIXME: The kind initializer depends on the declaration type.
  expr& init = build_var.make_copy_init(e);
  d.init_ = &init;
}


/// Try to find an initialization of the reference d by the expression e.
///
/// \todo Search for conversions.
void
semantics::reference_initialize(value_decl& d, expr& e)
{
  if (!equal(d.get_type(), e.get_type()))
    throw type_error(location(), "cannot bind to an object of a different type");
  expr& init = build_var.make_ref_init(e);
  d.init_ = &init;
}

} // namespace bpl
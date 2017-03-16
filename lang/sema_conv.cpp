// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

namespace bpl {

/// Determine if `e` can be converted to `t`
/// \todo Propagate source locations in the converted expression.
expr& 
semantics::standard_conversion(expr&e, type& t)
{
  expr* c = &e;

  // The expression already has type t.
  if (has_type(*c, t))
    return *c;

  // Attempt a reference-to-value conversions.
  if (is_reference_type(c->get_type())) {
    c = &build_var.make_val_expr(*c);
    if (has_type(*c, t))
      return *c;
  }
  
  /// FIXME: Improve diagnostics.
  throw type_error(e.get_location(), "cannot convert");
}


/// Perform a conversion to bool.
expr&
semantics::boolean_conversion(expr& e)
{
  return standard_conversion(e, build_bool.get_bool_type());
}

/// Convert both operands to bool.
conv_pair
semantics::boolean_conversion(expr& e1, expr& e2)
{
  return {boolean_conversion(e1), boolean_conversion(e2)};
}

/// Perform a conversion to an appropriate arithmetic type.
///
/// \todo Don't use int32 directly.
expr&
semantics::arithmetic_conversion(expr& e)
{
  return standard_conversion(e, build_int.get_int32_type());
}

/// Perform the usual arithmetic conversions on e1 and e2.
///
/// \todo Propagate source locations for converted expressions.
///
/// \todo Take a source location for the operator that requires a common
/// arithmetic type.
conv_pair
semantics::arithmetic_conversion(expr& e1, expr& e2)
{
  expr* c1 = &e1;
  expr* c2 = &e2;

  // Arithmetic operations require values, not references.
  if (is_reference_type(c1->get_type()))
    c1 = &build_var.make_val_expr(*c1);
  if (is_reference_type(c2->get_type()))
    c2 = &build_var.make_val_expr(*c2);

  // FIXME: Only allow arithmetic types. We only have arithmetic types
  // for now, so it's okay.
  if (equal(c1->get_type(), c2->get_type()))
    return {*c1, *c2};

  throw type_error(e1.get_location(), "cannot perform arithmetic conversions");
}

/// Convert e1 and e2 to their common type.
///
/// \todo Define common type and then perform the conversions needed
/// to reach the common type.
conv_pair
semantics::common_conversion(expr& e1, expr& e2)
{
  check_same(e1, e2);
  return {e1, e2};
}

} // namespace bpl

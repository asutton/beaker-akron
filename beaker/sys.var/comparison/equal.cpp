// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "equal.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {

// Returns true if a and b have equivalent object types.
bool
equal_type(const sys_var::ref_type& a, const sys_var::ref_type& b)
{
  return equal(a.get_object_type(), b.get_object_type());
}


bool 
equal_expr(const sys_var::ref_expr& a, const sys_var::ref_expr& b)
{
  return &a.get_declaration() == &b.get_declaration();
}

bool 
equal_expr(const sys_var::deref_expr& a, const sys_var::deref_expr& b)
{
  return equal(a.get_source(), b.get_source());
}

bool 
equal_expr(const sys_var::assign_expr& a, const sys_var::assign_expr& b)
{
  return equal(a.get_lhs(), b.get_lhs()) && equal(a.get_rhs(), b.get_rhs());
}

bool 
equal_expr(const sys_var::nop_init& a, const sys_var::nop_init& b)
{
  assert(false && "not implemented");
}

bool 
equal_expr(const sys_var::zero_init& a, const sys_var::zero_init& b)
{
  assert(false && "not implemented");
}

bool 
equal_expr(const sys_var::copy_init& a, const sys_var::copy_init& b)
{
  assert(false && "not implemented");
}

bool 
equal_expr(const sys_var::ref_init& a, const sys_var::ref_init& b)
{
  assert(false && "not implemented");
}


} // namespace beaker

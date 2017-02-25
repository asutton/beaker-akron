// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "hash.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_var {

// Append the hash of the object type of t to h.
void 
hash_type(hasher& h, const sys_var::ref_type& t)
{
  hash(h, t.get_object_type()); 
}

void
hash_expr(hasher& h, const sys_var::ref_expr& e)
{
  assert(false && "not implemented");
}

void
hash_expr(hasher& h, const sys_var::deref_expr& e)
{
  assert(false && "not implemented");
}

void
hash_expr(hasher& h, const sys_var::assign_expr& e)
{
  assert(false && "not implemented");
}

void
hash_expr(hasher& h, const sys_var::nop_init& e)
{
  assert(false && "not implemented");
}

void
hash_expr(hasher& h, const sys_var::zero_init& e)
{
  assert(false && "not implemented");
}

void
hash_expr(hasher& h, const sys_var::copy_init& e)
{
  assert(false && "not implemented");
}

void
hash_expr(hasher& h, const sys_var::ref_init& e)
{
  assert(false && "not implemented");
}


} // namespace sys_var
} // namespace beaker

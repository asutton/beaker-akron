// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "build.hpp"
#include "type.hpp"
#include "expr.hpp"

namespace beaker {
namespace sys_void {

builder::builder(module& m)
  : beaker::builder(m),
    void_(&get_language().make_singleton_set<void_type>())
{ }

/// Returns a the `void` type.
void_type&
builder::get_void_type()
{
  return void_->get();
}

/// Returns a new expression `nop`.
nop_expr&
builder::make_nop_expr()
{
  return make<nop_expr>(get_void_type());
}

/// Returns a new expression `void(e)`
void_expr&
builder::make_void_expr(expr& e)
{
  return make<void_expr>(get_void_type(), e);
}

/// Returns a new expression `trap`.
trap_expr&
builder::make_trap_expr()
{
  return make<trap_expr>(get_void_type());
}

} // namespace sys_void
} // namespace beaker

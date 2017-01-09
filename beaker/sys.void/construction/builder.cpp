// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "builder.hpp"
#include "../type.hpp"
#include "../expr.hpp"
#include <beaker/base/module.hpp>

namespace beaker {
namespace sys_void {

builder::builder(module& m)
  : basic_builder<sys_void_lang>(m),
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


} // namespace sys_void
} // namespace beaker

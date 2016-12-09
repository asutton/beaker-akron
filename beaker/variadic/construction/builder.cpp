// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "builder.hpp"
#include <beaker/variadic/type.hpp>
#include <beaker/variadic/expr.hpp>
#include <beaker/base/module.hpp>


namespace beaker {
namespace variadic {

/// Allocate a node builder for the language feature.
void* make_builder(module& m) { return new builder(m); }


builder::builder(module& m)
  : builder_base(m)
{ }

/// Returns the canonical list type.
list_type&
builder::get_list_type()
{
  return list_.get();
}

start_expr&
builder::make_start_expr(type& t, expr& l)
{
  return make<start_expr>(t, l);
}

end_expr&
builder::make_end_expr(type& t, expr& l)
{
  return make<end_expr>(t, l);
}

arg_expr&
builder::make_arg_expr(type& t, expr& l)
{
  return make<arg_expr>(t, l);
}

copy_expr&
builder::make_copy_expr(type& t, expr& l1, expr& l2)
{
  return make<copy_expr>(t, l1, l2);
}

} // namespace variadic
} // namespace beaker


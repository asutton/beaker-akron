// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "builder.hpp"
#include <beaker/procedural/stmt.hpp>
#include <beaker/base/module.hpp>


namespace beaker {
namespace procedural {

/// Allocate a node builder for the language feature.
void* make_builder(module& m) { return new builder(m); }


builder::builder(module& m)
  : builder_base(m)
{ }


if_then_stmt& 
builder::make_if_then_stmt(expr& e, stmt& s)
{
  return make<if_then_stmt>(e, s);
}

if_else_stmt& 
builder::make_if_else_stmt(expr& e, stmt& s1, stmt& s2)
{
  return make<if_else_stmt>(e, s1, s2);
}

while_stmt& 
builder::make_while_stmt(expr& e, stmt& s)
{
  return make<while_stmt>(e, s);
}

do_while_stmt& 
builder::make_do_while_stmt(expr& e, stmt& s)
{
  return make<do_while_stmt>(e, s);
}

} // namespace procedural
} // namespace beaker


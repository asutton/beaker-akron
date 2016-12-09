// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "builder.hpp"
#include <beaker/sample/name.hpp>
#include <beaker/sample/type.hpp>
#include <beaker/sample/expr.hpp>
#include <beaker/sample/decl.hpp>
#include <beaker/sample/stmt.hpp>
#include <beaker/base/module.hpp>


namespace beaker {
namespace sample {

/// Allocate a node builder for the language feature.
void* make_builder(module& m) { return new builder(m); }


builder::builder(module& m)
  : builder_base(m)
{ }

sample_name&
builder::get_sample_name()
{
  return make<sample_name>();
}

sample_type&
builder::get_sample_type()
{
  return make<sample_type>();
}

sample_expr&
builder::make_sample_expr(type& t)
{
  return make<sample_expr>(t);
}

sample_decl&
builder::make_sample_decl()
{
  return make<sample_decl>();
}

sample_stmt&
builder::make_sample_stmt()
{
  return make<sample_stmt>();
}

} // namespace sample
} // namespace beaker


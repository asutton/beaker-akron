// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "build.hpp"
#include "type.hpp"
#include "expr.hpp"

#include <algorithm>


namespace beaker {
namespace sys_tuple {

builder::builder(module& m)
  : basic_builder<sys_tuple_lang>(m),
    tup_(&get_language().make_canonical_set<tuple_type>())
{ }

/// FIXME: Check for non-void types.
static bool
check_element_types(const type_seq& ts)
{
  return std::all_of(ts.begin(), ts.end(), is_object_type);
}

tuple_type& 
builder::get_tuple_type(const type_seq& ts)
{
  assert(check_element_types(ts));
  return tup_->get(ts);
}

tuple_type& 
builder::get_tuple_type(type_seq&& ts)
{
  assert(check_element_types(ts));
  return tup_->get(std::move(ts));
}

/// Returns a tuple type for a sequence of expressions. Reference types are
/// automatically adjusted object types.
tuple_type& 
builder::get_tuple_type(const expr_seq& es)
{
  type_seq ts;
  for (const expr& e : es) {
    expr& elem = modifiable(e);
    type& type = get_object_type(elem.get_type());
    ts.push_back(type);
  }
  return get_tuple_type(std::move(ts));
}

/// Returns a new tuple expr.
///
/// \todo Apply object-to-reference conversions as needed.
tuple_expr& 
builder::make_tuple_expr(const expr_seq& es)
{
  tuple_type& type = get_tuple_type(es);
  return make<tuple_expr>(type, es);
}

tuple_expr& 
builder::make_tuple_expr(expr_seq&& es)
{
  tuple_type& t = get_tuple_type(es);
  return make<tuple_expr>(t, std::move(es));
}

/// Returns a new expression that computes the nth value of a tuple.
proj_expr& 
builder::make_proj_expr(expr& e, int n)
{
  tuple_type& t = cast<tuple_type>(e.get_type());
  return make<proj_expr>(t, e, n);
}



} // namespace sys_tuple
} // namespace beaker

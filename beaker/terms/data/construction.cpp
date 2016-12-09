// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "construction.hpp"
#include "expression.hpp"

namespace beaker {
namespace data {

builder::builder(allocator& a)
  : alloc_(&a)
{ }

// Returns the canonical tuple type for `{t*}`.
tuple_type&
builder::get_tuple_type(const type_seq& t)
{
  return tuples_.get(t);
}

// Returns the canonical tuple type for `{t*}`.
tuple_type&
builder::get_tuple_type(type_seq&& t)
{
  return tuples_.get(std::move(t));
}

// Returns the canonical array type for `t[n]`.
array_type&
builder::get_array_type(type& t, int n)
{
  return arrays_.get(t, n);
}

// Returns the canonical sequence type for `t[]`.
seq_type&
builder::get_seq_type(type& t)
{
  return seqs_.get(t);
}


// Returns a new tuple expression.
tuple_expr&
builder::make_tuple_expr(type& t, const expr_seq& e)
{
  return make<tuple_expr>(*alloc_, t, e);
}

// Returns a new tuple expression.
tuple_expr&
builder::make_tuple_expr(type& t, expr_seq&& e)
{
  return make<tuple_expr>(*alloc_, t, std::move(e));
}

// Returns a new element access expression.
elem_expr&
builder::make_elem_expr(type& t, expr& e, int n)
{
  return make<elem_expr>(*alloc_, t, e, n);
}

// Returns a new array index expression.
index_expr&
builder::make_index_expr(type& t, expr& e, expr& n)
{
  return make<index_expr>(*alloc_, t, e, n);
}

} // namespace data
} // namespace beaker

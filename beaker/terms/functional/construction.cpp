// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "construction.hpp"

namespace beaker {
namespace functional {

builder::builder(allocator& a)
  : alloc_(&a)
{ }


lambda_expr& 
builder::make_lambda_expr(type& t, decl& d)
{
  return make<lambda_expr>(*alloc_, t, d);
}

let_expr& 
builder::make_let_expr(type& t, decl& d, expr& e)
{
  return make<let_expr>(*alloc_, t, d, e);
}

seq_expr& 
builder::make_seq_expr(type& t, expr& e1, expr& e2)
{
  return make<seq_expr>(*alloc_, t, e1, e2);
}


} // namespace functional
} // namespace beaker

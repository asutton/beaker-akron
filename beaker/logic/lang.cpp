// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "lang.hpp"

#include <cassert>


namespace beaker {

extern bool eq_type(const type&, const type&);
extern bool eq_expr(const expr&, const expr&);

bool 
feature::eq(const type& a, const type& b)
{
  return eq_type(a, b);
}

bool 
feature::eq(const expr& a, const expr& b)
{
  return eq_expr(a, b);
}


extern void hash_type(hasher& h, const type&);
extern void hash_expr(hasher& h, const expr&);

void
feature::hash(hasher& h, const type& t)
{ 
  hash_type(h, t);
}

void
feature::hash(hasher& h, const expr& e)
{ 
  hash_expr(h, e);
}

extern void print_type(std::ostream&, const type&);
extern void print_expr(std::ostream&, const expr&);

void
feature::print(std::ostream& os, const type& t)
{ 
  print_type(os, t);
}

void
feature::print(std::ostream& os, const expr& e)
{ 
  print_expr(os, e);
}

extern cg::type gen_type(generator& gen, const type&);
extern cg::value gen_expr(generator& gen, const expr&);

cg::type
feature::gen(generator& gen, const type& t)
{
  return gen_type(gen, t);
}

cg::value
feature::gen(generator& gen, const expr& e)
{
  return gen_expr(gen, e);
}

} // namespace beaker

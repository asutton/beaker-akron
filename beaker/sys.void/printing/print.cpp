// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../type.hpp"
#include "../expr.hpp"

#include <iostream>


namespace beaker {
namespace sys_void {

void
print_algo::operator()(std::ostream& os, const type& t) const
{
  assert(is_void_type(t));
  os << "void";
}


void
print_algo::operator()(std::ostream& os, const expr& e) const
{
  switch (e.get_kind()) {
    case nop_expr_kind:
      os << "nop";
      break;
    case void_expr_kind:
      return print_prefix_expr(os, cast<void_expr>(e), "void");
    default:
      break;
  }
  assert(false && "not a void expression");
}

} // namespace sys_void
} // namespace beaker


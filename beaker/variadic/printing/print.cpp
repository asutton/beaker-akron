// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/variadic/printing/print.hpp>
#include <beaker/variadic/type.hpp>
#include <beaker/variadic/expr.hpp>

#include <iostream>


namespace beaker {
namespace variadic {

void
print_algo::operator()(std::ostream& os, const type& t) const
{
  assert(is<list_type>(t));
  os << "va.list";
}

void
print_algo::operator()(std::ostream& os, const expr& e) const
{
  assert(false && "not implemented");
}

} // namespace variadic
} // namespace beaker


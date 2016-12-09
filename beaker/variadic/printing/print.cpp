// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/variadic/type.hpp>
#include <beaker/variadic/expr.hpp>
#include <beaker/base/printing/print.hpp>

#include <iostream>


namespace beaker {
namespace variadic {

void
print_name(std::ostream& os, const name& n)
{
  assert(false && "not defined");
}

void
print_type(std::ostream& os, const type& t)
{
  assert(is<list_type>(t));
  os << "va.list";
}

void
print_expr(std::ostream& os, const expr& e)
{
  assert(false && "not implemented");
}

void
print_decl(std::ostream& os, const decl& d)
{
  assert(false && "not defined");
}

void
print_stmt(std::ostream& os, const stmt& s)
{
  assert(false && "not defined");
}

} // namespace variadic
} // namespace beaker


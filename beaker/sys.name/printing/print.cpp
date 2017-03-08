// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../name.hpp"

#include <beaker/base/symbol.hpp>


namespace beaker {

void 
print_name(pretty_printer& pp, const sys_name::basic_name& n)
{
  print(pp, n.get_symbol());
}

void 
print_name(pretty_printer& pp, const sys_name::internal_name& n)
{
  pp.print("<internal>");
}

} // namespace beaker

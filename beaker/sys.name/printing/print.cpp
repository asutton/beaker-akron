// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../name.hpp"

#include <beaker/util/symbol.hpp>


namespace beaker {

void 
print(pretty_printer& pp, const sys_name::basic_name& n)
{
  pp.print(n.get_symbol().get_spelling());
}

void 
print(pretty_printer& pp, const sys_name::internal_name& n)
{
  pp.print("<internal>");
}

} // namespace beaker

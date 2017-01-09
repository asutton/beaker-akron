// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../name.hpp"
#include <beaker/util/symbol.hpp>

#include <iostream>


namespace beaker {
namespace sys_name {

void
print_algo::operator()(std::ostream& os, const name& n) const
{
  switch (n.get_kind()) {
    case basic_name_kind:
      os << cast<basic_name>(n).get_symbol().get_spelling();
      return;
    case internal_name_kind:
      os << "<internal>";
      return;
    default:
      break;
  }
  assert(false && "not a system name");
}

} // namespace sys_name
} // namespace beaker

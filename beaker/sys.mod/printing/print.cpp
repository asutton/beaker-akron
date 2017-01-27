// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../decl.hpp"

#include <iostream>


namespace beaker {
namespace sys_mod {

template<typename T>
static void
print_module_decl(std::ostream& os, const T& d, const char* intro)
{
  print(os, intro);
  print(os, ' ');
  print(os, d.get_name());
  print(os, ';');
  print(os, '\n');
}

void
print_algo::operator()(std::ostream& os, const decl& d) const
{
  if (is<module_decl>(d))
    return print_module_decl(os, cast<module_decl>(d), "module");
  else if (is<import_decl>(d))
    return print_module_decl(os, cast<import_decl>(d), "import");
  assert(false && "not a module declaration");
}

} // namespace sys_mod
} // namespace beaker


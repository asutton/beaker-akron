// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../decl.hpp"

#include <beaker/base/module.hpp>

#include <iostream>


namespace beaker {
namespace sys_mod {

static void
print_module_decl(std::ostream& os, const module_decl& d)
{
  print(os, "module ");
  print(os, d.get_name());
  print(os, ";\n");
}

static void
print_import_decl(std::ostream& os, const import_decl& d)
{
  print(os, "import ");
  print(os, d.get_import().get_module_name());
  print(os, ";\n");
}

void
print_algo::operator()(std::ostream& os, const decl& d) const
{
  if (is<module_decl>(d))
    return print_module_decl(os, cast<module_decl>(d));
  else if (is<import_decl>(d))
    return print_import_decl(os, cast<import_decl>(d));
  assert(false && "not a module declaration");
}

} // namespace sys_mod
} // namespace beaker


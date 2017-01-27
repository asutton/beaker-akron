// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "write.hpp"
#include "../decl.hpp"


namespace beaker {
namespace sys_mod {

static inline void
write_module_decl(archive_writer& ar, const module_decl& d)
{
  write_name(ar, d.get_name());
}

// FIXME: Add the module to the archive's module section and write a reference 
// to it?
static inline void
write_import_decl(archive_writer& ar, const import_decl& d)
{
  assert(false && "not implemented");
}

void
write_algo::operator()(archive_writer& ar, const decl& d) const
{
  if (is<module_decl>(d))
    return write_module_decl(ar, cast<module_decl>(d));
  else if (is<import_decl>(d))
    return write_import_decl(ar, cast<import_decl>(d));
  assert(false && "not a module declaration");
}

} // namespace sys_mod
} // namespace beaker

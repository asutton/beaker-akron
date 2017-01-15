// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "write.hpp"
#include "../name.hpp"


namespace beaker {
namespace sys_name {

// Append the hash of t's parameter and return types to h.
static void
write_basic_name(archive_writer& ar, const basic_name& n)
{
  write_string(ar, n.get_symbol());
}

static void
write_internal_name(archive_writer& ar, const internal_name& n)
{
  write_int(ar, n.get_id());
}

void
write_algo::operator()(archive_writer& ar, const name& n) const
{
  if (is<basic_name>(n))
    return write_basic_name(ar, cast<basic_name>(n));
  else if (is<internal_name>(n))
    return write_internal_name(ar, cast<internal_name>(n));
  else
    assert(false && "not a name");
}

} // namespace sys_name
} // namespace beaker

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "write.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_tuple {

static void
write_tuple_type(archive_writer& ar, const tuple_type& t)
{
  write_seq(ar, t.get_element_types());
}

void
write_algo::operator()(archive_writer& ar, const type& t) const
{
  write_tuple_type(ar, cast<tuple_type>(t));
}


static void 
write_tuple_expr(archive_writer& ar, const tuple_expr& e)
{
  write_seq(ar, e.get_elements());
}

static void
write_proj_expr(archive_writer& ar, const proj_expr& e)
{
  write_expr(ar, e.get_object());
  write_int(ar, e.get_element());
}

void
write_algo::operator()(archive_writer& ar, const expr& e) const
{
  switch (e.get_kind()) {
    case tuple_expr_kind:
      return write_tuple_expr(ar, cast<tuple_expr>(e));
    case proj_expr_kind:
      return write_proj_expr(ar, cast<proj_expr>(e));
    default:
      break;
  }
  assert(false && "not a tuple expression");
}

} // namespace sys_tuple
} // namespace beaker

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "write.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_void {

void
write_algo::operator()(archive_writer& ar, const type& t) const
{
  assert(is<void_type>(t));
  // There's nothing to add to the encoding.
}


static void
write_void_expr(archive_writer& ar, const void_expr& e)
{
  write_expr(ar, e.get_operand());
}

void
write_algo::operator()(archive_writer& ar, const expr& e) const
{
  switch (e.get_kind()) {
    case nop_expr_kind:
      // Nothing to add.
      return;
    case void_expr_kind:
      return write_void_expr(ar, cast<void_expr>(e));
    default:
      break;
  }
  assert(false && "not a void expression");
}


} // namespace sys_void
} // namespace beaker

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "write.hpp"
#include "../type.hpp"
#include "../expr.hpp"
#include "../decl.hpp"


namespace beaker {
namespace sys_bool {

void
write_algo::operator()(archive_writer& ar, const type& t) const
{
  assert(is<bool_type>(t));
  // There's nothing to add to the encoding.
}


static void
write_bool_expr(archive_writer& ar, const bool_expr& e)
{
  write_int(ar, e.get_boolean());
}

void
write_algo::operator()(archive_writer& ar, const expr& e) const
{
  switch (e.get_kind()) {
    case bool_expr_kind:
      return write_bool_expr(ar, cast<bool_expr>(e));
    case and_expr_kind:
    case or_expr_kind:
    case imp_expr_kind:
    case eq_expr_kind:
    case and_then_expr_kind:
    case or_else_expr_kind:
      return write_binary_expr(ar, static_cast<const binary_expr&>(e));
    case not_expr_kind:
      return write_unary_expr(ar, static_cast<const unary_expr&>(e));
    default:
      break;
  }
  assert(false && "not a function expression");
}


void
write_algo::operator()(archive_writer& ar, const decl& d) const
{
  assert(is<assert_decl>(d));
  write_expr(ar, cast<assert_decl>(d).get_assertion());
}

} // namespace sys_bool
} // namespace beaker

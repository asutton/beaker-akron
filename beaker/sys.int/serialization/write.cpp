// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "write.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_int {

void
write_algo::operator()(archive_writer& ar, const type& t) const
{
  assert(is_integral_type(t));
  write_int(ar, cast<integral_type>(t).get_precision());
}


static void 
write_int_expr(archive_writer& ar, const int_expr& e)
{
  write_int(ar, e.get_integer());
}

void
write_algo::operator()(archive_writer& ar, const expr& e) const
{
  switch (e.get_kind()) {
    case int_expr_kind:
      return write_int_expr(ar, cast<int_expr>(e));
    case add_expr_kind:
    case sub_expr_kind:
    case mul_expr_kind:
    case div_expr_kind:
    case rem_expr_kind:
    case eq_expr_kind:
    case ne_expr_kind:
    case lt_expr_kind:
    case gt_expr_kind:
    case le_expr_kind:
    case ge_expr_kind:
      return write_binary_expr(ar, static_cast<const binary_expr&>(e));
    case neg_expr_kind:
    case rec_expr_kind:
      return write_unary_expr(ar, static_cast<const unary_expr&>(e));
    default:
      break;
  }
  assert(false && "not a function expression");
}

} // namespace sys_int
} // namespace beaker

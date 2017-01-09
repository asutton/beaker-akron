// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../type.hpp"
#include "../expr.hpp"

#include <iostream>


namespace beaker {
namespace sys_int {

void 
print_algo::operator()(std::ostream& os, const type& t) const
{
  switch (t.get_kind()) {
#define def_type(T) \
    case T ## _type_kind: \
      os << #T << cast<T ## _type>(t).get_precision(); return;
#include "../type.def"
    default:
      break;
  }
  assert(false && "not a numeric type");
}

void
print_int_expr(std::ostream& os, const int_expr& e)
{
  os << e.get_value().get_integer();
}

/// \todo Finish implementing me.
void
print_algo::operator()(std::ostream& os, const expr& e) const
{
  switch (e.get_kind()) {
    case int_expr_kind:
      return print_int_expr(os, cast<int_expr>(e));

    case eq_expr_kind:
      return print_infix_expr(os, cast<eq_expr>(e), "==");
    case ne_expr_kind:
      return print_infix_expr(os, cast<ne_expr>(e), "!=");
    case lt_expr_kind:
      return print_infix_expr(os, cast<lt_expr>(e), "<");
    case gt_expr_kind:
      return print_infix_expr(os, cast<gt_expr>(e), ">");
    case le_expr_kind:
      return print_infix_expr(os, cast<le_expr>(e), "<=");
    case ge_expr_kind:
      return print_infix_expr(os, cast<ge_expr>(e), ">=");
    
    default:
      break;
  }
  assert(false && "not an integral expression");
}

} // namespace sys_int
} // namespace beaker



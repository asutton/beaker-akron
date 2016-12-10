// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/numeric/printing/print.hpp>
#include <beaker/numeric/type.hpp>
#include <beaker/numeric/expr.hpp>

#include <iostream>


namespace beaker {
namespace numeric {

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
    default:
      break;
  }
  assert(false && "not a numeric expression");
}

} // namespace numeric
} // namespace beaker



// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/numeric/type.hpp>
#include <beaker/numeric/expr.hpp>
#include <beaker/base/printing/print.hpp>

#include <iostream>


namespace beaker {
namespace numeric {

void
print_name(std::ostream& os, const name& n)
{
  assert(false && "not defined");
}

// A helper function for printing numeric types with precision.
template<typename T>
static inline void 
print_numeric_type(std::ostream& os, const T& t, const char* name)
{
  os << name << t.get_precision();
}

void 
print_type(std::ostream& os, const type& t)
{
  switch (t.get_kind()) {
#define def_type(T) \
    case T ## _type_kind: \
      return print_numeric_type(os, cast<T ## _type>(t), #T);
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

void
print_expr(std::ostream& os, const expr& e)
{
  switch (e.get_kind()) {
    case int_expr_kind:
      return print_int_expr(os, cast<int_expr>(e));
    default:
      break;
  }
  assert(false && "not a numeric expression");
}

void
print_decl(std::ostream& os, const decl& d)
{
  assert(false && "not defined");
}

void
print_stmt(std::ostream& os, const stmt& s)
{
  assert(false && "not defined");
}

} // namespace numeric
} // namespace beaker



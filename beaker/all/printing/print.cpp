// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include <beaker/base/printing/print.hpp>

#include "../ast.hpp"


namespace beaker {

void
print(pretty_printer& pp, const name& n)
{
  assert(false && "not implemented");
}

void
print(pretty_printer& pp, const type& t)
{
  switch (t.get_kind()) {
#define def_type(NS, T, B) \
    case NS::T ## _type_kind: \
      return print(pp, cast<NS::T ## _type>(t));
#include <beaker/all/type.def>
  }
  assert(false && "invalid type");
}

/// Returns true if a and b denote the same computations.
void
print(pretty_printer& pp, const expr& e)
{
  switch (e.get_kind()) {
#define def_expr(NS, E, B) \
    case NS::E ## _expr_kind: \
      return print(pp, cast<NS::E ## _expr>(e));
#include <beaker/all/expr.def>
  }
  assert(false && "invalid expression");
}

void
print(pretty_printer& pp, const decl& d)
{
  assert(false && "not implemented");
}

void
print(pretty_printer& pp, const stmt& s)
{
  assert(false && "not implemented");
}

} // namespace beaer
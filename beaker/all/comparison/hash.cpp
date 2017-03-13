// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "hash.hpp"
#include "../ast.hpp"

#include <iostream>


namespace beaker {

/// Appends the kind of n's hash code and dispatch to an appropriate overload.
void
hash(hasher& h, const name& n)
{
  hash(h, n.get_kind());
  switch (n.get_kind()) {
#define def_name(NS, N) \
    case NS::N ## _name_kind: \
      return hash_name(h, cast<NS::N ## _name>(n));
#include <beaker/all/name.def>
  }
  assert(false && "invalid name");
}

/// Appends the kind of t's hash code and dispatch to an appropriate overload.
void
hash(hasher& h, const type& t)
{
  hash(h, t.get_kind());
  switch (t.get_kind()) {
#define def_type(NS, T) \
    case NS::T ## _type_kind: \
      return hash_type(h, cast<NS::T ## _type>(t));
#include <beaker/all/type.def>
  }
  assert(false && "invalid type");
}

/// Appends the kind of n's hash code and dispatch to an appropriate overload.
void
hash(hasher& h, const expr& e)
{
  hash(h, e.get_kind());
  switch (e.get_kind()) {
#define def_expr(NS, E) \
    case NS::E ## _expr_kind: \
      return hash_expr(h, cast<NS::E ## _expr>(e));
#define def_init(NS, E) \
    case NS::E ## _init_kind: \
      return hash_expr(h, cast<NS::E ## _init>(e));
#include <beaker/all/expr.def>
  }
  assert(false && "invalid expression");
}

} // namespace
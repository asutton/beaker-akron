// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "hash.hpp"
#include <beaker/base/name.hpp>
#include <beaker/base/type.hpp>
#include <beaker/base/expr.hpp>


namespace beaker {

#define def_lang(l) \
namespace l { \
  extern void hash_name(hasher&, const name&); \
  extern void hash_type(hasher&, const type&); \
  extern void hash_expr(hasher&, const expr&); \
}
#include "../lang.def"

void
hash(hasher& h, const name& n)
{
  hash(h, typeid(n));
  switch (n.get_feature()) {
#define def_lang(l) \
    case l ## _lang: \
      return l::hash_name(h, n);
#include "../lang.def"
    default:
    break;
  }
  assert(false && "name not supported");
}

void
hash(hasher& h, const type& t)
{
  hash(h, typeid(t));
  switch (t.get_feature()) {
#define def_lang(l) \
    case l ## _lang: \
      return l::hash_type(h, t);
#include "../lang.def"
    default:
    break;
  }
  assert(false && "type not supported");
}


void
hash(hasher& h, const expr& e)
{
  hash(h, typeid(e));
  switch (e.get_feature()) {
#define def_lang(l) \
    case l ## _lang: \
      return l::hash_expr(h, e);
#include "../lang.def"
    default:
    break;
  }
  assert(false && "expression not supported");
}

/// Hash a unary expression e into h.
void
hash_unary_expr(hasher& h, const unary_expr& e)
{
  hash(h, e.get_operand());
}

/// Hash a binary expression e into h.
void
hash_binary_expr(hasher& h, const binary_expr& e)
{
  hash(h, e.get_lhs());
  hash(h, e.get_rhs());
}

} // namespace
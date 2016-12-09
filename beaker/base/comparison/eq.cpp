// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "eq.hpp"
#include <beaker/base/name.hpp>
#include <beaker/base/type.hpp>
#include <beaker/base/expr.hpp>

namespace beaker {

#define def_lang(l) \
namespace l { \
  extern bool equivalent_name(const name&, const name&); \
  extern bool equivalent_type(const type&, const type&); \
  extern bool equivalent_expr(const expr&, const expr&); \
}
#include "../lang.def"


bool
equivalent(const name& a, const name& b) 
{
  if (a.get_kind() != b.get_kind())
    return false;
  feature& feat = language::get_feature(a.get_feature());
  return feat.eq(a, b);
}

#if 0
bool
equivalent(const type& a, const type& b) 
{
  if (a.get_kind() != b.get_kind())
    return false;
  switch (a.get_feature()) {
#define def_lang(l) \
    case l ## _lang: \
      return l::equivalent_type(a, b);
#include "../lang.def"
    default:
    break;
  }
  assert(false && "type not supported");
}

bool
equivalent(const expr& a, const expr& b) 
{
  if (a.get_kind() != b.get_kind())
    return false;
  switch (a.get_feature()) {
#define def_lang(l) \
    case l ## _lang: \
      return l::equivalent_expr(a, b);
#include "../lang.def"
    default:
    break;
  }
  assert(false && "expr not supported");
}

bool
eq_unary_expr(const unary_expr& a, const unary_expr& b)
{
  assert(a.get_kind() == b.get_kind());
  return equivalent(a.get_operand(), b.get_operand());
}

bool
eq_binary_expr(const binary_expr& a, const binary_expr& b)
{
  assert(a.get_kind() == b.get_kind());
  return equivalent(a.get_lhs(), b.get_lhs()) 
      && equivalent(a.get_rhs(), b.get_rhs());
}
#endif

} // namespace
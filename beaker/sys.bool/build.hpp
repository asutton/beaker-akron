// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_BOOL_BUILD_HPP
#define BEAKER_SYS_BOOL_BUILD_HPP

#include <beaker/sys.bool/fwd.hpp>

#include <beaker/base/build.hpp>


namespace beaker {
namespace sys_bool {

struct builder : factory
{
  builder(module&);

  // Canonical types
  bool_type& get_bool_type();

  // Expressions
  bool_expr& make_true_expr();
  bool_expr& make_false_expr();
  bool_expr& make_bool_expr(const value&);
  and_expr& make_and_expr(expr&, expr&);
  or_expr& make_or_expr(expr&, expr&);
  xor_expr& make_xor_expr(expr&, expr&);
  not_expr& make_not_expr(expr&);
  imp_expr& make_imp_expr(expr&, expr&);
  eq_expr& make_eq_expr(expr&, expr&);
  if_expr& make_if_expr(expr&, expr&, expr&);
  and_then_expr& make_and_then_expr(expr&, expr&);
  or_else_expr& make_or_else_expr(expr&, expr&);
  assert_expr& make_assert_expr(expr&);

  singleton_term_set<bool_type>* bool_;
};

} // namespace sys_bool
} // namespace beaker


#endif

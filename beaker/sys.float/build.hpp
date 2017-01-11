// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_FLOAT_BUILD_HPP
#define BEAKER_SYS_FLOAT_BUILD_HPP

#include <beaker/sys.float/fwd.hpp>

#include <beaker/base/build.hpp>


namespace beaker {
namespace sys_float {

// Provides access to resources needed to construct, validate, and
// evaluate numeric terms.
struct builder : basic_builder<sys_float_lang>
{
  builder(module&);

  // Canonical types
  float_type& get_float_type(int);
  float_type& get_float32_type();
  float_type& get_float64_type();

  float_expr& make_float_expr(type&, const value&);
  float_expr& make_float_expr(type&, value&&);
  float_expr& make_float_expr(type&, double);

  eq_expr& make_eq_expr(expr&, expr&);
  ne_expr& make_ne_expr(expr&, expr&);
  lt_expr& make_lt_expr(expr&, expr&);
  gt_expr& make_gt_expr(expr&, expr&);
  le_expr& make_le_expr(expr&, expr&);
  ge_expr& make_ge_expr(expr&, expr&);

  add_expr& make_add_expr(type&, expr&, expr&);
  sub_expr& make_sub_expr(type&, expr&, expr&);
  mul_expr& make_mul_expr(type&, expr&, expr&);
  div_expr& make_div_expr(type&, expr&, expr&);
  neg_expr& make_neg_expr(type&, expr&);
  rec_expr& make_rec_expr(type&, expr&);

  canonical_set<float_type>* float_;
};


// Returns the canonical type `float32`.
inline float_type& builder::get_float32_type() { return get_float_type(32); }

// Returns the canonical type `float64`.
inline float_type& builder::get_float64_type() { return get_float_type(64); }


} // namespace sys_float
} // namespace beaker


#endif

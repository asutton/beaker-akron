// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_INT_BUILD_HPP
#define BEAKER_SYS_INT_BUILD_HPP

#include <beaker/sys.int/fwd.hpp>

#include <beaker/base/build.hpp>


namespace beaker {
namespace sys_int {

// Provides access to resources needed to construct, validate, and
// evaluate numeric terms.
struct builder : factory
{
  builder(module&);

  // Canonical types
  nat_type& get_nat_type(int);
  nat_type& get_nat8_type();
  nat_type& get_nat16_type();
  nat_type& get_nat32_type();
  nat_type& get_nat64_type();

  int_type& get_int_type(int);
  int_type& get_int8_type();
  int_type& get_int16_type();
  int_type& get_int32_type();
  int_type& get_int64_type();

  mod_type& get_mod_type(int);
  mod_type& get_mod8_type();
  mod_type& get_mod16_type();
  mod_type& get_mod32_type();
  mod_type& get_mod64_type();

  int_expr& make_int_expr(type&, const value&);
  int_expr& make_int_expr(type&, value&&);
  int_expr& make_int_expr(type&, int);

  eq_expr& make_eq_expr(expr&, expr&);
  ne_expr& make_ne_expr(expr&, expr&);
  lt_expr& make_lt_expr(expr&, expr&);
  gt_expr& make_gt_expr(expr&, expr&);
  le_expr& make_le_expr(expr&, expr&);
  ge_expr& make_ge_expr(expr&, expr&);

  add_expr& make_add_expr(expr&, expr&);
  sub_expr& make_sub_expr(expr&, expr&);
  mul_expr& make_mul_expr(expr&, expr&);
  div_expr& make_div_expr(expr&, expr&);
  rem_expr& make_rem_expr(expr&, expr&);
  neg_expr& make_neg_expr(expr&);
  rec_expr& make_rec_expr(expr&);

  canonical_term_set<nat_type>* nat_;
  canonical_term_set<int_type>* int_;
  canonical_term_set<mod_type>* mod_;
};

// Returns the canonical type `nat8`.
inline nat_type& builder::get_nat8_type() { return get_nat_type(8); }

// Returns the canonical type `nat16`.
inline nat_type& builder::get_nat16_type() { return get_nat_type(16); }

// Returns the canonical type `nat32`.
inline nat_type& builder::get_nat32_type() { return get_nat_type(32); }

// Returns the canonical type `nat64`.
inline nat_type& builder::get_nat64_type() { return get_nat_type(64); }


// Returns the canonical type `int8`.
inline int_type& builder::get_int8_type() { return get_int_type(8); }

// Returns the canonical type `int16`.
inline int_type& builder::get_int16_type() { return get_int_type(16); }

// Returns the canonical type `int32`.
inline int_type& builder::get_int32_type() { return get_int_type(32); }

// Returns the canonical type `int64`.
inline int_type& builder::get_int64_type() { return get_int_type(64); }


// Returns the canonical type `mod8`.
inline mod_type& builder::get_mod8_type() { return get_mod_type(8); }

// Returns the canonical type `mod16`.
inline mod_type& builder::get_mod16_type() { return get_mod_type(16); }

// Returns the canonical type `mod32`.
inline mod_type& builder::get_mod32_type() { return get_mod_type(32); }

// Returns the canonical type `mod64`.
inline mod_type& builder::get_mod64_type() { return get_mod_type(64); }

} // namespace sys_int
} // namespace beaker


#endif

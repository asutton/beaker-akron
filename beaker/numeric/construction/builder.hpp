// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_TERM_NUMERIC_CONSTRUCTION_HPP
#define BEAKER_TERM_NUMERIC_CONSTRUCTION_HPP

#include <beaker/base/construction/builder.hpp>


namespace beaker {
namespace numeric {

// Forward declare all types and expressions.
#define def_type(T) struct T##_type;
#define def_expr(E) struct E##_expr;
#include "../type.def"
#include "../expr.def"


// Provides access to resources needed to construct, validate, and
// evaluate numeric terms.
//
// TODO: This should use an allocator.
struct builder : basic_builder<numeric_lang>
{
  builder(module&);

  // Canonical types
  char_type& get_char_type(int);
  char_type& get_char8_type();
  char_type& get_char16_type();
  char_type& get_char32_type();

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

  float_type& get_float_type(int);
  float_type& get_float32_type();
  float_type& get_float64_type();

  int_expr& make_int_expr(type&, const value&);
  int_expr& make_int_expr(type&, value&&);
  int_expr& make_int_expr(type&, int n);
  float_expr& make_float_expr(type&, const value&);
  float_expr& make_float_expr(type&, value&&);
  float_expr& make_float_expr(type&, double);

  eq_expr& make_eq_expr(type&, expr&, expr&);
  ne_expr& make_ne_expr(type&, expr&, expr&);
  lt_expr& make_lt_expr(type&, expr&, expr&);
  gt_expr& make_gt_expr(type&, expr&, expr&);
  le_expr& make_le_expr(type&, expr&, expr&);
  ge_expr& make_ge_expr(type&, expr&, expr&);

  add_expr& make_add_expr(type&, expr&, expr&);
  sub_expr& make_sub_expr(type&, expr&, expr&);
  mul_expr& make_mul_expr(type&, expr&, expr&);
  div_expr& make_div_expr(type&, expr&, expr&);
  rem_expr& make_rem_expr(type&, expr&, expr&);
  neg_expr& make_neg_expr(type&, expr&);
  rec_expr& make_rec_expr(type&, expr&);

  canonical_set<char_type> chars_;
  canonical_set<nat_type> nats_;
  canonical_set<int_type> ints_;
  canonical_set<mod_type> mods_;
  canonical_set<float_type> floats_;
};

// Returns the canonical type `char8`.
inline char_type& builder::get_char8_type() { return get_char_type(8); }

// Returns the canonical type `char16`.
inline char_type& builder::get_char16_type() { return get_char_type(16); }

// Returns the canonical type `char32`.
inline char_type& builder::get_char32_type() { return get_char_type(32); }


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


// Returns the canonical type `float32`.
inline float_type& builder::get_float32_type() { return get_float_type(32); }

// Returns the canonical type `float64`.
inline float_type& builder::get_float64_type() { return get_float_type(64); }


/// Returns the integer literal `n` with type t.
///
/// The type of the expression shall be an integral type.
inline 
int_expr& builder::make_int_expr(type& t, int n) 
{
  return make_int_expr(t, value(n)); 
}

/// Returns the floating point literal `n` with type t.
///
/// The type of the expression shall be a floating point type.
inline float_expr&
builder::make_float_expr(type& t, double n) 
{
  return make_float_expr(t, value(n));
}

} // namespace numeric
} // namespace


#endif

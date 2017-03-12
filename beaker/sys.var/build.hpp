// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VAR_BUILD_HPP
#define BEAKER_SYS_VAR_BUILD_HPP

#include <beaker/sys.var/fwd.hpp>
#include <beaker/sys.name/fwd.hpp>

#include <beaker/base/build.hpp>


namespace beaker {
namespace sys_var {

/// Provides access to resources needed to construct, validate, and
/// evaluate core terms.
struct builder : factory
{
  builder(module&);

  // Canonical types
  ref_type& get_ref_type(type&);

  // Expressions
  ref_expr& make_ref_expr(decl&);
  deref_expr& make_deref_expr(expr&);
  assign_expr& make_assign_expr(expr&, expr&);

  nop_init& make_nop_init(type&);
  zero_init& make_zero_init(type&);
  copy_init& make_copy_init(expr&);
  ref_init& make_ref_init(expr&);
  
  canonical_term_set<ref_type>* ref_;
};

} // namespace sys_var
} // namespace beaker


#endif

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VOID_BUILD_HPP
#define BEAKER_SYS_VOID_BUILD_HPP

#include <beaker/sys.void/fwd.hpp>

#include <beaker/base/build.hpp>


namespace beaker {
namespace sys_void {

/// Provides access to resources needed to construct, validate, and
/// evaluate logical terms.
struct builder : basic_builder<sys_void_lang>
{
  builder(module&);

  // Types
  void_type& get_void_type();

  // Expressions
  nop_expr& make_nop_expr();
  void_expr& make_void_expr(expr&);
  trap_expr& make_trap_expr();

  module* mod_;
  singleton_set<void_type>* void_;
};

} // namespace sys_void
} // namespace beaker


#endif

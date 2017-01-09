// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_LOGIC_CONSTRUCTION_BUILDER_HPP
#define BEAKER_LOGIC_CONSTRUCTION_BUILDER_HPP

#include <beaker/base/construction/builder.hpp>


namespace beaker {
namespace sys_void {

struct void_type;
struct nop_expr;
struct void_expr;

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

  module* mod_;
  singleton_set<void_type>* void_;
};

} // namespace sys_void
} // namespace beaker


#endif

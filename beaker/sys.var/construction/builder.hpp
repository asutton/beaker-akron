// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VAR_CONSTRUCTION_BUILDER_HPP
#define BEAKER_SYS_VAR_CONSTRUCTION_BUILDER_HPP

#include <beaker/base/construction/builder.hpp>


namespace beaker {

namespace sys_name {

struct basic_name;
struct internal_name;

} // namespace sys_name

namespace sys_var {

struct ref_type;

struct ref_expr;
struct deref_expr;
struct assign_expr;

struct nop_init;
struct zero_init;
struct copy_init;
struct ref_init;

struct var_decl;

/// Provides access to resources needed to construct, validate, and
/// evaluate core terms.
struct builder : basic_builder<sys_var_lang>
{
  builder(module&);

  // Names
  sys_name::basic_name& get_name(const char*);
  sys_name::internal_name& get_name();

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

  // Declarations
  var_decl& make_var_decl(name&, type&);
  var_decl& make_var_decl(const char*, type&);
  var_decl& make_var_decl(name&, type&, expr&);
  var_decl& make_var_decl(const char*, type&, expr&);
  
  canonical_set<ref_type>* ref_;
};

} // namespace sys_var
} // namespace beaker


#endif

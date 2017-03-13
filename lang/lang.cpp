// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lang.hpp"

#include <beaker/sys.void/ast.hpp>
#include <beaker/sys.bool/ast.hpp>
#include <beaker/sys.int/ast.hpp>
#include <beaker/sys.name/ast.hpp>
#include <beaker/sys.var/ast.hpp>
#include <beaker/sys.fn/ast.hpp>


namespace bpl {

language::language()
  : beaker::language(syms, {
      new beaker::sys_void::feature(), 
      new beaker::sys_bool::feature(), 
      new beaker::sys_int::feature(),
      new beaker::sys_name::feature(),
      new beaker::sys_var::feature(),
      new beaker::sys_fn::feature()
    })
{ }

/// Returns a builder for the requested feature.
beaker::sys_void::builder&
module::get_void_builder()
{
  return get_builder<beaker::sys_void::feature>();
}

/// Returns a builder for the requested feature.
beaker::sys_bool::builder&
module::get_bool_builder()
{
  return get_builder<beaker::sys_bool::feature>();
}

/// Returns a builder for the requested feature.
beaker::sys_int::builder&
module::get_int_builder()
{
  return get_builder<beaker::sys_int::feature>();
}

/// Returns a builder for the requested feature.
beaker::sys_name::builder&
module::get_name_builder()
{
  return get_builder<beaker::sys_name::feature>();
}

/// Returns a builder for the requested feature.
beaker::sys_var::builder&
module::get_var_builder()
{
  return get_builder<beaker::sys_var::feature>();
}

/// Returns a builder for the requested feature.
beaker::sys_fn::builder&
module::get_fn_builder()
{
  return get_builder<beaker::sys_fn::feature>();
}

} // namespace bpl

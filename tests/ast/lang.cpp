// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lang.hpp"

#include <beaker/sys.void/ast.hpp>
#include <beaker/sys.bool/ast.hpp>
#include <beaker/sys.int/ast.hpp>
#include <beaker/sys.name/ast.hpp>
#include <beaker/sys.var/ast.hpp>
#include <beaker/sys.fn/ast.hpp>
#include <beaker/sys.tuple/ast.hpp>


system_lang::system_lang(symbol_table& s)
  : language(s)
{
  add_feature<sys_void::feature>();
  add_feature<sys_bool::feature>();
  add_feature<sys_int::feature>();
  add_feature<sys_name::feature>();
  add_feature<sys_var::feature>();
  add_feature<sys_fn::feature>();
  add_feature<sys_tuple::feature>();
}


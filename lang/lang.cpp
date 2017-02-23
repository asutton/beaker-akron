// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lang.hpp"


namespace bpl {

language::language()
  : beaker::language(syms, {
      new beaker::sys_void::feature(), 
      new beaker::sys_bool::feature(), 
      new beaker::sys_int::feature(),
      new beaker::sys_name::feature()
    })
{ }


builder::builder(module& mod)
  : beaker::sys_void::builder(mod),
    beaker::sys_bool::builder(mod),
    beaker::sys_int::builder(mod),
    beaker::sys_name::builder(mod)
{ }

} // namespace bpl

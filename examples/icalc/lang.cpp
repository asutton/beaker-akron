// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lang.hpp"


namespace icalc {

language::language()
  : beaker::language(syms, {
      new beaker::sys_bool::feature(), 
      new beaker::sys_int::feature()
    })
{ }


builder::builder(module& mod)
  : beaker::sys_bool::builder(mod),
    beaker::sys_int::builder(mod)
{ }

} // namespace icalc

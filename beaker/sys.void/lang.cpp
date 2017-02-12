// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include "build.hpp"


namespace beaker {
namespace sys_void {

factory&
feature::make_builder(module& m) const
{
  return *new builder(m);
}

} // namespace sys_void
} // namespace beaker

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "scope.hpp"
#include "symbol.hpp"

namespace beaker {

/// Returns the bindings for the symbol.
scope_chain&
scope::get_bindings(const symbol& sym)
{
  return sym.get_bindings();
}

} // namespace beaker

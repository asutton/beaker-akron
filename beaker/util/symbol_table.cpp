// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "symbol_table.hpp"


namespace beaker {

/// Try to find a symbol for `str` in the table. Returns a pointer to that
/// symbol if it exists, and nullptr otherwise.
const symbol*
symbol_table::lookup(const std::string& str) const
{
  auto iter = syms.find(symbol(str));
  if (iter == syms.end())
    return nullptr;
  else
    return &*iter;
}

/// Gets a symbol for the given name, inserting a new symbol if it does not
/// exist.
const symbol&
symbol_table::get(const std::string& str)
{
  auto res = syms.emplace(symbol(str));
  return *res.first;
}

} // namespace beaker

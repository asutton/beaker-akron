// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "symbol_table.hpp"


namespace beaker {

// Insert a new symbol with the given spelling and kind. 
const symbol&
symbol_table::insert(const char* s, int k)
{
  cstring str = s;
  auto iter = syms.find(str);
  if (iter != syms.end())
    return *iter->second;

  strs.push_back(s);
  str = strs.back().c_str();
  symbol* sym = new symbol(str, k);
  syms.emplace(str, sym);
  spell.emplace(k, sym);
  return *sym;
}

const symbol*
symbol_table::find_by_spelling(const char* s) const
{
  cstring str = s;
  auto iter = syms.find(str);
  if (iter != syms.end())
    return iter->second;
  else
    return nullptr;
}

const symbol*
symbol_table::find_by_kind(int k) const
{
  auto iter = spell.find(k);
  if (iter != spell.end())
    return iter->second;
  else
    return nullptr;
}

} // namespace beaker

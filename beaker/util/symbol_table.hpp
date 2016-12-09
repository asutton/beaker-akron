// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_UTIL_SYMBOL_TABLE_HPP
#define BEAKER_UTIL_SYMBOL_TABLE_HPP

#include "string.hpp"
#include "symbol.hpp"

#include <list>
#include <string>
#include <unordered_map>


namespace beaker {

// The symbol table maintains a mapping of unique strings in an input
// to their corresponding symbols.
struct symbol_table
{
  struct hash
  {
    std::size_t operator()(cstring s) const
    {
      // TODO: This is a terrible hash function.
      std::size_t n = 0;
      for (char c : s)
        n ^= c;
      return n;
    }
  };

  struct equal
  {
    bool operator()(cstring a, cstring b) const
    {
      if (a.size() != b.size())
        return false;
      return !std::strncmp(a.data(), b.data(), a.size());
    }
  };

  using string_list = std::list<std::string>;
  using symbol_map = std::unordered_map<cstring, symbol*, hash, equal>;
  using spelling_map = std::unordered_map<int, symbol*>;

  symbol_table() = default;

  const symbol* find_by_spelling(const char*) const;
  const symbol* find_by_kind(int) const;

  symbol const& insert(char const*, int k);

  string_list strs;   // Storage for underlying strings
  symbol_map syms;    // Map from spelling to symbol
  spelling_map spell; // Map from kind to symbol
};

} // namespace beaker


#endif

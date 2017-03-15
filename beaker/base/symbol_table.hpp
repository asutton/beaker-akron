// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_SYMBOL_TABLE_HPP
#define BEAKER_BASE_SYMBOL_TABLE_HPP

#include <beaker/base/symbol.hpp>

#include <unordered_set>
#include <vector>


namespace beaker {


/// Hash support for the symbol table. Compute the hash on the spelling of
/// the symbol.
struct symbol_hash
{
  std::size_t operator()(const symbol& sym) const
  {
    std::hash<std::string> h;
    return h(sym.get_spelling());
  }
};

/// Equality support. Returns true when a and b have the same spelling.
struct symbol_eq 
{
  bool operator()(const symbol& a, const symbol& b) const
  {
    return a.get_spelling() == b.get_spelling();
  }
};


// -------------------------------------------------------------------------- //
// Symbol table

// The symbol table maintains a mapping of unique strings in an input
// to their corresponding symbols.
struct symbol_table
{
  using symbol_map = std::unordered_set<symbol, symbol_hash, symbol_eq>;

  symbol_table() = default;

  const symbol* lookup(const std::string&) const;
  const symbol& get(const std::string&);

  symbol_map syms;
};

} // namespace beaker


#endif

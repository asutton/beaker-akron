// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_UTIL_SYMBOL_TABLE_HPP
#define BEAKER_UTIL_SYMBOL_TABLE_HPP

#include <beaker/util/symbol.hpp>

#include <unordered_set>


namespace beaker {

/// FIXME: For lexical definitions, this is properly a "scope".
struct scope_contour : std::vector<const symbol*>
{
  ~scope_contour();

  template<typename T, typename... Args>
  T& add(const symbol& sym, Args&&... args);

  void remove(const symbol& sym);
};

/// Remove all bindings added to this contour during its lifetime.
inline 
scope_contour::~scope_contour()
{
  for (const symbol* sym : *this)
    remove(*sym);
}

/// Add a new binding to this contour.
template<typename T, typename... Args>
inline T&
scope_contour::add(const symbol& sym, Args&&... args)
{
  scope_chain& chain = sym.get_bindings();
  return chain.push<T>(std::forward<Args>(args...));
}

/// Remove a binding from this contour.
inline void
scope_contour::remove(const symbol& sym)
{
  sym.pop();
}


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

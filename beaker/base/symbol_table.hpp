// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_SYMBOL_TABLE_HPP
#define BEAKER_BASE_SYMBOL_TABLE_HPP

#include <beaker/base/symbol.hpp>

#include <unordered_set>
#include <vector>


namespace beaker {

/// Represents the sequence declared names within a region of text.
///
/// \todo Should we have multiple kinds of scopes? Probably. Should scopes
/// be associated with syntactic constructs? Probably.
struct scope_contour : std::vector<const symbol*>
{
  virtual ~scope_contour();

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
  return chain.push<T>(std::forward<Args>(args)...);
}

/// Remove a binding from this contour.
inline void
scope_contour::remove(const symbol& sym)
{
  scope_chain& chain = sym.get_bindings();
  chain.pop();
}


// -------------------------------------------------------------------------- //
// Scope stack

/// The scope stack maintains a stack of contours, which maintains the list
/// of names declared within a region of text.
///
/// \todo Support the allocation different kinds of scopes. These should
/// have a nice object-oriented flavor to them, and they can be specialized
/// to enforce language-specific declaration policies.
struct scope_stack : std::vector<scope_contour*>
{
  void push();
  void pop();
};

/// Push a new empty scope stack.
inline void
scope_stack::push()
{
  push_back(new scope_contour{});
}

/// Pop the current scope from the stack and remove all of the bindings
/// in that contour.
inline void
scope_stack::pop()
{
  assert(!empty() && "unbalanced scope stack");
  scope_contour* s = back();
  pop_back();
  delete s;
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

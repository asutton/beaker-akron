// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_UTIL_SYMBOL_HPP
#define BEAKER_UTIL_SYMBOL_HPP

#include <beaker/util/hash.hpp>

#include <cassert>
#include <list>
#include <memory>
#include <string>


namespace beaker {

// -------------------------------------------------------------------------- //
// Scope chain

/// A scope entry associates information with a symbol declared in some scope.
/// This is the base class of all such objects; that information is defined
/// by the derived language, not this utility library.
///
/// In general a language's scope entry will contain a name binding: the
/// the declaration of the name as a particular kind of entity.
struct scope_entry
{
  virtual ~scope_entry() = default;
};


/// The scope chain is stack of records that associates meaning with a symbol
/// in a symbol table.
struct scope_chain : std::list<std::unique_ptr<scope_entry>>
{
  const scope_entry& top() const;
  scope_entry& top();

  template<typename T, typename... Args>
  T& push(Args&&... args);

  void pop();
};

/// Returns the top of the stack.
inline const scope_entry&
scope_chain::top() const
{
  assert(!empty());
  return *back();
}

/// Returns the top of the stack.
inline scope_entry&
scope_chain::top()
{
  assert(!empty());
  return *back();
}

/// Push a new scope entry of type T (derived from scope_entry) with the given 
/// arguments.
template<typename T, typename... Args>
inline T& 
scope_chain::push(Args&&... args)
{
  emplace_back(new T(std::forward<Args>(args)...));
}

/// Pops the entry at the top of the stack.
inline void
scope_chain::pop()
{
  assert(!empty());
  pop_back();
}


// -------------------------------------------------------------------------- //
// Symbols

/// Represents an abstract symbol in the language. Symbols are typically created
/// to associate meaning with identifiers, although they can be used for other
/// kinds of symbols as well.
///
/// This is essentially a thin wrapper around a string. Only the symbol table
/// can create these objects, and that only occurs when a unique symbol is
/// inserted into the table. A corollary of this is that symbol equality can
/// be defined in terms of object identity.
struct symbol : std::string
{
  friend struct symbol_table;

private:
  symbol(const std::string&);
public:

  const std::string& get_spelling() const;

  scope_chain& get_bindings() const;
  scope_chain& get_bindings();

  bool operator==(const symbol&) const;
  bool operator!=(const symbol&) const;

  mutable scope_chain chain;
};

inline symbol::symbol(const std::string& s) : std::string(s) { }

/// Returns the spelling symbol as a string.
inline const std::string& symbol::get_spelling() const { return *this; }

/// Returns name bindings for the symbol. Note that this is non-const, meaning
/// the symbol's scope chain can be modified at any time.
inline scope_chain& symbol::get_bindings() const { return chain; }

/// Returns name bindings for the symbol.
inline scope_chain& symbol::get_bindings() { return chain; }

/// Returns this and s have the same identity.
inline bool symbol::operator==(const symbol& that) const { return this == &that; }

/// Returns true when this and s have different identities.
inline bool symbol::operator!=(const symbol& that) const { return this != &that; }

// Hash the identity of the symbol.
inline void
hash(hasher& h, const symbol& s)
{
  hash(h, &s);
}

/// Hash support for unordered containers.
struct symbol_hash
{
  std::size_t operator()(const symbol& sym) const
  {
    std::hash<const symbol*> h;
    return h(&sym);
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

} // namespace beaker


#endif

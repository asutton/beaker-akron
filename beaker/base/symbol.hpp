// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_SYMBOL_HPP
#define BEAKER_BASE_SYMBOL_HPP

#include <beaker/base/scope.hpp>

#include <beaker/util/hash.hpp>


namespace beaker {

struct name;
struct decl;
struct type;



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

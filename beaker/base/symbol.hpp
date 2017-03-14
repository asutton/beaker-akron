// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_SYMBOL_HPP
#define BEAKER_BASE_SYMBOL_HPP

#include <beaker/util/hash.hpp>

#include <string>


namespace beaker {

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
///
/// Note that symbols are not directly linked to their meaning. This is differs
/// from many common approaches to working with symbols and their declarations.
/// In order to accommodate languages with structured names, we separate symbols
/// and the symbol table from their use in scope chains. That is defined over
/// the name AST.
struct symbol : std::string
{
  friend struct symbol_table;

private:
  symbol(const std::string&);
public:

  const std::string& get_spelling() const;

  bool operator==(const symbol&) const;
  bool operator!=(const symbol&) const;
};

inline symbol::symbol(const std::string& s) : std::string(s) { }

/// Returns the spelling symbol as a string.
inline const std::string& symbol::get_spelling() const { return *this; }

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

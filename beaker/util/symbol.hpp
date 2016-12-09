// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_UTIL_SYMBOL_HPP
#define BEAKER_UTIL_SYMBOL_HPP

#include <beaker/util/string.hpp>
#include <beaker/util/hash.hpp>


namespace beaker {

// A symbol is the unique textual representation of a single punctuator,
// identifier, etc. in the language.
//
// TODO: Add extra information based on the kind of symbol. This should
// be a variant containing integer, and real data. For identifiers, we
// might also include links into a scope structure.
struct symbol
{
  symbol(cstring s, int k)
    : str(s), sk(k)
  { }

  char const* get_spelling() const;
  int get_kind() const;

  bool operator==(const symbol&);
  bool operator!=(const symbol&);

  cstring str; // Symbol spelling
  int sk; // Symbol kind
};

inline char const*
symbol::get_spelling() const { return str.data(); }

inline int
symbol::get_kind() const { return sk; }

/// Returns this and s have the same identity.
inline bool 
symbol::operator==(const symbol& s) { return get_spelling() == s.get_spelling(); }

/// Returns true when this and s have different identities.
inline bool 
symbol::operator!=(const symbol& s) { return get_spelling() != s.get_spelling(); }

// Hash the identity of the symbol.
inline void
hash(hasher& h, const symbol& s)
{
  hash(h, s.get_spelling());
}

} // namespace beaker


#endif

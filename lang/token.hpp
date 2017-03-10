// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BPL_TOKEN_HPP
#define BPL_TOKEN_HPP

#include <beaker/base/token.hpp>

#include <array>
#include <string>


namespace beaker {

struct symbol;
struct symbol_table;

} // namespace beaker

namespace bpl {

using beaker::location;
using beaker::token;
using beaker::token_seq;
using beaker::int_attr;
using beaker::symbol_attr;

using beaker::symbol;
using beaker::symbol_table;


// Kinds of tokens.
enum token_kind 
{
#define def_token(T, S) T ## _tok,
#define def_keyword(T, S) T ## _kw,
#include "token.def"
};

const char* get_token_name(int);
const char* get_token_name(const token&);

const char* get_token_spelling(int);
std::string get_token_spelling(const token&);


/// An array of tokens.
template<int N>
using tokens = std::array<token, N>;


/// An array of locations.
template<int N>
using locations = std::array<location, N>;


// -------------------------------------------------------------------------- //
// Token operations

/// Returns the integer value associated with the token.
inline std::intmax_t
get_integer(const token& tok)
{
  return tok.get_attribute<int_attr>().get_value();
}

/// Returns the symbol associated with the token.
inline const symbol&
get_symbol(const token& tok)
{
  return tok.get_attribute<symbol_attr>().get_symbol();
}

/// Returns the location of the given token.
inline location
get_location(const token& tok)
{
  return tok.get_location();
}

/// Returns an array of locations for the given tokens.
template<typename... Args>
inline locations<sizeof...(Args)> 
get_locations(const Args&... toks)
{
  return locations<sizeof...(Args)>{(toks.get_location())...};
}

} // namespace bpl


#endif


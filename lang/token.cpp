// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "token.hpp"

#include <cassert>


namespace bpl {

/// Returns the name of the token. For 
const char*
get_token_name(int k)
{
  switch (k) {
#define def_token(T, S) \
    case T ## _tok: return #T;
#define def_keyword(T, S) \
    case T ## _kw: return #T;
#include "token.def"
    default:
      break;
  }
  assert(false && "invalid token");
}

/// Returns the name of the token.
const char*
get_token_name(const token& tok)
{
  return get_token_name(tok.get_kind());
}

/// Returns the spelling of the token's symbol if the spelling is unique. For
/// tokens whose name represents a set of symbols, this returns name describing
/// that set (e.g., integer-literal).
const char*
get_token_spelling(int k)
{
  switch (k) {
#define def_token(T, S) \
    case T ## _tok: return S;
#define def_keyword(T, S) \
    case T ## _kw: return S;
#include "token.def"
    default:
      break;
  }
  assert(false && "invalid token");
}

/// Returns the name of the token.
///
/// \todo The token should have a reference to its lexeme, which would allow
/// us to return a const char*. Maybe.
std::string
get_token_spelling(const token& tok)
{
  switch (tok.get_kind()) {
    default:
      break;
    
    case eof_tok:
      return "end-of-input";

    case bool_tok: {
      auto& attr = static_cast<const beaker::bool_attr&>(tok.get_attribute());
      if (attr.get_value())
        return "true";
      else
        return "false";
    }
    
    case dec_int_tok: {
      // FIXME: Make sure we render the value in the appropriate base. We'll
      // need to add a radix to the int_attr class.
      auto& attr = static_cast<const beaker::int_attr&>(tok.get_attribute());
      return std::to_string(attr.get_value());
    }
  }
  return get_token_spelling(tok.get_kind());
}

} // namespace bpl
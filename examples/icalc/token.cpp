// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "token.hpp"

#include <cassert>


namespace icalc {

/// Returns the name of the token.
const char*
get_token_name(int k)
{
  switch (k) {
#define def_token(T) \
    case T ## _tok: return #T;
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

} // namespace icalc
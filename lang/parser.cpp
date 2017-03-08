// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "parser.hpp"

#include <beaker/base/printing/print.hpp>

#include <iostream>
#include <sstream>
#include <stdexcept>


namespace bpl {

/// Optionally consume a token of kind k.
token
parser::accept(int k)
{
  if (next_token_is(k))
    return consume();
  else
    return {};
}

/// Require that a token to be consumed has kind k.
///
/// \todo Print the expected symbol's name.
token
parser::expect(int k)
{
  if (next_token_is(k))
    return consume();
  
  // FIXME: Actually generate the token spelling.
  std::stringstream ss;
  ss << "expected '" << get_token_spelling(k) 
     << "' but got '" << get_token_spelling(current()) << "'";
  throw syntax_error(get_location(), ss.str().c_str());
}

/// Require that a token that is known to have kind `k`.
token
parser::require(int k)
{
  assert(lookahead() == k);
  return consume();
}

} // namespace bpl

#include "parse_name.cpp"
#include "parse_type.cpp"
#include "parse_expr.cpp"
#include "parse_decl.cpp"
#include "parse_stmt.cpp"

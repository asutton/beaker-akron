// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

namespace bpl {

/// Parse a declaration name.
///
///   declaration-name -> identifier
name&
parser::declaration_name()
{
  if (next_token_is(id_tok))
    return identifier();
  throw syntax_error(location(), "expected identifier");
}

/// Parses an identifier.
name&
parser::identifier()
{
  require(id_tok);
  assert(false && "not implemented");
  return act.on_identifier(require(id_tok));
}


} // namespace bpl

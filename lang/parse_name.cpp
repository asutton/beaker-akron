// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

namespace bpl {

/// Parse a declaration name.
///
///   declaration-name -> identifier
///
/// \todo Support a broader range of declaration names.
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
  return act.on_identifier(require(id_tok));
}


} // namespace bpl

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

namespace bpl {

/// Parse a type-id.
///
///   type-id -> simple-type
///
/// Returns the corresponding type.
type&
parser::type_id()
{
  return simple_type();
}

/// Parse a simple type.
///
///   simple-type -> 'void' | 'bool' | 'int'
type&
parser::simple_type()
{
  switch (lookahead()) {
    case void_kw: {
      token tok = consume();
      return act.on_int_type(get_location(tok));
    }
    case bool_kw: {
      token tok = consume();
      return act.on_bool_type(get_location(tok));
    }
    case int_kw: {
      token tok = consume();
      return act.on_int_type(get_location(tok));
    }
  }
  throw syntax_error(location(), "expected simple-type");
}


} // namespace bpl

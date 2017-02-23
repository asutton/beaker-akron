// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

namespace bpl {

/// Parse a type-id.
///
///   type-id -> simple-type
///
/// Returns the corresponding module.
type&
parser::type_id()
{
  decl_seq decls = toplevel_declaration_seq();
  assert(false && "not implemented");
}

/// Parse a simple type.
type&
parser::simple_type()
{
  switch (lookahead()) {
    case bool_kw:
      assert(false && "bool type not implemented");
    case int_kw:
      assert(false && "bool type not implemented");
  }
  throw syntax_error(location(), "expected simple-type");
}


} // namespace bpl

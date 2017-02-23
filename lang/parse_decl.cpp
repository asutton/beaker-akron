// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

namespace bpl {

/// Parse a module.
///
///   module -> toplevel-declaration-seq
///
/// Returns the corresponding module.
decl&
parser::module()
{
  decl_seq decls = toplevel_declaration_seq();
  assert(false && "not implemented");
}

/// Parse a sequence of declarations.
decl_seq
parser::toplevel_declaration_seq()
{
  switch (lookahead()) {
    case module_kw:
    case import_kw:
    case def_kw:
    default:
      break;
  }
  throw syntax_error(location(), "expected toplevel-declaration");
}

/// Parse a module declaration.
///
///   module-declaration -> 'module' identifier ';'
///
/// \todo A module name is not an identifier.
decl&
parser::module_declaration()
{
  token mod = require(module_kw);
  name& id = identifier();
  token semi = expect(semicolon_tok);
  assert(false && "not implemented");
}

/// Parse an import declaration.
///
///   import-declaration -> 'import' identifier ';'
///
/// \todo A module name is not an identifier.
decl&
parser::import_declaration()
{
  token imp = require(import_kw);
  name& id = identifier();
  token semi = expect(semicolon_tok);
  assert(false && "not implemented");
}

/// Parse a function declaration.
///
///   function-declaration
///     -> 'def' identifier '(' parameter-list ')' '->' type function-definition
///
/// \todo A function name may be a specialization name.
decl&
parser::function_declaration()
{
  token def = require(def_kw);
  name& id = declaration_name();

  // Consume through the parameter list.
  token_seq toks;
  toks.push_back(expect(lparen_tok));
  int match = 1;
  while (match != 0) {
    if (next_token_is(lparen_tok))
      ++match;
    if (next_token_is(rparen_tok))
      --match;
    toks.push_back(consume());
  }

  assert(false && "not implemented");
}


} // namespace bpl

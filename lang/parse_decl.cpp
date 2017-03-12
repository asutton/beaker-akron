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
  // FIXME: Establish a new declaration region for toplevel declarations
  // and ensure that declarations are (in fact) added to the the module.
  decl_seq decls = toplevel_declaration_seq();
  return act.finish_module();
}

/// Parse a sequence of declarations.
///
///   toplevel-declaration-seq -> toplevel-declaration-seq toplevel-declaration
///                             | toplevel-declaration
decl_seq
parser::toplevel_declaration_seq()
{
  decl_seq ds;
  while (!eof()) {
    // FIXME: Trap syntax errors and try to recover as gracefully as possible.
    decl& d = toplevel_declaration();
    ds.push_back(d);
  }
  return ds;
}

/// Parse a toplevel declaration.
///
///   toplevel-declaration -> module-declaration
///                         | import-declaration
///                         | function-declaration
decl&
parser::toplevel_declaration()
{
  switch (lookahead()) {
    case module_kw:
      return module_declaration();
    case import_kw:
      return import_declaration();
    case def_kw:
      return function_declaration();
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
  // token mod = require(module_kw);
  // name& id = identifier();
  // token semi = expect(semicolon_tok);
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
  // token imp = require(import_kw);
  // name& id = identifier();
  // token semi = expect(semicolon_tok);
  assert(false && "not implemented");
}

/// Parse a function declaration.
///
///   function-declaration
///     -> 'def' identifier '(' parameter-list ')' '->' type ';'
///      | 'def' identifier '(' parameter-list ')' '->' type function-definition
///
/// \todo Allow different forms of function name.
///
/// \todo Support different forms of function definitions (e.g., deleted).
decl&
parser::function_declaration()
{
  token def = require(def_kw);

  name& id = declaration_name();

  // FIXME: Parse a parameter declaration list.
  token lpar = expect(lparen_tok);
  token rpar = expect(rparen_tok);

  token arrow = expect(arrow_tok);
  type& ty = type_id();

  if (token semi = accept(semicolon_tok)) {
    auto locs = get_locations(def, lpar, rpar, arrow, semi);
    return act.on_function_declaration(id, ty, locs);
  }

  stmt& body = block_statement();
  auto locs = get_locations(def, lpar, rpar, arrow);
  return act.on_function_declaration(id, ty, body, locs);
}


} // namespace bpl

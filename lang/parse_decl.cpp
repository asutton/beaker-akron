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
  decl& mod = act.on_start_module();
  declarative_region reg(act, mod);
  toplevel_declaration_seq();
  return act.on_finish_module();
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
///     -> 'def' identifier '(' parameter-list ')' return-clause ';'
///      | 'def' identifier '(' parameter-list ')' return-clause function-definition
///
///   return-clause -> '->' type
///
/// Note that the return "type" is actually an output parameter declaration.
///
/// \todo Support named return types?
///
/// \todo Support deduced return types by make the return clause optional.
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
  decl_seq parms;
  token lpar, rpar;
  {
    declarative_region parm_reg(act, function_parameter_scope);
    lpar = expect(lparen_tok);
    rpar = expect(rparen_tok);
  }

  // TODO: Implement named return types.
  // TODO: Support deduced return types.
  token arrow = expect(arrow_tok);
  type& ret = type_id();

  // Declare the function.
  auto locs = get_locations(def, lpar, rpar, arrow);
  decl& fn = act.on_start_function(id, std::move(parms), ret, locs);
  declarative_region fn_reg(act, fn);

  // ';' | function-definitin
  if (token semi = accept(semicolon_tok)) {
    return act.on_finish_function(get_location(semi));
  }
  else {
    stmt& body = block_statement();
    return act.on_finish_function(body);
  }
}


} // namespace bpl

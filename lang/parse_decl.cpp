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
///     -> 'def' identifier '(' function-parameter-list ')' return-clause ';'
///      | 'def' identifier '(' function-parameter-list ')' return-clause function-definition
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

  declarative_region parm_reg(act, function_parameter_scope);
  token lpar = expect(lparen_tok);
  decl_seq parms;
  if (next_token_is_not(rparen_tok))
    parms = function_parameter_list();
  token rpar = expect(rparen_tok);

  // TODO: Implement named return types.
  // TODO: Support deduced return types.
  token arrow = expect(arrow_tok);
  type& ret = type_id();

  // Declare the function.
  auto locs = get_locations(def, lpar, rpar, arrow);
  decl& fn = act.on_start_function(id, std::move(parms), ret, locs);
  declarative_region fn_reg(act, fn);

  // ';' | function-definition
  if (token semi = accept(semicolon_tok)) {
    return act.on_finish_function(fn, get_location(semi));
  }
  else if (next_token_is(lbrace_tok)) {
    declarative_region fn_reg(act, fn);
    stmt& body = block_statement();
    return act.on_finish_function(fn, body);
  }
  else {
    throw syntax_error(current_location(), "expected function-definition");
  }
}

/// Parse a function parameter list.
///
///   function-parameter-list -> function-parameters [...]
///
///   function-parameters -> function-parameters ',' function-parameter
///                        | function-parameter
decl_seq
parser::function_parameter_list()
{
  decl_seq parms;
  while (true) {
    decl& parm = function_parameter();
    parms.push_back(parm);
    if (accept(comma_tok))
      continue;
    if (next_token_is(rparen_tok))
      break;
    else
      throw syntax_error(current_location(), "expected ',' or ')' after function-parameter");
  }
  return parms;
}

/// Parse a function parameteter.
///
///   function-parameter -> type-id identifier.
///
/// \todo Allow unnamed parameters.
///
/// \todo Implement default arguments.
decl&
parser::function_parameter()
{
  type& ty = type_id();
  name& id = identifier();
  return act.on_function_parameter(ty, id);
}

/// Parse a declaration that can be introduced by a statement.
///
///   statement-declaration -> variable-declaration
decl&
parser::statement_declaration()
{
  switch (lookahead()) {
    case var_kw:
      return variable_declaration();
    default:
      break;
  }
  throw syntax_error(current_location(), "expected statement-declaration");
}

/// Parse a variable declaration.
///
///   variable-declaration -> 'var' type-id identifier [variable-initializer] ';'
///
///   variable-initializer -> '=' initializer
///
///   initializer -> expression
///
/// \todo Implement default initialization.
///
/// \todo Support direct initialization and brace initialization.
///
/// \todo Allow variables to have names other than identifiers.
///
/// \todo Support decomposition patterns.
decl&
parser::variable_declaration()
{
  token start = require(var_kw);
  type& ty = type_id();
  name& id = identifier();

  // Declare the variable.
  decl& var = act.on_start_variable(ty, id, get_location(start));
  
  // FIXME
  token eq = expect(eq_tok);
  expr& init = expression();
  token semi = expect(semicolon_tok);

  return act.on_finish_variable(var, init, get_locations(eq, semi));
}


} // namespace bpl

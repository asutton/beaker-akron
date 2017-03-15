// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

namespace bpl {

/// Parse a statement.
stmt&
parser::statement()
{
  switch (lookahead()) {
    default:
      return expression_statement();
    case var_kw:
      return declaration_statement();
    case return_kw:
      return return_statement();
    case lbrace_tok:
      return block_statement();
  }
}

/// Parse a sequence of statements.
///
///   statement-seq -> statement
///                  | statement-seq statement
///
/// The "usual" end of a statement sequence is a '}' because these only occur
/// within block statements.
stmt_seq
parser::statement_seq()
{
  stmt_seq ss;
  while (!eof() && next_token_is_not(rbrace_tok)) {
    stmt& s = statement();
    ss.push_back(s);
  }
  return ss;
}

/// Parse an expression statement.
///
///   expression-statement -> expression ';'
stmt&
parser::expression_statement()
{
  expr& e = expression();
  token semi = expect(semicolon_tok);
  return act.on_expression_statement(e, get_location(semi));
}

/// Parse a declaration statement.
///
///   declaration-statement -> local-declaration
stmt&
parser::declaration_statement()
{
  decl& d = statement_declaration();
  return act.on_declaration_statement(d);
}

/// Parse a return statement.
///
///   return-statement -> 'return' [expression] ';'
stmt&
parser::return_statement()
{
  token ret = require(return_kw);
  if (token semi = accept(semicolon_tok))
    return act.on_return_statement(get_locations(ret, semi));
  expr& e = expression();
  token semi = expect(semicolon_tok);
  return act.on_return_statement(e, get_locations(ret, semi));
}

/// Parse a block statement.
///
///   block-statement -> '{' statement-seq '}'
stmt&
parser::block_statement()
{
  token lb = require(lbrace_tok);
  declarative_region(act, block_scope);
  stmt_seq ss = statement_seq();
  token rb = expect(rbrace_tok);
  return act.on_block_statement(std::move(ss), get_locations(lb, rb));
}

} // namespace bpl

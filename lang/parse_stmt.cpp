// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

namespace bpl {

/// Parse a statement.
stmt&
parser::statement()
{
  switch (lookahead()) {
    case '{':
      return block_statement();
  }
  assert(false && "not implemented");
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
  while (!eof() && next_token_is_not(lbrace_tok)) {
    stmt& s = statement();
    ss.push_back(s);
  }
  return ss;
}

/// Parse a block statement.
///
///   block-statement -> '{' statement-seq '}'
stmt&
parser::block_statement()
{
  token lb = expect(lbrace_tok);
  stmt_seq ss = statement_seq();
  token rb = expect(rbrace_tok);
  return act.on_block_statement(std::move(ss), get_locations(lb, rb));
}


} // namespace bpl

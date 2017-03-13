// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved


namespace bpl {

/// Returns a new block statement.
stmt&
semantics::on_block_statement(stmt_seq&& ss, locations<2> toks)
{
  return build_fn.make_block_stmt(std::move(ss));
}

} // namespace bpl

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved


namespace bpl {

stmt& 
semantics::on_expression_statement(expr&, location)
{
  assert(false && "not implemented");
}

/// Returns a new declaration statement.
stmt& 
semantics::on_declaration_statement(decl& d)
{
  return build_fn.make_decl_stmt(d);
}

/// REturns a new return statement.
stmt& 
semantics::on_return_statement(locations<2> locs)
{
  sys_fn::fn_decl& fn = current_function();
  if (!beaker::sys_void::is_void_type(fn.get_return_type()))
    throw type_error(locs[1], "void return in non-void function");
  return build_fn.make_ret_stmt();
}

/// FIXME: Actually initialize the return value!
stmt& 
semantics::on_return_statement(expr& e, locations<2>)
{
  return build_fn.make_ret_stmt(e);
}


/// Returns a new block statement.
stmt&
semantics::on_block_statement(stmt_seq&& ss, locations<2> toks)
{
  return build_fn.make_block_stmt(std::move(ss));
}

} // namespace bpl

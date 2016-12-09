// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/sample/name.hpp>
#include <beaker/sample/type.hpp>
#include <beaker/sample/expr.hpp>
#include <beaker/sample/decl.hpp>
#include <beaker/sample/stmt.hpp>
#include <beaker/base/generation/generation.hpp>


namespace beaker {
namespace sample {

std::string
generate_name(generator& gen, const name& n)
{
  assert(false && "not implemented");
}

llvm::Type*
generate_type(generator& gen, const type& t)
{
  assert(false && "not implemented");
}

llvm::Value*
generate_expr(generator& gen, const expr& e)
{
  assert(false && "not implemented");
}

llvm::Value*
generate_decl(generator& gen, const decl& d)
{
  assert(false && "not implemented");
}

void
generate_stmt(generator& gen, const stmt& s)
{
  assert(false && "not implemented");
}

} // namespace sample
} // namespace beaker

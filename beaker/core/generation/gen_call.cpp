// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/core/type.hpp>
#include <beaker/core/expr.hpp>
#include <beaker/core/decl.hpp>
#include <beaker/base/generation/generation.hpp>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>


namespace beaker {
namespace core {

// Returns information for the called function.
//
// FIXME: This is now defined in two files. Factor this into a common facility.
static cg::fn_info&
get_function_info(generator& gen, const expr& e)
{
  cg::type type = gen.get_type(e.get_type());
  cg::fn_info* info = type.get_note<cg::fn_info>();
  assert(info != nullptr);
  return *info; 
}

// Generate a call expression.
//
// TODO: Adjust the arguments of the call to include a (possibly) materialized
// temporary. Also, how do we elide that materialization if this call is in
// an initialization context (is that us, or is it higher-level).
//
// TODO: Consider how exceptions should be handled. If the function can
// propagate C++ exceptions, then we'll need to establish (or have established)
// a landing pad. If the language has no exceptions, then call is sufficient.
//
// TODO: Handle non-noexcept functions.
cg::value
generate_call_expr(generator& gen, const call_expr& e)
{
  cg::fn_info& info = get_function_info(gen, e.get_function());

  llvm::Builder ir(gen.get_current_block());
  cg::value fn = generate(gen, e.get_function());
  
  // FIXME: We probably want cg::values for analysis.
  std::vector<llvm::Value*> args;
  for (const expr& a : e.get_arguments())
    args.push_back(generate(gen, a));
  
  return ir.CreateCall(fn, args);
}


} // namespace core
} // namespace beaker

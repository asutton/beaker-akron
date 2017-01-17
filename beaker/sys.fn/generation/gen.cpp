// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "gen.hpp"
#include "../type.hpp"
#include "../expr.hpp"
#include "../decl.hpp"
#include "../stmt.hpp"

#include <beaker/sys.void/type.hpp>
#include <beaker/util/symbol.hpp>

#include <llvm/IR/Function.h>
#include <llvm/IR/Argument.h>

#include <sstream>


namespace beaker {
namespace sys_fn {

/// Generate the return type of a function type.
static cg::type
generate_return_type(generator& gen, const type& t, std::vector<llvm::Type*>& parms)
{
  cg::type ret = generate(gen, t);
  if (is_object_type(t)) {
    // The function has a return value whose type is indirect, then adjust the 
    // function to accept that value as an argument and make the function void.
    if (ret.is_indirect()) {
      ret = llvm::PointerType::getUnqual(ret);
      parms.push_back(ret);
      ret = llvm::Type::getVoidTy(gen.get_context());
    }
  }
  return ret;
}

/// Generate a parameter type of a function type.
static cg::type
generate_parm_type(generator& gen, const type& t, std::vector<llvm::Type*>& parms)
{
  cg::type parm = generate(gen, t);
  if (is_object_type(t)) {
    // Passing by value and passing require similar adjustments. If the 
    // parameter type is indirect, adjust the type to be passed indirectly.
    if (parm.is_indirect())
      parm = llvm::PointerType::getUnqual(parm);
  }
  parms.push_back(parm);
  return parm;
}

/// Generates a function type.
static cg::type
generate_fn_type(generator& gen, const fn_type& t)
{
  std::vector<llvm::Type*> parms;
  
  // Generate the return type.
  cg::type ret = generate_return_type(gen, t.get_return_type(), parms);
  
  // Generate parameter types.
  for (const type& p : t.get_parameter_types())
    generate_parm_type(gen, p, parms);

  // Create and annotate the function type.
  return llvm::FunctionType::get(ret, parms, false);
}

// Generate a common type from t.
cg::type
gen_algo::operator()(generator& gen, const type& t) const
{
  assert(is<fn_type>(t));
  return generate_fn_type(gen, cast<fn_type>(t));
}


static cg::value generate_call_expr(generator& gen, const call_expr&);

static cg::value
generate_eq_expr(generator& gen, const eq_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());
  llvm::Builder ir(gen.get_current_block());
  return ir.CreateICmpEQ(v1, v2);
}

static cg::value
generate_ne_expr(generator& gen, const ne_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());
  llvm::Builder ir(gen.get_current_block());
  return ir.CreateICmpEQ(v1, v2);  
}

// Generate an LLVM instruction for the common expression e.
cg::value
gen_algo::operator()(generator& gen, const expr& e) const
{
  switch (e.get_kind()) {
#define def_expr(E) \
    case E ## _expr_kind: \
      return generate_## E ##_expr(gen, cast<E ## _expr>(e));
#include "../expr.def"
    default:
      break;
  }
  assert(false && "expression not supported");
}


static cg::value generate_fn_decl(generator&, const fn_decl&);

// Generate an LLVM declaration from the common declaration d.
//
// Note that parameter declarations can only appear in function declarations
// and are generated explicitly in generate_fn_decl.
cg::value
gen_algo::operator()(generator& gen, const decl& d) const
{
  assert(is<fn_decl>(d));
  return generate_fn_decl(gen, cast<fn_decl>(d));
}


/// Generate each of the statements in turn.
///
/// A block statement establishes a new declaration context.
static void
generate_block_stmt(generator& gen, const block_stmt& s)
{
  generator::decl_context_guard guard(gen);
  for (const stmt& s1 : s.get_statements())
    generate(gen, s1);
}

static inline void
generate_expr_stmt(generator& gen, const expr_stmt& s)
{
  generate(gen, s.get_expression());
}

static inline void
generate_decl_stmt(generator& gen, const decl_stmt& s)
{
  generate(gen, s.get_declaration());
}

// Generate code for the statement `return e`.
void
generate_ret_stmt(generator& gen, const ret_stmt& s)
{
  cg::value ret = gen.get_return_value();
  
  // Initialize the return value. No special cases are needed for void returns.
  generator::init_guard guard(gen, ret);
  generate(gen, s.get_return());
  
  // And jump to the exit block.
  llvm::Builder ir(gen.get_current_block());
  ir.CreateBr(gen.get_exit_block());
}

void
gen_algo::operator()(generator& gen, const stmt& s) const
{
  switch (s.get_kind()) {
#define def_stmt(S) \
    case S ## _stmt_kind: \
      return generate_## S ##_stmt(gen, cast<S ##_stmt>(s));
#include "../stmt.def"
    default:
      break;
  }
  assert(false && "not a sys_fn statement");
}

} // namespace sys_fn
} // namespace beaker


#include "gen_call.cpp"
#include "gen_fn.cpp"

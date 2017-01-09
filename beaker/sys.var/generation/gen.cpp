// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "gen.hpp"
#include "../type.hpp"
#include "../expr.hpp"
#include "../decl.hpp"
#include <beaker/util/symbol.hpp>

#include <sstream>


namespace beaker {
namespace sys_var {


/// Returns an LLVM pointer type.
static inline cg::type
generate_ref_type(generator& gen, const ref_type& t)
{
  llvm::Type* type = generate(gen, t.get_object_type());
  return llvm::PointerType::getUnqual(type);
}

// Generate a common type from t.
cg::type
gen_algo::operator()(generator& gen, const type& t) const
{
  assert(is<ref_type>(t));
  return generate_ref_type(gen, cast<ref_type>(t));
}

// Generates the the address of a declaration.
static cg::value
generate_ref_expr(generator& gen, const ref_expr& e)
{
  llvm::Builder ir(gen.get_current_block());
  return gen.get_value(e.get_declaration());
}

/// Generates a load of a reference. 
static cg::value
generate_deref_expr(generator& gen, const deref_expr& e)
{
  assert(is_reference_type(e.get_source().get_type()));
  cg::value ref = generate(gen, e.get_source());
  cg::type type = generate(gen, e.get_type());
  if (type.is_direct()) {
    // We only need a load when the result type of the expression direct. 
    // Otherwise, we are simply using the reference as the value.
    llvm::Builder ir(gen.get_current_block());
    ref = ir.CreateLoad(ref);
  }
  return ref;
}

/// Assigns the value of the LHS to the object referenced by the RHS.
static cg::value
generate_assign_expr(generator& gen, const assign_expr& e)
{
  cg::value ptr = generate(gen, e.get_lhs());
  cg::value val = generate(gen, e.get_rhs());
  cg::type type = generate(gen, e.get_rhs().get_type());
  llvm::Builder ir(gen.get_current_block());
  if (type.is_direct()) {
    // Assigning direct types is easy. Just store the value at the address.
    ir.CreateStore(val, ptr);
  }
  else {
    // Assigning indirect types requires a memcpy.
    //
    // TODO: See notes in generate_copy_init() on mem copies. We probably
    // want to know the size and alignment of types ahead of time (although
    // this works for now).
    llvm::Constant* size = llvm::ConstantExpr::getSizeOf(type);
    ir.CreateMemCpy(ptr, val, size, 0);
  }
  return ptr;
}


static cg::value generate_nop_init(generator& gen, const nop_init&);
static cg::value generate_zero_init(generator& gen, const zero_init&);
static cg::value generate_copy_init(generator& gen, const copy_init&);
static cg::value generate_ref_init(generator& gen, const ref_init&);

// Generate an LLVM instruction for the common expression e.
cg::value
gen_algo::operator()(generator& gen, const expr& e) const
{
  switch (e.get_kind()) {
#define def_expr(E) \
    case E ## _expr_kind: \
      return generate_## E ##_expr(gen, cast<E ## _expr>(e));
#define def_init(E) \
    case E ## _init_kind: \
      return generate_## E ##_init(gen, cast<E ## _init>(e));
#include "../expr.def"
    default:
      break;
  }
  assert(false && "expression not supported");
}


static cg::value generate_var_decl(generator&, const var_decl&);

// Generate an LLVM declaration from the common declaration d.
//
// Note that parameter declarations can only appear in function declarations
// and are generated explicitly in generate_fn_decl.
cg::value
gen_algo::operator()(generator& gen, const decl& d) const
{
  assert(is<var_decl>(d));
  return generate_var_decl(gen, cast<var_decl>(d));
}

} // namespace sys_var
} // namespace beaker

#include "gen_init.cpp"
#include "gen_var.cpp"

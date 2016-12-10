// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/core/generation/gen.hpp>
#include <beaker/core/name.hpp>
#include <beaker/core/type.hpp>
#include <beaker/core/expr.hpp>
#include <beaker/core/decl.hpp>
#include <beaker/core/stmt.hpp>
#include <beaker/util/symbol.hpp>

#include <sstream>


namespace beaker {
namespace core {

/// The symbol for a basic name is that name's spelling.
static inline std::string
generate_basic_name(generator& gen, const basic_name& n)
{
  return n.get_symbol().get_spelling();
}

/// The symbol for an internal name has the form "_B_id_N" where N is the 
/// unique identifier.
static inline std::string
generate_internal_name(generator& gen, const internal_name& n)
{
  std::stringstream ss;
  ss << "_B_id_" << cast<internal_name>(n).get_id();
  return ss.str(); 
}

/// \todo Replace this with name mangling.
std::string
gen_algo::operator()(generator& gen, const name& n) const
{
  if (n.get_kind() == basic_name_kind)
    return generate_basic_name(gen, cast<basic_name>(n));
  else if (n.get_kind() == internal_name_kind)
    return generate_internal_name(gen, cast<internal_name>(n));
  else
    assert(false && "not a core name");
}

/// Returns llvm void type.
static inline cg::type
generate_void_type(generator& gen, const void_type& t)
{
  return llvm::Type::getVoidTy(gen.get_context());
}

/// Returns an llvm pointer type.
static inline cg::type
generate_ref_type(generator& gen, const ref_type& t)
{
  llvm::Type* elem = generate(gen, t.get_object_type());
  return llvm::PointerType::getUnqual(elem);
}

/// Generates a function type.
static cg::type
generate_fn_type(generator& gen, const fn_type& t)
{
  std::vector<cg::parm_info> pinfo;
  std::vector<llvm::Type*> parms;
  
  // If the function returns an indirect type by value, then adjust the return 
  // value so it's passed as an indirect parameter. The actual return value
  // is adjusted to be void.
  const type& rt = t.get_return_type();
  cg::type ret = generate(gen, rt);
  if (is_object_type(rt) && ret.is_indirect()) {
    parms.push_back(ret);
    pinfo.emplace_back(cg::indirect_parm, true, false);
    ret = llvm::Type::getVoidTy(gen.get_context());
  }
  
  // Generate parameter types. If a parameter type is indirect, then make
  // make it a pointer, and pass by value.
  for (const type& pt : t.get_parameter_types()) {
    cg::type parm = generate(gen, pt);
    if (is_object_type(pt) && parm.is_indirect()) {
      parm = llvm::PointerType::getUnqual(parm);
      pinfo.emplace_back(cg::indirect_parm, false, true);
    }
    else {
      pinfo.emplace_back(cg::direct_parm, false, false);
    }
    parms.push_back(parm);
  }

  // Create and annotate the function type.
  cg::type fn = llvm::FunctionType::get(ret, parms, t.is_variadic());
  gen.annotate<cg::fn_info>(fn, std::move(pinfo));
  return fn;
}

// Generate a common type from t.
cg::type
gen_algo::operator()(generator& gen, const type& t) const
{
  switch (t.get_kind()) {
    case void_type_kind:
      return generate_void_type(gen, cast<void_type>(t));
    case ref_type_kind:
      return generate_ref_type(gen, cast<ref_type>(t));
    case fn_type_kind:
      return generate_fn_type(gen, cast<fn_type>(t));
    default:
      break;
  }
  assert(false && "not a core type");
}

/// Generates no code and returns nullptr.
///
/// Because this does not return a value, the result cannot be used as an
/// operand to other instructions.
static inline cg::value
generate_nop_expr(generator& gen, const nop_expr& e)
{
  return nullptr;
}

/// Generates the evaluation of the operand and returns nullptr.
///
/// Because this does not return a value, the result cannot be used as an
/// operand to other instructions.
static inline cg::value
generate_void_expr(generator& gen, const void_expr& e)
{
  generate(gen, e.get_operand());
  return nullptr;
}

// Generates the address of the referenced declaration.
static cg::value
generate_ref_expr(generator& gen, const ref_expr& e)
{
  return gen.get_value(e.get_declaration());
}

static cg::value
generate_temp_expr(generator& gen, const temp_expr& e)
{
  assert(false && "not implemented");
}

/// Generates the initialization of an object by a value.
///
/// Returns the address of the initialized object.
static cg::value
generate_copy_expr(generator& gen, const copy_expr& e)
{
  llvm::Builder ir(gen.get_current_block());
  cg::value ptr = generate(gen, e.get_lhs());
  cg::value val = generate(gen, e.get_rhs());
  ir.CreateStore(val, ptr);
  return ptr;
}


/// Generates a load of a reference.
static cg::value
generate_deref_expr(generator& gen, const deref_expr& e)
{
  llvm::Builder ir(gen.get_current_block());
  cg::value v = generate(gen, e.get_source());
  return ir.CreateLoad(v);
}

static cg::value generate_call_expr(generator& gen, const call_expr&);
static cg::value generate_zero_init(generator& gen, const zero_init&);
static cg::value generate_copy_init(generator& gen, const copy_init&);
static cg::value generate_call_init(generator& gen, const call_init&);

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
static cg::value generate_fn_decl(generator&, const fn_decl&);

// Generate an LLVM declaration from the common declaration d.
cg::value
gen_algo::operator()(generator& gen, const decl& d) const
{
  switch (d.get_kind()) {
    case var_decl_kind:
    return generate_var_decl(gen, cast<var_decl>(d));
    case ref_decl_kind:
    case const_decl_kind:
      break;
    case fn_decl_kind:
      return generate_fn_decl(gen, cast<fn_decl>(d));
    default:
      break;
  }
  assert(false && "declaration not supported");
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
  // Initialize the return value.
  generator::init_guard guard(gen, gen.get_return_value());
  generate(gen, s.get_return());

  // Jump to the exit block.
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
  assert(false && "not a core statement");
}

} // namespace core
} // namespace beaker

#include "gen_init.cpp"
#include "gen_call.cpp"
#include "gen_var.cpp"
#include "gen_fn.cpp"

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


/// Returns LLVM void type.
static inline cg::type
generate_void_type(generator& gen, const void_type& t)
{
  return llvm::Type::getVoidTy(gen.get_context());
}

/// Returns an LLVM pointer type.
static inline cg::type
generate_ref_type(generator& gen, const ref_type& t)
{
  llvm::Type* type = generate(gen, t.get_object_type());
  return llvm::PointerType::getUnqual(type);
}

/// An input parameter of indirect type is a pointer. An input parameter
/// of direct type is simply that type.
cg::type
generate_in_type(generator& gen, const in_type& t)
{
  cg::type type = generate(gen, t.get_object_type());
  if (type.is_direct())
    return type;
  return llvm::PointerType::getUnqual(type);
}

// An output parameter is always a pointer
cg::type
generate_out_type(generator& gen, const out_type& t)
{
  cg::type type = generate(gen, t.get_object_type());
  return llvm::PointerType::getUnqual(type);
}

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

static cg::type
generate_parm_type(generator& gen, const type& t, std::vector<llvm::Type*>& parms)
{
  cg::type parm = generate(gen, t);
  if (is_object_type(t) || is<in_type>(t)) {
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
  return llvm::FunctionType::get(ret, parms, t.is_variadic());
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
    case in_type_kind:
      return generate_in_type(gen, cast<in_type>(t));
    case out_type_kind:
      return generate_out_type(gen, cast<out_type>(t));
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
  cg::value ret = gen.get_return_value();
  
  // Initialize the return value. Note that this will be null if the return
  // value has direct type.
  generator::init_guard guard(gen, ret);
  cg::value val = generate(gen, s.get_return());
  
  llvm::Builder ir(gen.get_current_block());
  if (!ret) {
    // If the return value has direct type, then we didn't store a value,
    // and we can simply return the value.
    ir.CreateRet(val);
  }
  else {
    // Otherwise, we stored the indirect return value, and we should jump to 
    // the exit block, which will return void.
    ir.CreateBr(gen.get_exit_block());
  }
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

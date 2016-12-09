// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/data/type.hpp>
#include <beaker/data/expr.hpp>
#include <beaker/core/type.hpp>
#include <beaker/base/generation/generation.hpp>


namespace beaker {
namespace data {

std::string
generate_name(generator& gen, const name& n)
{
  assert(false && "not defined");
}

// Generates a literal record type from t.
//
// FIXME: Currently all tuples are emitted as indirect types. This should
// really depend on its size.
//
// TODO: It might be worthwhile to cache the type mapping so we don't have 
// to recompute it each time.
static cg::type
generate_tuple_type(generator& gen, const tuple_type& t)
{
  std::vector<llvm::Type*> types;
  for (const type& t1 : t.get_element_types())
    types.push_back(generate(gen, t1));
  return {llvm::StructType::get(gen.get_context(), types), false};
}

// Generates an array type from t.
//
// FIXME: All arrays are currently emitted as indirect types. This should 
// really depend on its size.
cg::type
generate_array_type(generator& gen, const array_type& t)
{
  cg::type type = generate(gen, t.get_element_type());
  return {llvm::ArrayType::get(type, t.get_extent()), false};
}

// Generates a pointer type from t.
static cg::type
generate_seq_type(generator& gen, const seq_type& t)
{
  cg::type type = generate(gen, t.get_element_type());
  return llvm::PointerType::getUnqual(type);
}

cg::type
generate_type(generator& gen, const type& t)
{
  switch (t.get_kind()) {
    case tuple_type_kind:
      return generate_tuple_type(gen, cast<tuple_type>(t));
    case array_type_kind:
      return generate_array_type(gen, cast<array_type>(t));
    case seq_type_kind:
      return generate_seq_type(gen, cast<seq_type>(t));
    default:
      break;
  }
  assert(false && "not a data type");
}


// Generate a tuple or array register.
template<typename T>
static cg::value
generate_aggregate(generator& gen, const T& e)
{
  cg::type type = generate(gen, e.get_type());
  cg::value agg = llvm::UndefValue::get(type);

  // Iteratively construct the aggregate value.
  llvm::IRBuilder<> ir(gen.get_current_block());
  int n = 0;
  for (const expr& e1 : e.get_elements()) {
    cg::value elem = generate(gen, e1);
    agg = ir.CreateInsertValue(agg, elem, n++);
  }
  return agg; 
}

// Generate a tuple object from e.
//
// TODO: If e is a tuple literal, then we can generate a struct constant.
cg::value
generate_tuple_expr(generator& gen, const tuple_expr& e)
{
  return generate_aggregate(gen, e);
}

// Generate an array object from e.
//
// TODO: If e is an array literal, then we can generate an array constant.
cg::value
generate_array_expr(generator& gen, const array_expr& e)
{
  return generate_aggregate(gen, e);
}

// Generate an element access to a tuple or array reference.
//
// Because the expression is a reference, the sub-object is accessed by 
// it's address.
cg::value
generate_elem_ref(generator& gen, const elem_expr& e)
{
  assert(is<core::ref_type>(e.get_type()));
  cg::value tuple = generate(gen, e.get_tuple());
  llvm::IRBuilder<> ir(gen.get_current_block());
  return ir.CreateConstGEP1_32(tuple, e.get_index());
}

// Generate an element access to a tuple or array object.
cg::value
generate_elem_obj(generator& gen, const elem_expr& e)
{
  cg::value tuple = generate(gen, e.get_tuple());
  llvm::IRBuilder<> ir(gen.get_current_block());
  return ir.CreateExtractValue(tuple, e.get_index());
}

// Generate an element access to a tuple or array.
//
// The behavior of this operation depends on whether the aggregate is 
// stored (i.e., accessed by reference) or not.
cg::value
generate_elem_expr(generator& gen, const elem_expr& e)
{
  const type& t = e.get_type();
  if (is<core::ref_type>(t))
    return generate_elem_ref(gen, e);
  else
    return generate_elem_obj(gen, e);
}

// Generate an element access to an array or sequence reference.
cg::value
generate_index_expr(generator& gen, const index_expr& e)
{
  const type& t = e.get_type();
  assert(is<core::ref_type>(t) && "not implemented");
  cg::value array = generate(gen, e.get_array());
  cg::value index = generate(gen, e.get_index());
  llvm::IRBuilder<> ir(gen.get_current_block());
  return ir.CreateGEP(array, index);
}

// Generate an expression from e.
cg::value
generate_expr(generator& gen, const expr& e)
{
  switch (e.get_kind()) {
    case tuple_expr_kind:
      return generate_tuple_expr(gen, cast<tuple_expr>(e));
    case array_expr_kind:
      return generate_array_expr(gen, cast<array_expr>(e));
    case elem_expr_kind:
      return generate_elem_expr(gen, cast<elem_expr>(e));
    case index_expr_kind:
      return generate_index_expr(gen, cast<index_expr>(e));
    default:
      break;
  }
  assert(false && "not a data expression");
}

cg::value
generate_decl(generator& gen, const decl& s)
{
  assert(false && "not defined");
}

void
generate_stmt(generator& gen, const stmt& s)
{
  assert(false && "not defined");
}


} // namespace data
} // namespace beaker


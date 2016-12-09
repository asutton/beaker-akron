// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "generation.hpp"
#include "type.hpp"
#include "expression.hpp"

#include <llvm/IR/IRBuilder.h>

#include <iostream>


namespace beaker {
namespace data {

// Generates a literal record type from t.
//
// TODO: It might be worthwhile to cache the type mapping so we don't have 
// to recompute it each time.
static llvm::Type*
generate_tuple_type(generator& gen, const tuple_type& t)
{
  std::vector<llvm::Type*> types;
  for (const type& t1 : t.get_element_types())
    types.push_back(generate(gen, t1));
  return llvm::StructType::get(gen.get_context(), types);
}

// Generates an array type from t.
llvm::Type*
generate_array_type(generator& gen, const array_type& t)
{
  llvm::Type* type = generate(gen, t.get_element_type());
  return llvm::ArrayType::get(type, t.get_extent());
}

// Generates a pointer type from t.
static llvm::Type*
generate_seq_type(generator& gen, const seq_type& t)
{
  llvm::Type* type = generate(gen, t.get_element_type());
  return llvm::PointerType::getUnqual(type);
}

llvm::Type*
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
static llvm::Value*
generate_aggregate(generator& gen, const T& e)
{
  llvm::Type* type = generate(gen, e.get_type());
  llvm::Value* agg = llvm::UndefValue::get(type);

  // Iteratively construct the aggregate value.
  llvm::IRBuilder<> ir(gen.get_current_block());
  int n = 0;
  for (const expr& e1 : e.get_elements()) {
    llvm::Value* elem = generate(gen, e1);
    agg = ir.CreateInsertValue(agg, elem, n++);
  }
  return agg; 
}

// Generate a tuple object from e.
//
// TODO: If e is a tuple literal, then we can generate a struct constant.
llvm::Value*
generate_tuple_expr(generator& gen, const tuple_expr& e)
{
  return generate_aggregate(gen, e);
}

// Generate an array object from e.
//
// TODO: If e is an array literal, then we can generate an array constant.
llvm::Value*
generate_array_expr(generator& gen, const array_expr& e)
{
  return generate_aggregate(gen, e);
}

// Generate an element access to a tuple or array reference.
//
// Because the expression is a reference, the sub-object is accessed by 
// it's address.
llvm::Value*
generate_elem_ref(generator& gen, const elem_expr& e)
{
  assert(is<ref_type>(e.get_type()));
  llvm::Value* tuple = generate(gen, e.get_tuple());
  llvm::IRBuilder<> ir(gen.get_current_block());
  return ir.CreateConstGEP1_32(tuple, e.get_index());
}

// Generate an element access to a tuple or array object.
llvm::Value*
generate_elem_obj(generator& gen, const elem_expr& e)
{
  llvm::Value* tuple = generate(gen, e.get_tuple());
  llvm::IRBuilder<> ir(gen.get_current_block());
  return ir.CreateExtractValue(tuple, e.get_index());
}

// Generate an element access to a tuple or array.
//
// The behavior of this operation depends on whether the aggregate is 
// stored (i.e., accessed by reference) or not.
llvm::Value*
generate_elem_expr(generator& gen, const elem_expr& e)
{
  const type& t = e.get_type();
  if (is<ref_type>(t))
    return generate_elem_ref(gen, e);
  else
    return generate_elem_obj(gen, e);
}

// Generate an element access to an array or sequence reference.
llvm::Value*
generate_index_expr(generator& gen, const index_expr& e)
{
  const type& t = e.get_type();
  assert(is<ref_type>(t) && "not implemented");
  llvm::Value* array = generate(gen, e.get_array());
  llvm::Value* index = generate(gen, e.get_index());
  llvm::IRBuilder<> ir(gen.get_current_block());
  return ir.CreateGEP(array, index);
}

// Generate an expression from e.
llvm::Value*
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

} // namespace data
} // namespace beaker


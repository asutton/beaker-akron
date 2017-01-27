// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "gen.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_tuple {

/// Returns an annamed struct type. Note that tuples are always passed
/// indirectly.
cg::type
generate_tuple_type(generator& gen, const tuple_type& t)
{
  std::vector<llvm::Type*> types;
  for (const type& elem : t.get_element_types())
    types.push_back(generate(gen, elem));
  llvm::Context& cxt = gen.get_context();
  return {llvm::StructType::create(cxt, types, "__tuple"), true};
}

cg::type
gen_algo::operator()(generator& gen, const type& t) const
{
  return generate_tuple_type(gen, cast<tuple_type>(t));
}


/// The expression `{e1, e2, ..., en}` results in an object containing
/// the values of the given expressions.  Each subexpression is evaluated
/// in lexical order.
cg::value
generate_tuple_expr(generator& gen, const tuple_expr& e)
{
  assert(is<tuple_type>(e.get_type()));

  // Genrate the object that will store those values.
  cg::value obj = gen.make_alloca(generate(gen, e.get_type()));

  // Generate the subexpressions in the order in which they appear.
  const expr_seq& args = e.get_elements();
  for (int i = 0; i < args.size(); ++i) {
      // Get the address of the sub-object being initialized.
      llvm::Builder ir(gen.get_current_block());
      llvm::Value *ix[] {
        ir.getInt32(0),
        ir.getInt32(i)
      };
      cg::value sub = ir.CreateInBoundsGEP(obj, ix);

      // Initialize the sub-object.
      generator::init_guard guard(gen, sub);
      generate(gen, args[i]);
  }
  return obj;
}

// Returns the projection of the nth term from the object denoted by `e`. Since
// tuples are always stored, the generated object will always be a reference.
cg::value
generate_proj_expr(generator& gen, const proj_expr& e)
{
  const type& t = e.get_object().get_type();
  const tuple_type& tt = cast<tuple_type>(t);
  assert(e.get_element() < tt.get_element_types().size());

  cg::value obj = generate(gen, e.get_object());
  
  // Get the nth sub-object.
  llvm::Builder ir(gen.get_current_block());
  llvm::Value *ix[] {
    ir.getInt32(0),
    ir.getInt32(e.get_element())
  };
  cg::value sub = ir.CreateInBoundsGEP(obj, ix);
  
  if (is_reference_expression(e.get_object())) {
    // When e is a reference, return a reference to the subobject.
    return sub;
  }
  else {
    // Otherwise, return the value of the object. This also depends on whether
    // or not the type of the nth element is indirect or not.
    const type& et = tt.get_element_type(e.get_element());
    cg::type elem = generate(gen, et);
    if (elem.is_indirect())
      return sub;
    else
      return ir.CreateLoad(sub);
  }
}

cg::value
gen_algo::operator()(generator& gen, const expr& e) const
{
  switch (e.get_kind()) {
    case tuple_expr_kind:
      return generate_tuple_expr(gen, cast<tuple_expr>(e));
    case proj_expr_kind:
      return generate_proj_expr(gen, cast<proj_expr>(e));
  }
  assert(false && "not a tuple expression");
}

} // namespace sys_tuple
} // namespace beaker


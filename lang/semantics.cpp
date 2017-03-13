// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "semantics.hpp"

#include <beaker/base/printing/print.hpp>

#include <beaker/sys.void/ast.hpp>
#include <beaker/sys.bool/ast.hpp>
#include <beaker/sys.int/ast.hpp>
#include <beaker/sys.name/ast.hpp>
#include <beaker/sys.var/ast.hpp>
#include <beaker/sys.fn/ast.hpp>

#include <iostream>
#include <stdexcept>


namespace bpl {

semantics::semantics(module& m)
  : mod(m),
    build_void(m.get_void_builder()),
    build_bool(m.get_bool_builder()),
    build_int(m.get_int_builder()),
    build_name(m.get_name_builder()),
    build_var(m.get_var_builder()),
    build_fn(m.get_fn_builder())
{ }


/// Returns the current funtion. Behavior is undefined if not in a function.
///
/// \todo If we allow scoped declarations within a function definition, should
/// we walk up to the function or not?
const beaker::sys_fn::fn_decl& 
semantics::current_function() const
{
  return cast<beaker::sys_fn::fn_decl>(current_context());
}

/// Returns the current funtion. Behavior is undefined if not in a function.
beaker::sys_fn::fn_decl& 
semantics::current_function()
{
  return cast<beaker::sys_fn::fn_decl>(current_context());
}


/// Returns the default scope kind corresponding to the given declaration.
static int
get_scope_kind(const decl& d)
{
  switch (d.get_kind()) {
    case beaker::module_decl_kind: 
      return module_scope;
    case beaker::sys_fn::fn_decl_kind: 
      return function_scope;
    default: 
      break;
  }
  assert(false && "no scope associated with declaration");
};


/// Enter a declarative region corresponding to declaration `d`.
declarative_region::declarative_region(semantics& s, decl& d)
  : sema(s), prev_dc(sema.cur_cxt)
{
  // Set the current declaration context.
  sema.cur_cxt = dc(d);

  // Enter the corresponding scope.
  sema.enter_scope(get_scope_kind(d));
}

/// Enter a declarative region that is not explicitly associated with a 
/// declaration context. Examples are function prototype scope amd block
/// scope.
declarative_region::declarative_region(semantics& s, int k)
  : sema(s), prev_dc(sema.cur_cxt)
{
  sema.enter_scope(k);
}

/// Restore the previous declarative region and the scope stack.
declarative_region::~declarative_region()
{
  sema.cur_cxt = prev_dc;
  sema.leave_scope();
}


} // namespace bpl

#include "sema_name.cpp"
#include "sema_type.cpp"
#include "sema_expr.cpp"
#include "sema_decl.cpp"
#include "sema_stmt.cpp"

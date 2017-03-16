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


/// Returns true if the converted expressions have type bool.
bool
is_boolean(const conv_pair& p)
{
  return sys_bool::is_boolean_type(p.get_type());
}

/// Returns true if the converted expressions have integral type.
bool
is_integral(const conv_pair& p)
{
  return sys_int::is_integral_type(p.get_type());
}



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
const sys_fn::fn_decl& 
semantics::current_function() const
{
  return cast<sys_fn::fn_decl>(current_context());
}

/// Returns the current funtion. Behavior is undefined if not in a function.
sys_fn::fn_decl& 
semantics::current_function()
{
  return cast<sys_fn::fn_decl>(current_context());
}


/// Returns true if `e` has type `bool`.
static inline bool
is_boolean(const expr& e)
{
  return beaker::sys_bool::is_boolean_expression(e);
}

/// Returns true if both `e1` and `e2` have type `bool`.
static inline bool
are_boolean(const expr& e1, const expr& e2)
{
  return is_boolean(e1) && is_boolean(e2);
}

/// Returns true if `e` has type `int`.
static inline bool
is_integral(const expr& e)
{
  return beaker::sys_int::is_integral_expression(e);
}

/// Returns true if both `e1` and `e2` have type `int`.
static inline bool
are_integral(const expr& e1, const expr& e2)
{
  return is_integral(e1) && is_integral(e2);
}

/// Returns true if both `e1` and `e2` have the same type.
static inline bool
same_typed(const expr& e1, const expr& e2)
{
  return equal(e1.get_type(), e2.get_type());
}

/// Check that `e1` has type bool.
expr&
semantics::check_bool(expr& e1)
{
  if (!is_boolean(e1))
    throw type_error(e1.get_location(), "operand does not have type 'bool'");
  return e1;
}

/// Check that `e1` and `e2` both have type bool.
void
semantics::check_bool(expr& e1, expr& e2)
{
  check_bool(e1);
  check_bool(e2);
}

/// Check that `e1` has type int.
expr&
semantics::check_int(expr& e1)
{
  if (!is_integral(e1))
    throw type_error(e1.get_location(), "operand does not have type 'int'");
  return e1;
}

/// Check that `e1` and `e2` both have type int.
void
semantics::check_int(expr& e1, expr& e2)
{
  check_int(e1);
  check_int(e2);
}

/// Check that `e1` and `e2` have the same type.
void
semantics::check_same(expr& e1, expr& e2)
{
  if (!same_typed(e1, e2))
    throw type_error(e1.get_location(), "operands have different types");
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
#include "sema_conv.cpp"
#include "sema_init.cpp"

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VAR_EXPR_HPP
#define BEAKER_SYS_VAR_EXPR_HPP

#include <beaker/base/expr.hpp>


namespace beaker {

struct name;
struct decl;
struct named_decl;
struct typed_decl;

namespace sys_var {

enum 
{
  first_expr_kind = sys_var_lang_block,
#define def_expr(NS, E) E ## _expr_kind,
#define def_init(NS, E) E ## _init_kind,
#include "expr.def"
  last_expr_kind
};


/// Represents the use of a declared variable or function as a value.
///
/// A reference to a variable has reference type. Note that reference variables
/// already have reference type.
///
/// A reference to a function has function type (functions are always passed
/// indirectly).
///
/// The value of a reference expression is the address of the referred to 
/// object. Note that using the value of a reference requires a dereference
/// operation.
///
/// TODO: Extend this class so that it can support unresolved declarations
/// as well. It should have a union combining a name, a declaration, and
/// possibly an overload set (when we get those).
struct ref_expr : expr 
{
  static constexpr int node_kind = ref_expr_kind;

  ref_expr(type&, decl&);

  const typed_decl& get_declaration() const;
  typed_decl& get_declaration();

  const name& get_name() const;
  name& get_name();

  decl* decl_;
};

inline
ref_expr::ref_expr(type& t, decl& d)
  : expr(node_kind, t), decl_(&d)
{ }


/// Represents the the expression `val(e)`, which "converts" a reference into
/// an value. This generally corresponds to loading the stored value of an 
/// object.
///
/// The type of e shall be a reference type `t&`. The type of the expression
/// is the object type `t`. The value of the expression is the object referred
/// to by the expression `e`.
struct val_expr : unary_expr_impl<val_expr_kind>
{
  using unary_expr_impl<val_expr_kind>::unary_expr_impl;

  const expr& get_source() const;
  expr& get_source();
};

/// Returns the source operand of the conversion.
inline const expr& val_expr::get_source() const { return get_first(); }

/// Returns the source operand of the conversion.
inline expr& val_expr::get_source() { return get_first(); }


/// Represents the expression `e1 = e2`.
///
/// Copies the value of `e2` in the object denoted by `e1`. The type of `e1`
/// shall be a reference to the type of `e2`, and `e2` shall be scalar. The 
/// result of the expression is the reference `e1`. 
///
/// This is a copy constructor for scalar types.
struct assign_expr : binary_expr_impl<assign_expr_kind>
{
  using binary_expr_impl<assign_expr_kind>::binary_expr_impl;
};


// -------------------------------------------------------------------------- //
// Initializers

/// Represents the trivial initialization of an object. Trivial initialization
/// leaves an object partially formed (i.e., with indeterminate value).
struct nop_init : nullary_init_impl<nop_init_kind>
{
  using nullary_init_impl<nop_init_kind>::nullary_init_impl;
};


/// Represents the zero initialization of an object.
///
/// Note that we can produce a reasonable zero value for all types.
struct zero_init : nullary_init_impl<zero_init_kind>
{
  using nullary_init_impl<zero_init_kind>::nullary_init_impl;
};


/// Represents the initialization of an object by a value.
///
/// Copy initialization transfers the contents in the initializing expression
/// to the initialized object. The type of the expression shall be the same as 
/// that of the object being initialized.
struct copy_init : unary_init_impl<copy_init_kind>
{
  using unary_init_impl<copy_init_kind>::unary_init_impl;
};


/// Represents the initialization of a reference by an object.
///
/// Reference initialization binds the initialized reference to the initializing
/// expression (i.e., the address computed by that expression). The type of the 
/// expression shall be the same as that of the object being initialized.
struct ref_init : unary_init_impl<ref_init_kind>
{
  using unary_init_impl<ref_init_kind>::unary_init_impl;
};

} // namespace sys_var
} // namespace beaker


#endif

// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_CORE_EXPR_HPP
#define BEAKER_CORE_EXPR_HPP

#include <beaker/base/expr.hpp>


namespace beaker {

struct name;
struct decl;
struct named_decl;
struct typed_decl;

namespace core {

enum 
{
  first_expr_kind = core_lang_block,
#define def_expr(e) e ## _expr_kind,
#define def_init(e) e ## _init_kind,
#include "expr.def"
  last_expr_kind
};


/// The expression `nop`.
///
/// The type of the expression is void. It has no effect.
struct nop_expr : generic_nullary_expr<nop_expr_kind>
{
  using generic_nullary_expr<nop_expr_kind>::generic_nullary_expr;
};


/// The expression `void(e)`.
///
/// The type of the expression is `void`. The value of `e` is discarded.
struct void_expr : generic_unary_expr<void_expr_kind>
{
  using generic_unary_expr<void_expr_kind>::generic_unary_expr;
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


/// Represents the expression `e1 = e2`.
///
/// Copies the value of `e2` in the object denoted by `e1`. The type of `e1`
/// shall be a reference to the type of `e2`, and `e2` shall be scalar. The 
/// result of the expression is the reference `e1`. 
///
/// This is a copy constructor for scalar types.
struct assign_expr : generic_binary_expr<assign_expr_kind>
{
  using generic_binary_expr<assign_expr_kind>::generic_binary_expr;
};


/// Represents the expression `temp(t)`.
///
/// This materializes a temporary of type `t`, yielding a reference to that
/// object.
///
/// TODO: Do I really want this expression, or can I deal with materialization
/// inside codegen.
struct temp_expr : generic_nullary_expr<temp_expr_kind>
{
  temp_expr(type&, type&);

  const type& get_soure_type() const;
  type& get_soure_type();

  type* src_;
};

inline 
temp_expr::temp_expr(type& t1, type& t2)
  : generic_nullary_expr<temp_expr_kind>(t1), src_(&t2)
{ }

/// Returns the source type of the expression.
inline const type& temp_expr::get_soure_type() const { return *src_; }

/// Returns the source type of the expression.
inline type& temp_expr::get_soure_type() { return *src_; }


// Represents the evaluation of a function, given arguments.
struct call_expr : expr 
{
  static constexpr int node_kind = call_expr_kind;

  call_expr(type&, expr&, const expr_seq&);
  call_expr(type&, expr&, expr_seq&&);

  const expr& get_function() const;
  expr& get_function();

  const expr_seq& get_arguments() const;
  expr_seq& get_arguments();

  expr* fn_;
  expr_seq args_;
};

inline
call_expr::call_expr(type& t, expr& f, const expr_seq& a)
  : expr(node_kind, t), fn_(&f), args_(a)
{ }

inline
call_expr::call_expr(type& t, expr& f, expr_seq&& a)
  : expr(node_kind, t), fn_(&f), args_(std::move(a))
{ }

// Returns the called function.
inline const expr& call_expr::get_function() const { return *fn_; }

// Returns the called function.
inline expr& call_expr::get_function() { return *fn_; }

// Returns the sequence of call arguments.
inline const expr_seq& call_expr::get_arguments() const { return args_; }

// Returns the sequence of call arguments.
inline expr_seq& call_expr::get_arguments() { return args_; }


/// Represents the the expression `deref(e)`.
///
/// A dereference expression converts a reference into an object. This 
/// generally corresponds to loading the stored value of an object.
///
/// The type of e shall be a reference type `ref t`. The type of the expression
/// is the object type `t`. The value of the expression is the object referred
/// to by the expression `e`.
///
/// \todo This nominally represents an lvalue to rvalue conversion, but
/// those aren't typically applied to class objects. We usually end up
/// invoking a constructor to produce a copy... right? Investigate.
struct deref_expr : conversion_expr<deref_expr_kind>
{
  using conversion_expr<deref_expr_kind>::conversion_expr;
};


// -------------------------------------------------------------------------- //
// Initializers

/// Represents the trivial initialization of an object. Trivial initialization
/// leaves an object partially formed (i.e., with indeterminate value).
struct nop_init : generic_nullary_init<nop_init_kind>
{
  using generic_nullary_init<nop_init_kind>::generic_nullary_init;
};


/// Represents the zero initialization of an object.
///
/// Note that we can produce a reasonable zero value for all types.
struct zero_init : generic_nullary_init<zero_init_kind>
{
  using generic_nullary_init<zero_init_kind>::generic_nullary_init;
};


/// Represents the initialization of an object by a value.
///
/// Copy initialization transfers the contents in the initializing expression
/// to the initialized object. The type of the expression shall be the same as 
/// that of the object being initialized.
struct copy_init : generic_unary_init<copy_init_kind>
{
  using generic_unary_init<copy_init_kind>::generic_unary_init;
};


/// Represents the initialization of a reference by an object.
///
/// Reference initialization binds the initialized reference to the initializing
/// expression (i.e., the address computed by that expression). The type of the 
/// expression shall be the same as that of the object being initialized.
struct ref_init : generic_unary_init<ref_init_kind>
{
  using generic_unary_init<ref_init_kind>::generic_unary_init;
};


/// Represents the initialization of an object by a function.
///
/// This is similar to a call expression except that the function is given 
/// directly, and the argument list is "incomplete". The first argument is 
/// implicitly the object being initialized.
///
/// FIXME: Do we need this in core? Also, this should follow the pattern
/// of the other initializers.
struct call_init : expr 
{
  static constexpr int node_kind = call_init_kind;

  call_init(type&, decl&, const expr_seq&);
  call_init(type&, decl&, expr_seq&&);

  const decl& get_function() const;
  decl& get_function();

  const expr_seq& get_arguments() const;
  expr_seq& get_arguments();

  decl* fn_;
  expr_seq args_;
};

inline
call_init::call_init(type& t, decl& f, const expr_seq& a)
  : expr(node_kind, t), fn_(&f), args_(a)
{ }

inline
call_init::call_init(type& t, decl& f, expr_seq&& a)
  : expr(node_kind, t), fn_(&f), args_(std::move(a))
{ }

// Returns the called function.
inline const decl& call_init::get_function() const { return *fn_; }

// Returns the called function.
inline decl& call_init::get_function() { return *fn_; }

// Returns the sequence of call arguments.
inline const expr_seq& call_init::get_arguments() const { return args_; }

// Returns the sequence of call arguments.
inline expr_seq& call_init::get_arguments() { return args_; }


// -------------------------------------------------------------------------- //
// Operations

bool is_void_expression(const expr&);
bool is_object_expression(const expr&);
bool is_reference_expression(const expr&);

} // namespace core
} // namespace beaker


#endif

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef TEST_AST_BUILD_HPP
#define TEST_AST_BUILD_HPP

#include "lang.hpp"

#include <beaker/sys.void/ast.hpp>
#include <beaker/sys.bool/ast.hpp>
#include <beaker/sys.int/ast.hpp>
#include <beaker/sys.name/ast.hpp>
#include <beaker/sys.var/ast.hpp>
#include <beaker/sys.fn/ast.hpp>


/// Synthesize the builders of the language into a single object.
struct builder
{
  builder(beaker::module&);

  // Names
  auto& get_name(const char* s) { return name_.get_name(s); }
  auto& get_name() { return name_.get_name(); }

  // Types
  auto& get_void_type() { return void_.get_void_type(); }
  auto& get_bool_type()  {return bool_.get_bool_type(); }
  auto& get_nat_type(int n) { return int_.get_nat_type(n); }
  auto& get_nat8_type() { return int_.get_nat8_type(); }
  auto& get_nat16_type() { return int_.get_nat16_type(); }
  auto& get_nat32_type() { return int_.get_nat32_type(); }
  auto& get_nat64_type() { return int_.get_nat64_type(); }
  auto& get_int_type(int n) { return int_.get_int_type(n); }
  auto& get_int8_type() { return int_.get_int8_type(); }
  auto& get_int16_type() { return int_.get_int16_type(); }
  auto& get_int32_type() { return int_.get_int32_type(); }
  auto& get_int64_type() { return int_.get_int64_type(); }
  auto& get_mod_type(int n) { return int_.get_int_type(n); }
  auto& get_mod8_type() { return int_.get_mod8_type(); }
  auto& get_mod16_type() { return int_.get_mod16_type(); }
  auto& get_mod32_type() { return int_.get_mod32_type(); }
  auto& get_mod64_type() { return int_.get_mod64_type(); }
  auto& get_ref_type(type& t) { return var_.get_ref_type(t); }
  auto& get_fn_type(const type_seq& ts, type& t) { return fn_.get_fn_type(ts, t); }
  auto& get_fn_type(type_seq&& ts, type& t) { return fn_.get_fn_type(std::move(ts), t); }
  auto& get_fn_type(decl_seq& ds, decl& d) { return fn_.get_fn_type(ds, d); }

  // Expressions
  auto& make_nop_expr() { return void_.make_nop_expr(); }
  auto& make_void_expr(expr& e) { return void_.make_void_expr(e); }
  
  auto& make_true_expr() { return bool_.make_true_expr(); }
  auto& make_false_expr() { return bool_.make_true_expr(); }
  auto& make_bool_expr(const value& v) { return bool_.make_bool_expr(v); }
  auto& make_bool_and_expr(expr& e1, expr& e2) { return bool_.make_and_expr(e1, e2); }
  auto& make_bool_or_expr(expr& e1, expr& e2) { return bool_.make_or_expr(e1, e2); }
  auto& make_bool_not_expr(expr& e) { return bool_.make_not_expr(e); }
  auto& make_bool_imp_expr(expr& e1, expr& e2) { return bool_.make_imp_expr(e1, e2); }
  auto& make_bool_eq_expr(expr& e1, expr& e2) { return bool_.make_eq_expr(e1, e2); }
  auto& make_if_expr(expr& e1, expr& e2, expr& e3) { return bool_.make_if_expr(e1, e2, e2); }
  auto& make_and_then_expr(expr& e1, expr& e2) { return bool_.make_and_then_expr(e1, e2); }
  auto& make_or_else_expr(expr& e1, expr& e2) { return bool_.make_or_else_expr(e1, e2); }

  auto& make_int_expr(type& t, const value& v) { return int_.make_int_expr(t, v); }
  auto& make_int_expr(type& t, value&& v) { return int_.make_int_expr(t, std::move(v)); }
  auto& make_int_expr(type& t, int n) { return int_.make_int_expr(t, n); }
  auto& make_int_eq_expr(expr& e1, expr& e2) { return int_.make_eq_expr(e1, e2); }
  auto& make_int_ne_expr(expr& e1, expr& e2) { return int_.make_ne_expr(e1, e2); }
  auto& make_int_lt_expr(expr& e1, expr& e2) { return int_.make_lt_expr(e1, e2); }
  auto& make_int_gt_expr(expr& e1, expr& e2) { return int_.make_gt_expr(e1, e2); }
  auto& make_int_le_expr(expr& e1, expr& e2) { return int_.make_le_expr(e1, e2); }
  auto& make_int_ge_expr(expr& e1, expr& e2) { return int_.make_ge_expr(e1, e2); }
  auto& make_int_add_expr(expr& e1, expr& e2) { return int_.make_add_expr(e1, e2); }
  auto& make_int_sub_expr(expr& e1, expr& e2) { return int_.make_sub_expr(e1, e2); }
  auto& make_int_mul_expr(expr& e1, expr& e2) { return int_.make_mul_expr(e1, e2); }
  auto& make_int_div_expr(expr& e1, expr& e2) { return int_.make_div_expr(e1, e2); }
  auto& make_int_rem_expr(expr& e1, expr& e2) { return int_.make_rem_expr(e1, e2); }
  auto& make_int_neg_expr(expr& e) { return int_.make_neg_expr(e); }
  auto& make_int_rec_expr(expr& e) { return int_.make_rec_expr(e); }
  auto& make_ref_expr(decl& d) { return var_.make_ref_expr(d); }
  auto& make_deref_expr(expr& e) { return var_.make_deref_expr(e); }
  auto& make_assign_expr(expr& e1, expr& e2) { return var_.make_assign_expr(e1, e2); }
  
  auto& make_call_expr(expr& e, const expr_seq& es) { return fn_.make_call_expr(e, es); }
  auto& make_call_expr(expr& e, expr_seq&& es) { return fn_.make_call_expr(e, std::move(es)); }
  auto& make_fn_eq_expr(expr& e1, expr& e2) { return fn_.make_eq_expr(e1, e2); }
  auto& make_fn_ne_expr(expr& e1, expr& e2) { return fn_.make_ne_expr(e1, e2); }

  // Initializers
  auto& make_nop_init(type& t) { return var_.make_nop_init(t); }
  auto& make_zero_init(type& t) { return var_.make_zero_init(t); }
  auto& make_copy_init(expr& e) { return var_.make_copy_init(e); }
  auto& make_ref_init(expr& e) { return var_.make_ref_init(e); }


  // Declarations
  auto& make_assert_decl(decl& cxt, expr& e) { return bool_.make_assert_decl(cxt, e); }
  
  // Declarations
  sys_var::var_decl& make_global_var_decl(name&, type&, expr&);
  sys_var::var_decl& make_global_var_decl(const char*, type&, expr&);
  sys_var::var_decl& make_local_var_decl(decl&, name&, type&, expr&);
  sys_var::var_decl& make_local_var_decl(decl&, const char*, type&, expr&);

  // Declarations
  sys_fn::fn_decl& make_fn_decl(name&, type&, const decl_seq&, decl&, stmt&);
  sys_fn::fn_decl& make_fn_decl(const char*, type&, const decl_seq&, decl&, stmt&);
  sys_fn::fn_decl& make_fn_decl(name&, type&, decl_seq&&, decl&, stmt&);
  sys_fn::fn_decl& make_fn_decl(const char*, type&, decl_seq&&, decl&, stmt&);

  sys_fn::parm_decl& make_parm_decl(name&, type&);
  sys_fn::parm_decl& make_parm_decl(const char*, type&);

  // Statements
  auto& make_block_stmt() { return fn_.make_block_stmt(); }
  auto& make_block_stmt(const stmt_seq& s) { return fn_.make_block_stmt(s); }
  auto& make_block_stmt(stmt_seq&& ss) { return fn_.make_block_stmt(std::move(ss)); }
  auto& make_expr_stmt(expr& e) { return fn_.make_expr_stmt(e); }
  auto& make_decl_stmt(decl& d) { return fn_.make_decl_stmt(d); }
  auto& make_ret_stmt(expr& e) { return fn_.make_ret_stmt(e); }

  // Common snippets
  decl& make_main();
  stmt& make_return(int);

  module* mod_;
  beaker::sys_void::builder& void_;
  beaker::sys_bool::builder& bool_;
  beaker::sys_int::builder& int_;
  beaker::sys_name::builder& name_;
  beaker::sys_var::builder& var_;
  beaker::sys_fn::builder& fn_;
};

inline 
builder::builder(module& mod)
  : mod_(&mod),
    void_(mod.get_builder<sys_void::builder>()),
    bool_(mod.get_builder<sys_bool::builder>()),
    int_(mod.get_builder<sys_int::builder>()),
    name_(mod.get_builder<sys_name::builder>()),
    var_(mod.get_builder<sys_var::builder>()),
    fn_(mod.get_builder<sys_fn::builder>())
{ }

// -------------------------------------------------------------------------- //
// Builder context

/// Establishes a builder as the current global builder.
struct global_builder
{
  static builder* current;

  global_builder(builder& b)
  {
    assert(!current);
    prev = current;
    current = &b;
  }

  ~global_builder()
  {
    current = prev;
  }

  /// Returns the current builder.
  static builder& get() { return *current; }

  builder* prev;
};


// -------------------------------------------------------------------------- //
// Expression template language.

/// Represents a literal value.
template<typename T>
struct literal_et
{
  explicit literal_et(const T b) : val(b) { }

  operator expr&() 
  { 
    return global_builder::get().make_bool_expr(value(val)); 
  }
  
  T val;
};

/// Represents a reference to a declaration.
struct ref_et
{
  explicit ref_et(decl& d) : d(&d) { }

  operator expr&() 
  {
    return global_builder::get().make_ref_expr(*d);
  }

  decl* d;
};

// Kinds of unary operations.
enum unary_op
{
  not_op,
  comp_op,
  neg_op,
  pos_op
};

// Represents a unary expression.
template<unary_op K, typename E>
struct unary_et
{
  unary_et(E e) : arg(e) { }

  operator expr&() 
  { 
    expr* e = &(expr&)arg;

    // Unary expressions generally require values.
    //
    // FIXME: This actually depends on the operator.
    if (is_reference_expression(*e))
      e = &global_builder::get().make_deref_expr(*e);

    switch (K) {
      case not_op:
        if (sys_bool::is_boolean_expression(*e))
          return global_builder::get().make_bool_not_expr(*e);
        else
          assert(false && "invalid operand");
        break;
      default:
        break;
    }
    assert(false && "operation not implemented");
  }

  E arg;
};

/// Kinds of binary expressions.
enum binary_op
{
  // Relational operations
  eq_op, ne_op, lt_op, gt_op, le_op, ge_op,

  // Arithmetic operations
  add_op, sub_op, mul_op, div_op, rem_op,

  // Binary operations
  and_op, or_op, xor_op, imp_op,

  // Logical operations
  and_if_op, or_if_op,
};

/// Returns true if e1 and e2 are boolean expressions.
inline bool
are_boolean(expr& e1, expr& e2)
{
  return sys_bool::is_boolean_expression(e1) && 
         sys_bool::is_boolean_expression(e2);
}

/// Returns true if e1 and e2 are integer expressions.
inline bool
are_integer(expr& e1, expr& e2)
{
  return sys_int::is_integral_expression(e1) && 
         sys_int::is_integral_expression(e2);
}

// Represents a binary expression.
template<binary_op K, typename E1, typename E2>
struct binary_et
{
  binary_et(E1 e1, E2 e2) : arg1(e1), arg2(e2) { }

  operator expr&() 
  {
    expr* e1 = &(expr&)arg1;
    expr* e2 = &(expr&)arg2;

    // Binary expressions generally require objects, not references.
    //
    // FIXME: This actually depends on the operator (e.g., assignment).
    if (is_reference_expression(*e1))
      e1 = &global_builder::get().make_deref_expr(*e1);
    if (is_reference_expression(*e2))
      e2 = &global_builder::get().make_deref_expr(*e2);

    switch (K) {
      case eq_op:
        if (are_boolean(*e1, *e2))
          return global_builder::get().make_bool_eq_expr(*e1, *e2);
        else if (are_integer(*e1, *e2))
          return global_builder::get().make_int_eq_expr(*e1, *e2);
        else
          assert(false && "invalid operands");
        break;
      
      case ne_op:
        if (are_integer(*e1, *e2))
          return global_builder::get().make_int_ne_expr(*e1, *e2);
        else
          assert(false && "invalid operands");
        break;
      
      case lt_op:
      case gt_op:
      case le_op:
      case ge_op:
        assert(false && "operation not implemented");
        break;

      case add_op:
      case sub_op:
      case mul_op:
      case div_op:
      case rem_op:
        assert(false && "operation not implemented");
        break;

      case and_op:
        if (are_boolean(*e1, *e2))
          return global_builder::get().make_bool_and_expr(*e1, *e2);
        else
          assert(false && "invalid operands");
        break;
      
      case or_op:
        if (are_boolean(*e1, *e2))
          return global_builder::get().make_bool_or_expr(*e1, *e2);
        else
          assert(false && "invalid operands");
        break;
      
      case xor_op:
        assert(false && "operation not implemented");
      
      case imp_op:
        if (are_boolean(*e1, *e2))
          return global_builder::get().make_bool_imp_expr(*e1, *e2);
        else
          assert(false && "invalid operands");
        break;

      case and_if_op:
        if (are_boolean(*e1, *e2))
          return global_builder::get().make_and_then_expr(*e1, *e2);
        else
          assert(false && "invalid operands");
        break;
      
      case or_if_op:
        if (are_boolean(*e1, *e2))
          return global_builder::get().make_or_else_expr(*e1, *e2);
        else
          assert(false && "invalid operands");
        break;

      default:
        assert(false && "operation not implemented");
    }
  }

  E1 arg1;
  E2 arg2;
};


/// Returns the literal `true`.
inline literal_et<bool> true_() { return literal_et<bool>(true); }

/// Returns the literal `false`.
inline literal_et<bool> false_() { return literal_et<bool>(false); }


template<typename T>
struct is_expression : std::false_type { };

template<typename T>
struct is_expression<literal_et<T>> : std::true_type { };

template<>
struct is_expression<ref_et> : std::true_type { };

template<unary_op K, typename E>
struct is_expression<unary_et<K, E>> : std::true_type { };

template<binary_op K, typename E1, typename E2>
struct is_expression<binary_et<K, E1, E2>> : std::true_type { };

template<typename T>
constexpr bool is_expression_v = is_expression<T>::value;

template<typename T1, typename T2>
constexpr bool are_expressions_v = is_expression_v<T1> && is_expression_v<T2>;

template<typename T, typename R = void>
using enable_if_expression_t = std::enable_if_t<is_expression_v<T>, R>;

template<typename T1, typename T2, typename R = void>
using enable_if_expressions_t = std::enable_if_t<are_expressions_v<T1, T2>, R>;


/// Returns expression `e1 == e2`.
template<typename E1, typename E2, typename = enable_if_expressions_t<E1, E2>> 
inline auto operator==(E1 e1, E2 e2) { return binary_et<eq_op, E1, E2>(e1, e2); }

template<typename E, typename = enable_if_expression_t<E>>
auto operator==(decl& d, E e) { return ref_et(d) == e; }

template<typename E, typename = enable_if_expression_t<E>>
auto operator==(E e, decl& d) { return e == ref_et(d); }

inline auto operator==(decl& d1, decl& d2) { return ref_et(d1) == ref_et(d2); }


/// Returns expression `e1 & e2`.
template<typename E1, typename E2, typename = enable_if_expressions_t<E1, E2>> 
inline auto operator&(E1 e1, E2 e2) { return binary_et<and_op, E1, E2>(e1, e2); }

template<typename E, typename = enable_if_expression_t<E>>
auto operator&(decl& d, E e) { return ref_et(d) & e; }

template<typename E, typename = enable_if_expression_t<E>>
auto operator&(E e, decl& d) { return e & ref_et(d); }

inline auto operator&(decl& d1, decl& d2) { return ref_et(d1) & ref_et(d2); }


/// Returns expression `e1 | e2`.
template<typename E1, typename E2, typename = enable_if_expressions_t<E1, E2>> 
inline auto operator|(E1 e1, E2 e2) { return binary_et<or_op, E1, E2>(e1, e2); }

template<typename E, typename = enable_if_expression_t<E>>
auto operator|(decl& d, E e) { return ref_et(d) | e; }

template<typename E, typename = enable_if_expression_t<E>>
auto operator|(E e, decl& d) { return e | ref_et(d); }

inline auto operator|(decl& d1, decl& d2) { return ref_et(d1) | ref_et(d2); }


/// Returns expression `e1 ^ e2`.
template<typename E1, typename E2, typename = enable_if_expressions_t<E1, E2>> 
inline auto operator^(E1 e1, E2 e2) { return binary_et<xor_op, E1, E2>(e1, e2); }

template<typename E, typename = enable_if_expression_t<E>>
auto operator^(decl& d, E e) { return ref_et(d) ^ e; }

template<typename E, typename = enable_if_expression_t<E>>
auto operator^(E e, decl& d) { return e ^ ref_et(d); }

inline auto operator^(decl& d1, decl& d2) { return ref_et(d1) | ref_et(d2); }
 

/// Returns expression `!e`.
template<typename E, typename = enable_if_expression_t<E>> 
auto operator!(E e) { return unary_et<not_op, E>(e); }

/// Returns the expression `!d`.
inline auto operator!(decl& d) { return !ref_et(d); }


// -------------------------------------------------------------------------- //
// Expression template language.

inline stmt_seq&
get_body(decl& d)
{
  stmt& s = cast<sys_fn::fn_decl>(d).get_definition();
  return cast<sys_fn::block_stmt>(s).get_statements();
}


// -------------------------------------------------------------------------- //
// Statement builder

/// Output objects can be used to capture output arguments by as if by 
/// reference. Initialization 
template<typename T>
struct out
{
  out& operator=(T& x) {
    assert(!obj);
    obj = &x;
    return *this;
  }

  operator T&() { return *obj; }

  T* obj = nullptr;
};


/// A helper class that can be used to create statements. This class is not
/// constructed directly, but rather through the `add_stmts()` function.
struct stmt_builder
{
  stmt_builder(decl& d)
    : build(global_builder::get()), cxt(d), stmts(get_body(d))
  { }

  stmt_builder& var(out<decl>&, name&, type&, expr&);
  stmt_builder& var(out<decl>&, const char*, type&, expr&);
  stmt_builder& check(expr&);
  
  builder& build;
  decl& cxt;
  stmt_seq& stmts;
};

/// Create a statement that declares a local variable and adds that to the 
/// builder's list of statements.  Saves a reference to the variable in `out`.
inline stmt_builder& 
stmt_builder::var(out<decl>& var, name& n, type& t, expr& e)
{ 
  var = build.make_local_var_decl(cxt, n, t, e);
  sys_fn::decl_stmt& stmt = build.make_decl_stmt(var);
  stmts.push_back(stmt);
  return *this;
}

/// Create a statement that declares a local variable and adds that to the 
/// builder's list of statements.  Saves a reference to the variable in `out`.
inline stmt_builder& 
stmt_builder::var(out<decl>& var, const char* n, type& t, expr& e)
{ 
  var = build.make_local_var_decl(cxt, n, t, e);
  sys_fn::decl_stmt& stmt = build.make_decl_stmt(var);
  stmts.push_back(stmt);
  return *this;
}

inline stmt_builder& 
stmt_builder::check(expr& e)
{
  sys_bool::assert_decl& decl = build.make_assert_decl(cxt, e);
  sys_fn::decl_stmt& stmt = build.make_decl_stmt(decl);
  stmts.push_back(stmt);
  return *this;
}


/// Returns a statement builder for the given function.
inline stmt_builder add_stmts(decl& d) { return {d}; }

#endif

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
#include <beaker/sys.tuple/ast.hpp>


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
  auto& get_tuple_type(const type_seq& ts) { return tup_.get_tuple_type(ts); }
  auto& get_tuple_type(type_seq&& ts) { return tup_.get_tuple_type(std::move(ts)); }

  // Expressions
  auto& make_nop_expr() { return void_.make_nop_expr(); }
  auto& make_void_expr(expr& e) { return void_.make_void_expr(e); }
  auto& make_trap_expr() { return void_.make_trap_expr(); }
  
  auto& make_true_expr() { return bool_.make_true_expr(); }
  auto& make_false_expr() { return bool_.make_false_expr(); }
  auto& make_bool_expr(const value& v) { return bool_.make_bool_expr(v); }
  auto& make_bool_and_expr(expr& e1, expr& e2) { return bool_.make_and_expr(e1, e2); }
  auto& make_bool_or_expr(expr& e1, expr& e2) { return bool_.make_or_expr(e1, e2); }
  auto& make_bool_xor_expr(expr& e1, expr& e2) { return bool_.make_xor_expr(e1, e2); }
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

  auto& make_tuple_expr(const expr_seq& es) { return tup_.make_tuple_expr(es); }
  auto& make_tuple_expr(expr_seq&& es) { return tup_.make_tuple_expr(std::move(es)); }
  auto& make_proj_expr(expr& e, int n) { return tup_.make_proj_expr(e, n); }

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
  beaker::sys_tuple::builder& tup_;
};

inline 
builder::builder(module& mod)
  : mod_(&mod),
    void_(mod.get_builder<sys_void::builder>()),
    bool_(mod.get_builder<sys_bool::builder>()),
    int_(mod.get_builder<sys_int::builder>()),
    name_(mod.get_builder<sys_name::builder>()),
    var_(mod.get_builder<sys_var::builder>()),
    fn_(mod.get_builder<sys_fn::builder>()),
    tup_(mod.get_builder<sys_tuple::builder>())
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
// Operations

inline stmt_seq&
get_body(decl& d)
{
  stmt& s = cast<sys_fn::fn_decl>(d).get_definition();
  return cast<sys_fn::block_stmt>(s).get_statements();
}


// -------------------------------------------------------------------------- //
// Utilities

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


#endif

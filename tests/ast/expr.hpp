// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef TEST_AST_EXPR_HPP
#define TEST_AST_EXPR_HPP

#include "build.hpp"


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


/// Kinds of nullary expressions.
enum nullary_op 
{
  nop_op,
  trap_op,
};

/// Represents a nullary expression.
template<nullary_op K>
struct nullary_et
{
  operator expr&()
  {
    switch (K) {
      case nop_op:
        return global_builder::get().make_nop_expr();
      case trap_op:
        return global_builder::get().make_trap_expr();
      default:
        break;
      assert(false && "invalid operation");
    }
  }
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
        if (are_boolean(*e1, *e2))
          return global_builder::get().make_bool_xor_expr(*e1, *e2);
        else
          assert(false && "invalid operands");
        break;
      
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


template<int I, typename... Args>
std::enable_if_t<(I < sizeof...(Args)), void>
static inline get_args(expr_seq& args, const std::tuple<Args...>& src)
{
  expr& e = (expr&)std::get<I>(src);
  args.push_back(e);
  get_args<I + 1>(args, src);
}

template<int I, typename... Args>
std::enable_if_t<(I == sizeof...(Args)), void>
static inline get_args(expr_seq& args, const std::tuple<Args...>& src)
{ }

// Represents a function call. 
template<typename F, typename... Args>
struct call_et
{
  call_et(F f, const Args&... a) : e1(f), en(a...) { }

  operator expr&()
  {
    expr& fn = (expr&)e1;
    expr_seq args;
    get_args<0>(args, en);
    return global_builder::get().make_call_expr(fn, std::move(args));
  }

  F e1;
  std::tuple<Args...> en;
};


template<typename T>
struct is_expression : std::false_type { };

template<typename T>
struct is_expression<literal_et<T>> : std::true_type { };

template<>
struct is_expression<ref_et> : std::true_type { };

template<unary_op K>
struct is_expression<nullary_et<K>> : std::true_type { };

template<unary_op K, typename E>
struct is_expression<unary_et<K, E>> : std::true_type { };

template<binary_op K, typename E1, typename E2>
struct is_expression<binary_et<K, E1, E2>> : std::true_type { };

template<typename E, typename... Es>
struct is_expression<call_et<E, Es...>> : std::true_type { };

template<typename T>
constexpr bool is_expression_v = is_expression<T>::value;

template<typename T1, typename T2>
constexpr bool are_expressions_v = is_expression_v<T1> && is_expression_v<T2>;

template<typename T, typename R = void>
using enable_if_expression_t = std::enable_if_t<is_expression_v<T>, R>;

// FIXME: Make this variadic in order to support a better call expression.
template<typename T1, typename T2, typename R = void>
using enable_if_expressions_t = std::enable_if_t<are_expressions_v<T1, T2>, R>;



/// Returns the literal `true`.
inline auto true_() { return literal_et<bool>(true); }

/// Returns the literal `false`.
inline auto false_() { return literal_et<bool>(false); }

/// Returns the expression `nop`.
inline auto nop() { return nullary_et<nop_op>(); }

/// Returns the expression `trap`.
inline auto trap() { return nullary_et<trap_op>(); }


/// Returns the expression `e1 == e2`.
template<typename E1, typename E2, typename = enable_if_expressions_t<E1, E2>> 
inline auto operator==(E1 e1, E2 e2) { return binary_et<eq_op, E1, E2>(e1, e2); }

template<typename E, typename = enable_if_expression_t<E>>
auto operator==(decl& d, E e) { return ref_et(d) == e; }

template<typename E, typename = enable_if_expression_t<E>>
auto operator==(E e, decl& d) { return e == ref_et(d); }

inline auto operator==(decl& d1, decl& d2) { return ref_et(d1) == ref_et(d2); }


/// Returns the expression `e1 & e2`.
template<typename E1, typename E2, typename = enable_if_expressions_t<E1, E2>> 
inline auto operator&(E1 e1, E2 e2) { return binary_et<and_op, E1, E2>(e1, e2); }

template<typename E, typename = enable_if_expression_t<E>>
auto operator&(decl& d, E e) { return ref_et(d) & e; }

template<typename E, typename = enable_if_expression_t<E>>
auto operator&(E e, decl& d) { return e & ref_et(d); }

inline auto operator&(decl& d1, decl& d2) { return ref_et(d1) & ref_et(d2); }


/// Returns the expression `e1 | e2`.
template<typename E1, typename E2, typename = enable_if_expressions_t<E1, E2>> 
inline auto operator|(E1 e1, E2 e2) { return binary_et<or_op, E1, E2>(e1, e2); }

template<typename E, typename = enable_if_expression_t<E>>
auto operator|(decl& d, E e) { return ref_et(d) | e; }

template<typename E, typename = enable_if_expression_t<E>>
auto operator|(E e, decl& d) { return e | ref_et(d); }

inline auto operator|(decl& d1, decl& d2) { return ref_et(d1) | ref_et(d2); }


/// Returns the expression `e1 ^ e2`.
template<typename E1, typename E2, typename = enable_if_expressions_t<E1, E2>> 
inline auto operator^(E1 e1, E2 e2) { return binary_et<xor_op, E1, E2>(e1, e2); }

template<typename E, typename = enable_if_expression_t<E>>
auto operator^(decl& d, E e) { return ref_et(d) ^ e; }

template<typename E, typename = enable_if_expression_t<E>>
auto operator^(E e, decl& d) { return e ^ ref_et(d); }

inline auto operator^(decl& d1, decl& d2) { return ref_et(d1) ^ ref_et(d2); }
 

/// Returns the expression `e1 && e2`.
template<typename E1, typename E2, typename = enable_if_expressions_t<E1, E2>> 
inline auto operator&&(E1 e1, E2 e2) { return binary_et<and_if_op, E1, E2>(e1, e2); }

template<typename E, typename = enable_if_expression_t<E>>
auto operator&&(decl& d, E e) { return ref_et(d) && e; }

template<typename E, typename = enable_if_expression_t<E>>
auto operator&&(E e, decl& d) { return e && ref_et(d); }

inline auto operator&&(decl& d1, decl& d2) { return ref_et(d1) && ref_et(d2); }
 

/// Returns the expression `e1 || e2`.
template<typename E1, typename E2, typename = enable_if_expressions_t<E1, E2>> 
inline auto operator||(E1 e1, E2 e2) { return binary_et<or_if_op, E1, E2>(e1, e2); }

template<typename E, typename = enable_if_expression_t<E>>
auto operator||(decl& d, E e) { return ref_et(d) || e; }

template<typename E, typename = enable_if_expression_t<E>>
auto operator||(E e, decl& d) { return e || ref_et(d); }

inline auto operator||(decl& d1, decl& d2) { return ref_et(d1) || ref_et(d2); }
 

/// Returns the expression `!e`.
template<typename E, typename = enable_if_expression_t<E>> 
auto operator!(E e) { return unary_et<not_op, E>(e); }

/// Returns the expression `!d`.
inline auto operator!(decl& d) { return !ref_et(d); }


/// Returns the abstract expression `e1 => e2`. Note that this is not directly
/// supported by C++ syntax, so it's written in the lisp style: `imp(e1, e2)`.
template<typename E1, typename E2, typename = enable_if_expressions_t<E1, E2>> 
inline auto imp(E1 e1, E2 e2) { return binary_et<imp_op, E1, E2>(e1, e2); }

template<typename E, typename = enable_if_expression_t<E>>
auto imp(decl& d, E e) { return imp(ref_et(d), e); }

template<typename E, typename = enable_if_expression_t<E>>
auto imp(E e, decl& d) { return imp(e, ref_et(d)); }

inline auto imp(decl& d1, decl& d2) { return imp(ref_et(d1), ref_et(d2)); }


/// Returns the function call expression E().
///
/// TODO: Make this variadic so it takes multiple arguments.
template<typename E, typename = enable_if_expression_t<E>> 
inline auto call(E e) { return call_et<E>(e); }

inline auto call(decl& d) { return call(ref_et(d)); }


#endif

// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_EXPR_HPP
#define BEAKER_BASE_EXPR_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/seq.hpp>
#include <beaker/base/value.hpp>
#include <beaker/util/cast.hpp>


namespace beaker {

struct type;

// -------------------------------------------------------------------------- //
// Expression base class

// Represents the set of expressions in a language.
struct expr 
{
  explicit expr(int, type&);
  virtual ~expr() = default;

  int get_feature() const;
  int get_kind() const;

  const type& get_type() const;
  type& get_type();
  int get_type_kind() const;

  const int kind_;
  type* type_;
};

// Construct an expression with kind k.
inline expr::expr(int k, type& t) : kind_(k), type_(&t) { }

// Returns the language pack that defines the expression.
inline int expr::get_feature() const { return get_language(kind_); }

// Returns the kind of expression.
inline int expr::get_kind() const { return kind_; }

// Returns the type of the expression.
inline const type& expr::get_type() const { return *type_; }

// Returns the kind of the expression.
inline type& expr::get_type() { return *type_; }


// -------------------------------------------------------------------------- //
// Expression sequence

// A sequence of expressions.
using expr_seq = seq<expr>;


// -------------------------------------------------------------------------- //
// Literal expressions

/// Represents expressions whose values are literals.
struct literal_expr : expr 
{
  literal_expr(int k, type& t, const value&);
  literal_expr(int k, type& t, value&&);

  const value& get_value() const;
  value& get_value();

  beaker::value val_;
};

// Initialize the the literal expression with the value v.
inline
literal_expr::literal_expr(int k, type& t, const value& v)
  : expr(k, t), val_(v)
{ }

// Initialize the the literal expression with the value v.
inline
literal_expr::literal_expr(int k, type& t, value&& v)
  : expr(k, t), val_(std::move(v))
{ }

// Returns the value of the literal.
inline const value& literal_expr::get_value() const { return val_; }

// Returns the value of the literal.
inline value& literal_expr::get_value() { return val_; }


/// A helper class for defining literal expressions.
template<int K>
struct generic_literal_expr : literal_expr
{
  static constexpr int node_kind = K;

  generic_literal_expr(type& t, const value&);
  generic_literal_expr(type& t, value&&);
};

// Initialize the the literal expression with the value v.
template<int K>
inline
generic_literal_expr<K>::generic_literal_expr(type& t, const value& v)
  : literal_expr(K, t, v)
{ }

// Initialize the the literal expression with the value v.
template<int K>
inline
generic_literal_expr<K>::generic_literal_expr(type& t, value&& v)
  : literal_expr(K, t, std::move(v))
{ }


// -------------------------------------------------------------------------- //
// Nullary expressions

// Represents a non-literal nullary expression.
struct nullary_expr : expr 
{
  nullary_expr(int k, type&);
};

// Initialize the the unary expression of kind k.
inline nullary_expr::nullary_expr(int k, type& t)
  : expr(k, t)
{ }


// Represents a non-literal nullary expression with no operands.
template<int K>
struct generic_nullary_expr : nullary_expr
{
  static constexpr int node_kind = K;

  generic_nullary_expr(type&);
};

// Initialize the the nullary expression of kind k.
template<int K>
inline generic_nullary_expr<K>::generic_nullary_expr(type& t)
  : nullary_expr(K, t)
{ }


// -------------------------------------------------------------------------- //
// Unary expressions

// Represents an expression with a single operand.
struct unary_expr : expr {
  unary_expr(int k, type&, expr&);

  const expr& get_operand() const;
  expr& get_operand();

  const expr& get_first() const;
  expr& get_first();

  expr* arg_;
};

// Initialize the the unary expression of kind k.
inline unary_expr::unary_expr(int k, type& t, expr& e)
  : expr(k, t), arg_(&e) 
{ }

// Returns the operand of the unary expression.
inline const expr& unary_expr::get_operand() const { return *arg_; }

// Returns the operand of the unary expression.
inline expr& unary_expr::get_operand() { return *arg_; }

// Returns the first operand of the expression.
inline const expr& unary_expr::get_first() const { return get_operand(); }

// Returns the first operand of the expression.
inline expr& unary_expr::get_first() { return get_operand(); }


// Represents an expression with a single operand.
template<int K>
struct generic_unary_expr : unary_expr {
  static constexpr int node_kind = K;

  generic_unary_expr(type&, expr&);
};

// Initialize the the unary expression of kind k.
template<int K>
inline generic_unary_expr<K>::generic_unary_expr(type& t, expr& e)
  : unary_expr(K, t, e)
{ }


// -------------------------------------------------------------------------- //
// Binary expressions

// Represents an expression with two operands.
struct binary_expr : expr {
  binary_expr(int k, type&, expr&, expr&);

  const expr& get_lhs() const;
  expr& get_lhs();

  const expr& get_rhs() const;
  expr& get_rhs();

  const expr& get_first() const;
  expr& get_first();

  const expr& get_second() const;
  expr& get_second();

  expr* args_[2];
};

// Initialize the the binary expression of kind k.
inline
binary_expr::binary_expr(int k, type& t, expr& e1, expr& e2)
  : expr(k, t), args_{&e1, &e2}
{ }

// Returns the left operand.
inline const expr& binary_expr::get_lhs() const { return *args_[0]; }

// Returns the left operand.
inline expr& binary_expr::get_lhs() { return *args_[0]; }

// Returns the right operand.
inline const expr& binary_expr::get_rhs() const { return *args_[1]; }

// Returns the right operand.
inline expr& binary_expr::get_rhs() { return *args_[1]; }

// Returns the left operand.
inline const expr& binary_expr::get_first() const { return get_lhs(); }

// Returns the left operand.
inline expr& binary_expr::get_first() { return get_lhs(); }

// Returns the right operand.
inline const expr& binary_expr::get_second() const { return get_rhs(); }

// Returns the right operand.
inline expr& binary_expr::get_second() { return get_rhs(); }


// Represents an expression with two operands.
template<int K>
struct generic_binary_expr : binary_expr {
  static constexpr int node_kind = K;

  generic_binary_expr(type&, expr&, expr&);
};

// Initialize the the binary expression of kind k.
template<int K>
inline
generic_binary_expr<K>::generic_binary_expr(type& t, expr& e1, expr& e2)
  : binary_expr(K, t, e1, e2)
{ }


// -------------------------------------------------------------------------- //
// Ternary expressions

// Represents an expression with three operands.
struct ternary_expr : expr {
  ternary_expr(int k, type&, expr&, expr&, expr&);

  const expr& get_first() const;
  expr& get_first();

  const expr& get_second() const;
  expr& get_second();

  const expr& get_third() const;
  expr& get_third();

  expr* args_[3];
};

// Initialize the the binary expression of kind k.
inline
ternary_expr::ternary_expr(int k, type& t, expr& e1, expr& e2, expr& e3)
  : expr(k, t), args_{&e1, &e2, &e3}
{ }

// Returns the left operand.
inline const expr& ternary_expr::get_first() const { return *args_[0]; }

// Returns the left operand.
inline expr& ternary_expr::get_first() { return *args_[0]; }

// Returns the right operand.
inline const expr& ternary_expr::get_second() const { return *args_[1]; }

// Returns the right operand.
inline expr& ternary_expr::get_second() { return *args_[1]; }

// Returns the right operand.
inline const expr& ternary_expr::get_third() const { return *args_[2]; }

// Returns the right operand.
inline expr& ternary_expr::get_third() { return *args_[2]; }


// Represents an expression with two operands.
template<int K>
struct generic_ternary_expr : ternary_expr {
  static constexpr int node_kind = K;

  generic_ternary_expr(type&, expr&, expr&, expr& e3);
};

// Initialize the the binary expression of kind k.
template<int K>
inline
generic_ternary_expr<K>::generic_ternary_expr(type& t, expr& e1, expr& e2, expr& e3)
  : ternary_expr(K, t, e1, e2, e3)
{ }


// -------------------------------------------------------------------------- //
// Initializers

/// Represents the initialization of an object of a given type from some 
/// intrinsic value.
///
/// Initializers do not produce values in the usual way; they are ostensibly
/// void expressions. An algorithm may, however, associate a value with an 
/// initializer as needed.
struct nullary_init : expr
{
  nullary_init(int, type&);

  const type& get_object_type() const;
  type& get_object_type();
};

inline
nullary_init::nullary_init(int k, type& t)
  : expr(k, t)
{ }

/// Returns the type of object initialized by the expression. This is 
/// same as the type of the expression.
inline const type&
nullary_init::get_object_type() const { return get_type(); }

/// Returns the type of object initialized by the expression. This is 
/// same as the type of the expression.
inline type&
nullary_init::get_object_type() { return get_type(); }


/// A helper for creating initializers.
template<int K>
struct generic_nullary_init : nullary_init
{
  static constexpr int node_kind = K;

  generic_nullary_init(type&);
};

template<int K>
inline
generic_nullary_init<K>::generic_nullary_init(type& t)
  : nullary_init(K, t)
{ }


/// Represents then initialization an object by an expression.
///
/// Initializers do not produce values in the usual way; they are ostensibly
/// void expressions. An algorithm may, however, associate a value with an 
/// initializer as needed.
struct unary_init : expr
{
  unary_init(int, type&, expr&);

  const type& get_object_type() const;
  type& get_object_type();

  const expr& get_expression() const;
  expr& get_expression();

  expr* val_;
};

inline
unary_init::unary_init(int k, type& t, expr& e)
  : expr(k, t), val_(&e)
{ }

/// Returns the type of object initialized by the expression. This is 
/// same as the type of the expression.
inline const type& unary_init::get_object_type() const { return get_type(); }

/// Returns the type of object initialized by the expression. This is 
/// same as the type of the expression.
inline type& unary_init::get_object_type() { return get_type(); }

/// Returns the expression that initializes the object.
inline const expr& unary_init::get_expression() const { return *val_; }

/// Returns the expression that initializes the object.
inline expr& unary_init::get_expression() { return *val_; }


/// A helper for creating initializers.
template<int K>
struct generic_unary_init : unary_init
{
  static constexpr int node_kind = K;

  generic_unary_init(type&, expr&);
};

template<int K>
inline
generic_unary_init<K>::generic_unary_init(type& t, expr& e)
  : unary_init(K, t, e)
{ }



// -------------------------------------------------------------------------- //
// Conversions

// Represents an expression that performs a conversion.
//
// TODO: Associate a conversion rank with each conversion expression.
template<int K>
struct conversion_expr : expr {
  static constexpr int node_kind = K;

  conversion_expr(type&, expr&);

  const expr& get_source() const;
  expr& get_source();

  expr* arg_;
};

// Initialize the conversion expression.
template<int K>
inline
conversion_expr<K>::conversion_expr(type& t, expr& e)
  : expr(K, t), arg_(&e)
{ }

// Returns the source expression of the conversion.
template<int K>
inline const expr& conversion_expr<K>::get_source() const { return *arg_; }

// Returns the target expression of the conversion.
template<int K>
inline expr& conversion_expr<K>::get_source() { return *arg_; }

} // namespace beaker


#endif

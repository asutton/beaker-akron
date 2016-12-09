// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_PROCEDURAL_STMT_HPP
#define BEAKER_PROCEDURAL_STMT_HPP

#include <beaker/base/stmt.hpp>


namespace beaker {

struct expr;
struct decl;

namespace procedural {

enum 
{
  first_stmt_kind = procedural_lang_block,
#define def_stmt(e) e ## _stmt_kind,
#include "stmt.def"
  last_stmt_kind
};


/// Represents the statement `if e then s`.
struct if_then_stmt : stmt
{
  static constexpr int node_kind = if_then_stmt_kind;

  if_then_stmt(expr&, stmt&);

  const expr& get_condition() const;
  expr& get_condition();

  const stmt& get_then() const;
  stmt& get_then();

  expr* cond_;
  stmt* then_;
};

inline 
if_then_stmt::if_then_stmt(expr& e, stmt& s)
  : stmt(node_kind), cond_(&e), then_(&s)
{ }

/// Returns the condition.
inline const expr& if_then_stmt::get_condition() const { return *cond_; }

/// Returns the condition.
inline expr& if_then_stmt::get_condition() { return *cond_; }

/// Returns the then branch.
inline const stmt& if_then_stmt::get_then() const { return *then_; }

/// Returns the then branch.
inline stmt& if_then_stmt::get_then() { return *then_; }


/// Represents the statement `if e then s1 else s2`.
struct if_else_stmt : stmt
{
  static constexpr int node_kind = if_else_stmt_kind;

  if_else_stmt(expr&, stmt&, stmt&);

  const expr& get_condition() const;
  expr& get_condition();

  const stmt& get_then() const;
  stmt& get_then();

  const stmt& get_else() const;
  stmt& get_else();

  expr* cond_;
  stmt* then_;
  stmt* else_;
};

inline 
if_else_stmt::if_else_stmt(expr& e, stmt& s1, stmt& s2)
  : stmt(node_kind), cond_(&e), then_(&s1), else_(&s2)
{ }

/// Returns the condition.
inline const expr& if_else_stmt::get_condition() const { return *cond_; }

/// Returns the condition.
inline expr& if_else_stmt::get_condition() { return *cond_; }

/// Returns the then branch.
inline const stmt& if_else_stmt::get_then() const { return *then_; }

/// Returns the then branch.
inline stmt& if_else_stmt::get_then() { return *then_; }

/// Returns the else branch.
inline const stmt& if_else_stmt::get_else() const { return *else_; }

/// Returns the else branch.
inline stmt& if_else_stmt::get_else() { return *else_; }


/// A helper class for simple loops.
template<int K>
struct loop_stmt : stmt
{
  static constexpr int node_kind = K;

  loop_stmt(expr&, stmt&);

  const expr& get_condition() const;
  expr& get_condition();

  const stmt& get_body() const;
  stmt& get_body();

  expr* cond_;
  stmt* body_;
};

template<int K>
inline 
loop_stmt<K>::loop_stmt(expr& e, stmt& s)
  : stmt(K), cond_(&e), body_(&s) 
{ }

/// Returns the loop condition.
template<int K>
inline const expr& loop_stmt<K>::get_condition() const { return *cond_; }

/// Returns the loop condition.
template<int K>
inline expr& loop_stmt<K>::get_condition() { return *cond_; }

/// Returns the loop body.
template<int K>
inline const stmt& loop_stmt<K>::get_body() const { return *body_; }

/// Returns the loop body.
template<int K>
inline stmt& loop_stmt<K>::get_body() { return *body_; }


/// Represents the statement `while e s`.
struct while_stmt : loop_stmt<while_stmt_kind>
{
  using loop_stmt<while_stmt_kind>::loop_stmt;
};


/// Represents the statement `do s while e`.
struct do_while_stmt : loop_stmt<do_while_stmt_kind>
{
  using loop_stmt<do_while_stmt_kind>::loop_stmt;
};

} // namespace procedural
} // namespace beaker


#endif

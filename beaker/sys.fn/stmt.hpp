// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_FN_STMT_HPP
#define BEAKER_SYS_FN_STMT_HPP

#include <beaker/base/stmt.hpp>


namespace beaker {

struct expr;
struct decl;

namespace sys_fn {

enum 
{
  first_stmt_kind = sys_fn_lang_block,
#define def_stmt(e) e ## _stmt_kind,
#include "stmt.def"
  last_stmt_kind
};

/// Represents the compound statement `{ s1 s2 ... sn}`
struct block_stmt : stmt
{
  static constexpr int node_kind = block_stmt_kind;

  block_stmt(const stmt_seq&);
  block_stmt(stmt_seq&&);

  const stmt_seq& get_statements() const;
  stmt_seq& get_statements();

  stmt_seq stmts_;
};

inline
block_stmt::block_stmt(const stmt_seq& s)
 : stmt(node_kind), stmts_(s)
 { }

inline
block_stmt::block_stmt(stmt_seq&& s) 
: stmt(node_kind), stmts_(std::move(s))
{ }

/// Returns the sequence of statements in the block.
inline const stmt_seq& block_stmt::get_statements() const { return stmts_; }

/// Returns the sequence of statements in the block.
inline stmt_seq& block_stmt::get_statements() { return stmts_; }


/// Represents the statement `e;`.
struct expr_stmt : stmt
{
  static constexpr int node_kind = expr_stmt_kind;

  expr_stmt(expr&);

  const expr& get_expression() const;
  expr& get_expression();

  expr* expr_;
};

inline expr_stmt::expr_stmt(expr& e) : stmt(node_kind), expr_(&e) { }

/// Returns the declaration.
inline const expr& expr_stmt::get_expression() const { return *expr_; }

/// Returns the declaration.
inline expr& expr_stmt::get_expression() { return *expr_; }


/// Represents a statement that declares a name.
struct decl_stmt : stmt
{
  static constexpr int node_kind = decl_stmt_kind;

  decl_stmt(decl&);

  const decl& get_declaration() const;
  decl& get_declaration();

  decl* decl_;
};

inline decl_stmt::decl_stmt(decl& d) : stmt(node_kind), decl_(&d) { }

/// Returns the declaration.
inline const decl& decl_stmt::get_declaration() const { return *decl_; }

/// Returns the declaration.
inline decl& decl_stmt::get_declaration() { return *decl_; }


/// Represents the statement `return e`.
///
/// The expression `e` must be either an initializer or the `nop` expression. 
/// When `e` is an initializer, it initializes the return value. After 
/// initializing the return value, the return statement returns control to the 
/// calling function.
struct ret_stmt : stmt
{
  static constexpr int node_kind = ret_stmt_kind;

  ret_stmt(expr&);

  const expr& get_return() const;
  expr& get_return();

  expr* val_;
};

inline ret_stmt::ret_stmt(expr& e) : stmt(node_kind), val_(&e) { }

/// Returns the initializer for the return value.
inline const expr& ret_stmt::get_return() const { return *val_; }

/// Returns the initializer for the return value.
inline expr& ret_stmt::get_return() { return *val_; }

} // namespace sys_fn
} // namespace beaker


#endif

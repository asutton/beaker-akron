// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_BOOL_DECL_HPP
#define BEAKER_SYS_BOOL_DECL_HPP

#include <beaker/base/decl.hpp>

namespace beaker {
namespace sys_bool {

enum 
{
  first_decl_kind = sys_bool_lang_block,
#define def_decl(e) e ## _decl_kind,
#include "decl.def"
  last_decl_kind
};

/// Represents declarations of the form `assert e`. An assertion of a logical 
/// expression (property) holds the value of that expression as an 
/// incontrovertible truth.
///
/// If the value of the expression is `false`, then the behavior of the program
/// is undefined. However, if by analysis, the expression can be proven true,
/// the evaluation is not required. Alternatively, a translation may simply
/// assume that all assertions are true, and disable evaluation in that way.
///
/// The asserted expression must have type `bool`.
///
/// Assertions are modeled as declarations because they may introduce facts
/// into a fact-checking environment.
struct assert_decl : decl
{
  static constexpr int node_kind = assert_decl_kind;

  assert_decl(expr&);

  const expr& get_assertion() const;
  expr& get_assertion();

  expr* expr_;
};

inline
assert_decl::assert_decl(expr& e)
  : decl(node_kind), expr_(&e)
{ }

/// Returns the asserted expression.
inline const expr& assert_decl::get_assertion() const { return *expr_; }

/// Returns the asserted expression.
inline expr& assert_decl::get_assertion() { return *expr_; }

} // namespace sys_bool
} // namespace beaker


#endif

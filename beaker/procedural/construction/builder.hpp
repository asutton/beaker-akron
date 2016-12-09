// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_PROCEDURAL_CONSTRUCTION_BUILDER_HPP
#define BEAKER_PROCEDURAL_CONSTRUCTION_BUILDER_HPP

#include <beaker/base/construction/builder.hpp>


namespace beaker {

struct expr;
struct decl;

namespace procedural {

/// Forward declare all statements.
#define def_stmt(e) struct e##_stmt;
#include "../stmt.def"


/// Provides access to resources needed to construct, validate, and
/// evaluate procedural terms.
struct builder : builder_base
{
  builder(module&);

  // Statement
  if_then_stmt& make_if_then_stmt(expr&, stmt&);
  if_else_stmt& make_if_else_stmt(expr&, stmt&, stmt&);
  while_stmt& make_while_stmt(expr&, stmt&);
  do_while_stmt& make_do_while_stmt(expr&, stmt&);
};


} // namespace procedural
} // namespace beaker


#endif

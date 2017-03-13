// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_FN_FWD_HPP
#define BEAKER_SYS_FN_FWD_HPP


namespace beaker {
namespace sys_fn {

struct feature;
#define def_type(NS, T) struct T ## _type;
#include "type.def"
#define def_expr(NS, E) struct E ## _expr;
#include "expr.def"
#define def_decl(NS, D) struct D ## _decl;
#include "decl.def"
#define def_stmt(NS, S) struct S ## _stmt;
#include "stmt.def"
struct builder;

} // namespace sys_fn
} // namespace beaker


#endif

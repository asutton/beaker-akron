// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_FN_FWD_HPP
#define BEAKER_SYS_FN_FWD_HPP


namespace beaker {
namespace sys_fn {

struct feature;
#define def_type(T) struct T##_type;
#define def_expr(E) struct E##_expr;
#define def_decl(D) struct D##_decl;
#define def_stmt(S) struct S##_stmt;
#include "type.def"
#include "expr.def"
#include "decl.def"
#include "stmt.def"
struct builder;

} // namespace sys_fn
} // namespace beaker


#endif

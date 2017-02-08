// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VOID_FWD_HPP
#define BEAKER_SYS_VOID_FWD_HPP


namespace beaker {
namespace sys_void {

struct feature;
#define def_type(T, B) struct T ##_type;
#include "type.def"
#define def_expr(E, B) struct E ##_expr;
#include "expr.def"
struct builder;

} // namespace sys_void
} // namespace beaker


#endif

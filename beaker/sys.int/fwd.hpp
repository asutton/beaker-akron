// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_INT_FWD_HPP
#define BEAKER_SYS_INT_FWD_HPP


namespace beaker {
namespace sys_int {

struct feature;
struct integral_type;
#define def_type(NS, T, B) struct T ## _type;
#include "type.def"
#define def_expr(NS, E, B) struct E ## _expr;
#include "expr.def"
struct builder;

} // namespace sys_int
} // namespace beaker


#endif

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_INT_FWD_HPP
#define BEAKER_SYS_INT_FWD_HPP


namespace beaker {
namespace sys_int {

struct feature;
#define def_type(T) struct T##_type;
#define def_expr(E) struct E##_expr;
#include "type.def"
#include "expr.def"
struct builder;

} // namespace sys_int
} // namespace beaker


#endif

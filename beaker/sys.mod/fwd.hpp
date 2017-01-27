// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_MOD_FWD_HPP
#define BEAKER_SYS_MOD_FWD_HPP


namespace beaker {
namespace sys_mod {

struct feature;
#define def_decl(D) struct D##_decl;
#include "decl.def"
struct builder;

} // namespace sys_mod
} // namespace beaker


#endif

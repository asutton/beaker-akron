// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_NAME_FWD_HPP
#define BEAKER_SYS_NAME_FWD_HPP


namespace beaker {
namespace sys_name {

struct feature;
#define def_name(NS, N) struct N ## _name;
#include "name.def"
struct builder;

} // namespace sys_name
} // namespace beaker


#endif

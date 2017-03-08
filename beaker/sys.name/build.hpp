// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_NAME_BUILD_HPP
#define BEAKER_SYS_NAME_BUILD_HPP

#include <beaker/sys.name/fwd.hpp>

#include <beaker/base/build.hpp>


namespace beaker {

struct symbol;

namespace sys_name {

/// Provides access to resources needed to construct, validate, and
/// evaluate sys_name terms.
struct builder : factory
{
  builder(module&);

  internal_name& get_name();
  basic_name& get_name(const char*);
  basic_name& get_name(const symbol&);

  canonical_term_set<basic_name>* name_;
  int current_id_;
};

} // namespace sys_name
} // namespace beaker


#endif

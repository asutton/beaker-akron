// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VAR_COMPARISON_HASH_HPP
#define BEAKER_SYS_VAR_COMPARISON_HASH_HPP

#include <beaker/sys.var/fwd.hpp>

#include <beaker/base/comparison/hash.hpp>


namespace beaker {

void hash_type(hasher&, const sys_var::ref_type&);

void hash_expr(hasher&, const sys_var::ref_expr&);
void hash_expr(hasher&, const sys_var::val_expr&);
void hash_expr(hasher&, const sys_var::assign_expr&);
void hash_expr(hasher&, const sys_var::nop_init&);
void hash_expr(hasher&, const sys_var::zero_init&);
void hash_expr(hasher&, const sys_var::copy_init&);
void hash_expr(hasher&, const sys_var::ref_init&);

} // namespace beaker


#endif

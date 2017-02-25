// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VAR_COMPARISON_EQUAL_HPP
#define BEAKER_SYS_VAR_COMPARISON_EQUAL_HPP

#include <beaker/sys.var/fwd.hpp>

#include <beaker/base/comparison/equal.hpp>


namespace beaker {

bool equal_type(const sys_var::ref_type&, const sys_var::ref_type&);

bool equal_expr(const sys_var::ref_expr&, const sys_var::ref_expr&);
bool equal_expr(const sys_var::deref_expr&, const sys_var::deref_expr&);
bool equal_expr(const sys_var::assign_expr&, const sys_var::assign_expr&);
bool equal_expr(const sys_var::nop_init&, const sys_var::nop_init&);
bool equal_expr(const sys_var::zero_init&, const sys_var::zero_init&);
bool equal_expr(const sys_var::copy_init&, const sys_var::copy_init&);
bool equal_expr(const sys_var::ref_init&, const sys_var::ref_init&);

} // namespace beaker


#endif

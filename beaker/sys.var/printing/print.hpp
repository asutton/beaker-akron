// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VAR_PRINTING_PRINT_HPP
#define BEAKER_SYS_VAR_PRINTING_PRINT_HPP

#include <beaker/sys.var/fwd.hpp>

#include <beaker/base/printing/print.hpp>


namespace beaker {

void print_type(pretty_printer&, const sys_var::ref_type&);

void print_expr(pretty_printer&, const sys_var::ref_expr&);
void print_expr(pretty_printer&, const sys_var::val_expr&);
void print_expr(pretty_printer&, const sys_var::assign_expr&);
void print_expr(pretty_printer&, const sys_var::nop_init&);
void print_expr(pretty_printer&, const sys_var::zero_init&);
void print_expr(pretty_printer&, const sys_var::copy_init&);
void print_expr(pretty_printer&, const sys_var::ref_init&);

} // namespace beaker


#endif

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_INT_PRINTING_PRINT_HPP
#define BEAKER_SYS_INT_PRINTING_PRINT_HPP

#include <beaker/sys.int/fwd.hpp>

#include <beaker/base/printing/print.hpp>


namespace beaker {

void print(pretty_printer&, const sys_int::nat_type&);
void print(pretty_printer&, const sys_int::int_type&);
void print(pretty_printer&, const sys_int::mod_type&);

void print(pretty_printer&, const sys_int::int_expr&);
void print(pretty_printer&, const sys_int::eq_expr&);
void print(pretty_printer&, const sys_int::ne_expr&);
void print(pretty_printer&, const sys_int::lt_expr&);
void print(pretty_printer&, const sys_int::gt_expr&);
void print(pretty_printer&, const sys_int::le_expr&);
void print(pretty_printer&, const sys_int::ge_expr&);
void print(pretty_printer&, const sys_int::add_expr&);
void print(pretty_printer&, const sys_int::sub_expr&);
void print(pretty_printer&, const sys_int::mul_expr&);
void print(pretty_printer&, const sys_int::quo_expr&);
void print(pretty_printer&, const sys_int::rem_expr&);
void print(pretty_printer&, const sys_int::neg_expr&);
void print(pretty_printer&, const sys_int::rec_expr&);

} // namespace beaker


#endif

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_INT_PRINTING_PRINT_HPP
#define BEAKER_SYS_INT_PRINTING_PRINT_HPP

#include <beaker/sys.int/fwd.hpp>

#include <beaker/base/printing/print.hpp>


namespace beaker {
namespace sys_int {

void print_nat_type(pretty_printer&, const nat_type&);
void print_int_type(pretty_printer&, const int_type&);
void print_mod_type(pretty_printer&, const mod_type&);

void print_int_expr(pretty_printer&, const int_expr&);
void print_eq_expr(pretty_printer&, const eq_expr&);
void print_ne_expr(pretty_printer&, const ne_expr&);
void print_lt_expr(pretty_printer&, const lt_expr&);
void print_gt_expr(pretty_printer&, const gt_expr&);
void print_le_expr(pretty_printer&, const le_expr&);
void print_ge_expr(pretty_printer&, const ge_expr&);
void print_add_expr(pretty_printer&, const add_expr&);
void print_sub_expr(pretty_printer&, const sub_expr&);
void print_mul_expr(pretty_printer&, const mul_expr&);
void print_div_expr(pretty_printer&, const div_expr&);
void print_rem_expr(pretty_printer&, const rem_expr&);
void print_neg_expr(pretty_printer&, const neg_expr&);
void print_rec_expr(pretty_printer&, const rec_expr&);

} // namespace sys_int
} // namespace beaker


#endif

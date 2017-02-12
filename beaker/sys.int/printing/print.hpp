// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_INT_PRINTING_PRINT_HPP
#define BEAKER_SYS_INT_PRINTING_PRINT_HPP

#include <beaker/sys.int/fwd.hpp>

#include <beaker/base/printing/print.hpp>


namespace beaker {
namespace sys_int {

void print(pretty_printer&, const nat_type&);
void print(pretty_printer&, const int_type&);
void print(pretty_printer&, const mod_type&);

void print(pretty_printer&, const int_expr&);
void print(pretty_printer&, const eq_expr&);
void print(pretty_printer&, const ne_expr&);
void print(pretty_printer&, const lt_expr&);
void print(pretty_printer&, const gt_expr&);
void print(pretty_printer&, const le_expr&);
void print(pretty_printer&, const ge_expr&);
void print(pretty_printer&, const add_expr&);
void print(pretty_printer&, const sub_expr&);
void print(pretty_printer&, const mul_expr&);
void print(pretty_printer&, const quo_expr&);
void print(pretty_printer&, const rem_expr&);
void print(pretty_printer&, const neg_expr&);
void print(pretty_printer&, const rec_expr&);

} // namespace sys_int
} // namespace beaker


#endif

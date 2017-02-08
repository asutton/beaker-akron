// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_BOOL_PRINTING_PRINT_HPP
#define BEAKER_SYS_BOOL_PRINTING_PRINT_HPP

#include <beaker/sys.bool/fwd.hpp>

#include <beaker/base/printing/print.hpp>


namespace beaker {
namespace sys_bool {

void print_bool_type(pretty_printer&, const bool_type&);

void print_bool_expr(pretty_printer&, const bool_expr&);
void print_and_expr(pretty_printer&, const and_expr&);
void print_or_expr(pretty_printer&, const or_expr&);
void print_xor_expr(pretty_printer&, const xor_expr&);
void print_not_expr(pretty_printer&, const not_expr&);
void print_imp_expr(pretty_printer&, const imp_expr&);
void print_eq_expr(pretty_printer&, const eq_expr&);
void print_if_expr(pretty_printer&, const if_expr&);
void print_and_then_expr(pretty_printer&, const and_then_expr&);
void print_or_else_expr(pretty_printer&, const or_else_expr&);
void print_assert_expr(pretty_printer&, const assert_expr&);

} // namespace sys_bool
} // namespace beaker


#endif

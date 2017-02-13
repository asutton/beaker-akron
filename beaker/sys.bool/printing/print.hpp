// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_BOOL_PRINTING_PRINT_HPP
#define BEAKER_SYS_BOOL_PRINTING_PRINT_HPP

#include <beaker/sys.bool/fwd.hpp>

#include <beaker/base/printing/print.hpp>


namespace beaker {

void print(pretty_printer&, const sys_bool::bool_type&);

void print(pretty_printer&, const sys_bool::bool_expr&);
void print(pretty_printer&, const sys_bool::and_expr&);
void print(pretty_printer&, const sys_bool::or_expr&);
void print(pretty_printer&, const sys_bool::xor_expr&);
void print(pretty_printer&, const sys_bool::not_expr&);
void print(pretty_printer&, const sys_bool::imp_expr&);
void print(pretty_printer&, const sys_bool::eq_expr&);
void print(pretty_printer&, const sys_bool::if_expr&);
void print(pretty_printer&, const sys_bool::and_then_expr&);
void print(pretty_printer&, const sys_bool::or_else_expr&);
void print(pretty_printer&, const sys_bool::assert_expr&);

} // namespace beaker


#endif

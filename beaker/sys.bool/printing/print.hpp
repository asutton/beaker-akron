// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_BOOL_PRINTING_PRINT_HPP
#define BEAKER_SYS_BOOL_PRINTING_PRINT_HPP

#include <beaker/sys.bool/fwd.hpp>

#include <beaker/base/printing/print.hpp>


namespace beaker {
namespace sys_bool {

void print(pretty_printer&, const bool_type&);

void print(pretty_printer&, const bool_expr&);
void print(pretty_printer&, const and_expr&);
void print(pretty_printer&, const or_expr&);
void print(pretty_printer&, const xor_expr&);
void print(pretty_printer&, const not_expr&);
void print(pretty_printer&, const imp_expr&);
void print(pretty_printer&, const eq_expr&);
void print(pretty_printer&, const if_expr&);
void print(pretty_printer&, const and_then_expr&);
void print(pretty_printer&, const or_else_expr&);
void print(pretty_printer&, const assert_expr&);

} // namespace sys_bool
} // namespace beaker


#endif

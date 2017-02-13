// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VOID_PRINTING_PRINT_HPP
#define BEAKER_SYS_VOID_PRINTING_PRINT_HPP

#include <beaker/sys.void/fwd.hpp>
#include <beaker/base/printing/print.hpp>


namespace beaker {

void print(pretty_printer&, const sys_void::void_type&);

void print(pretty_printer&, const sys_void::nop_expr&);
void print(pretty_printer&, const sys_void::void_expr&);
void print(pretty_printer&, const sys_void::trap_expr&);

} // namespace beaker


#endif

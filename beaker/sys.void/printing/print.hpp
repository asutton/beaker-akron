// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VOID_PRINTING_PRINT_HPP
#define BEAKER_SYS_VOID_PRINTING_PRINT_HPP

#include <beaker/sys.void/fwd.hpp>
#include <beaker/base/printing/print.hpp>


namespace beaker {
namespace sys_void {

void print_void_type(pretty_printer&, const void_type&);

void print_nop_expr(pretty_printer&, const nop_expr&);
void print_void_expr(pretty_printer&, const void_expr&);
void print_trap_expr(pretty_printer&, const trap_expr&);

} // namespace sys_void
} // namespace beaker


#endif

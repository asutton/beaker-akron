// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_FN_PRINTING_PRINT_HPP
#define BEAKER_SYS_FN_PRINTING_PRINT_HPP

#include <beaker/sys.fn/fwd.hpp>

#include <beaker/base/printing/print.hpp>


namespace beaker {

void print_type(pretty_printer&, const sys_fn::fn_type&);

void print_expr(pretty_printer&, const sys_fn::call_expr&);
void print_expr(pretty_printer&, const sys_fn::eq_expr&);
void print_expr(pretty_printer&, const sys_fn::ne_expr&);

void print_decl(pretty_printer&, const sys_fn::fn_decl&);
void print_decl(pretty_printer&, const sys_fn::parm_decl&);
void print_decl(pretty_printer&, const sys_fn::var_decl&);

void print_stmt(pretty_printer&, const sys_fn::block_stmt&);
void print_stmt(pretty_printer&, const sys_fn::decl_stmt&);
void print_stmt(pretty_printer&, const sys_fn::expr_stmt&);
void print_stmt(pretty_printer&, const sys_fn::ret_stmt&);

} // namespace beaker

#endif

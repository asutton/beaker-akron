// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_NAME_PRINTING_PRINT_HPP
#define BEAKER_SYS_NAME_PRINTING_PRINT_HPP

#include <beaker/sys.name/fwd.hpp>

#include <beaker/base/printing/print.hpp>


namespace beaker {

void print_name(pretty_printer&, const sys_name::basic_name&);
void print_name(pretty_printer&, const sys_name::internal_name&);

} // namespace beaker


#endif

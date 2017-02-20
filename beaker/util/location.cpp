// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "location.hpp"

#include <iostream>


namespace beaker {

/// Print a common representation of source code locations.
std::ostream& 
operator<<(std::ostream& os, const location& loc)
{
  if (loc.is_invalid())
    return os << "<invalid-location>";
  if (loc.is_file_location())
    os << loc.get_filename() << ":";
  else
    os << "<standard-input>" << ":";
  return os << loc.get_line() << ":" << loc.get_start_column();
}

} // namespace beaker

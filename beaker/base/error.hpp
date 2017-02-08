// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_ERROR_HPP
#define BEAKER_BASE_ERROR_HPP

#include <stdexcept>


namespace beaker {

/// Represents an error that occurred during translation or evaluation. Errors
/// are intended to be used for diagnostics.
///
/// \todo Integrate source code location information into errors. Provide
/// additional error aggregation and error handling facilities.
struct error : std::runtime_error
{
  error(const char*);
  error(const std::string&);
};

/// Initialize an error with the given message.
inline error::error(const char* str) : std::runtime_error(str) { }

/// Initialize an error with the given message.
inline error::error(const std::string& str) : std::runtime_error(str) { }

} // namespace beaker

#endif

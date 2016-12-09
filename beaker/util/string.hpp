// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_UTIL_STRING_HPP
#define BEAKER_UTIL_STRING_HPP

#include <cstring>

namespace beaker {


/// A null-terminated string that does not own the underlying contents.
struct cstring
{
  cstring(const char*);
  cstring(const char*, std::size_t);

  std::size_t size() const;

  const char* data() const;

  const char* begin() const;
  const char* end() const;

  const char* str;
  std::size_t len;
};

inline
cstring::cstring(const char* s)
  : str(s), len(std::strlen(s))
{ }

/// FIXME: Make sure this is null-terminated...
inline
cstring::cstring(const char* s, std::size_t n)
  : str(s), len(n)
{ }

/// Returns the length of the string. This does not include the null-terminator.
inline std::size_t
cstring::size() const { return len; }

/// Returns the underlying string data.
inline const char*
cstring::data() const { return str; }

/// Returns a pointer to the first character in the string.
inline char const* 
cstring::begin() const { return str; }

/// Returns a pointer past the last character in the string.
inline char const* 
cstring::end() const { return str + len; }

} // namespace beaker

#endif

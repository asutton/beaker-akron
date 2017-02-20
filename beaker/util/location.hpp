// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_UTIL_LOCATION_HPP
#define BEAKER_UTIL_LOCATION_HPP

#include <iosfwd>


namespace beaker
{

/// Represents a location in a source file or input object.
///
/// \todo This is a VERY heavyweight object. Make this a single 32-bit integer
/// that indexes into a data structure that describes source code locations.
///
/// \todo The "file" is actually: a file on disk, an input stream, or in
/// memory. Factor that into our input. Note that this class, as defined,
/// is a regular type. Other designs make this a handle that can't be 
/// interpreted without external context.
struct location
{
  location();
  location(int, int);
  location(const char*, int, int);

  bool is_invalid() const;
  bool is_file() const;

  const char* get_filename() const;
  int get_line() const;
  int get_column() const;

  const char* file; // YUCK!
  int line;
  int col;
};

/// Initialize this as an invalid source location.
inline location::location() : location(nullptr, 0, 0) { }

/// Initialize this as an in-memory source location with location (l, c).
inline location::location(int l, int c) : location(nullptr, l, c) { }

/// Initialize this as an source location in input file f with location (l, c).
inline location::location(const char* f, int l, int c) : file(f), line(l), col(c) { }

/// Returns true when the location is invalid. Note that all other fields
/// are also 0.
inline bool location::is_invalid() const { return line == 0; }

/// Returns true when this is a file.
inline bool location::is_file() const { return file != nullptr; }

/// Returns the name of the input file.
inline const char* location::get_filename() const { return file; }

/// Returns the line number.
inline int location::get_line() const { return line; }

/// Returns the column number.
inline int location::get_column() const { return col; }

std::ostream& operator<<(std::ostream&, const location&);

} // namespace beaker


#endif

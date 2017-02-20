// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_UTIL_LOCATION_HPP
#define BEAKER_UTIL_LOCATION_HPP

#include <iosfwd>


namespace beaker
{

/// Represents a location of a symbol in a source file or input object. The
/// location is represented by a quadruple: the source file, the line, the
/// starting column, and the ending column (as a limit). Note that locations
/// never span lines (although strings in the language may).
///
/// \todo This is a VERY heavy object. Consider an alternative design where
/// this is an index into a separate data structures that describes locations.
/// Hint: maintain a single text repository and make this index into an ordered
/// tree of symbols.
///
/// \todo A string literal is a symbol that can span multiple lines. This can
/// be handled gracefully if indexing into a separate data structure; we 
/// probably want to embed offset + length in the line number if we don't.
///
/// \todo The "file" is actually: a file on disk, an input stream, or in
/// memory. Factor that into our input. Note that this class, as defined,
/// is a regular type. Other designs make this a handle that can't be 
/// interpreted without external context.
struct location
{
  location();
  location(int, int, int);
  location(const char*, int, int, int);

  bool is_invalid() const;
  bool is_file_location() const;

  const char* get_filename() const;
  int get_line() const;
  int get_column() const;
  int get_start_column() const;
  int get_end_column() const;

  const char* file; // YUCK!
  int line;
  int start;
  int end;
};

/// Initialize this as an invalid source location.
inline location::location() : location(nullptr, 0, 0, 0) { }

/// Initialize this as an in-memory source location with location (l, s, e).
inline location::location(int l, int s, int e) : location(nullptr, l, s, e) { }

/// Initialize this as an source location in input file f with location 
/// (l, c, e).
inline 
location::location(const char* f, int l, int s, int e)
  : file(f), line(l), start(s), end(e) 
{ }

/// Returns true when the location is invalid. Note that all other fields
/// are also 0.
inline bool location::is_invalid() const { return line == 0; }

/// Returns true when this is a file.
inline bool location::is_file_location() const { return file != nullptr; }

/// Returns the name of the input file.
inline const char* location::get_filename() const { return file; }

/// Returns the line number.
inline int location::get_line() const { return line; }

/// Returns the starting column number. When used without qualification, this
/// refers to the start column.
inline int location::get_column() const { return start; }

/// Returns the starting column number.
inline int location::get_start_column() const { return start; }

/// Returns the starting column number.
inline int location::get_end_column() const { return end; }

std::ostream& operator<<(std::ostream&, const location&);


// -------------------------------------------------------------------------- //

struct locatable
{
  locatable() : loc_() { }
  locatable(location loc) : loc_(loc) { }

  location get_location() const { return loc_; }
  void set_location(location loc) { loc_ = loc; }
  
  location loc_;
};

} // namespace beaker


#endif

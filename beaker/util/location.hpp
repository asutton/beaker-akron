// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_UTIL_LOCATION_HPP
#define BEAKER_UTIL_LOCATION_HPP

#include <array>
#include <iosfwd>
#include <memory>
#include <vector>


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
// Term layout

struct layout
{
  virtual ~layout() = default;
  virtual int size() const = 0;
  virtual location operator[](int) const = 0;

  location get_location() const;
  location get_start_location() const;
  location get_end_location() const;
};

/// Returns the location of the term.
inline location layout::get_location() const { return get_start_location(); }

/// Returns the starting location of a term.
inline location layout::get_start_location() const { return (*this)[0]; }

/// Returns the end location of a term.
inline location layout::get_end_location() const { return (*this)[size() - 1]; }


/// An array layout contains a fixed-size sequence of source code locations.
template<int N>
struct array_layout : layout, std::array<location, N>
{
  using base_type = std::array<location, N>;

  array_layout(std::initializer_list<location>);
  
  int size() const override;
  location operator[](int) const override;
};

template<int N>
inline 
array_layout<N>::array_layout(std::initializer_list<location> locs)
{ 
  std::copy(locs.begin(), locs.end(), this->begin());
}

/// Returns the number of locations in the layout.
template<int N>
inline int array_layout<N>::size() const { return base_type::size(); }

/// Returns the nth location in the layout.
template<int N>
inline location array_layout<N>::operator[](int n) const { return base_type::operator[](n); }


/// A vector layout contains a variable-length sequence of tokens.
struct vector_layout : layout, std::vector<location>
{
  using base_type = std::vector<location>;

  vector_layout(const base_type&);
  vector_layout(base_type&&);
  vector_layout(std::initializer_list<location>);
  
  int size() const override;
  location operator[](int) const override;
};

/// Initialize this layout with a copy of the locations in `vec`.
inline vector_layout::vector_layout(const base_type& vec) : base_type(vec) { }

/// Initialize this layout with the moved locations in `vec`.
inline vector_layout::vector_layout(base_type&& vec) : base_type(std::move(vec)) { }

/// Initialize this layout with the locations in `locs`.
inline vector_layout::vector_layout(std::initializer_list<location> locs) : base_type(locs) { }

/// Returns the number of locations in the layout.
inline int vector_layout::size() const { return base_type::size(); }

/// Returns the nth location in the layout.
inline location vector_layout::operator[](int n) const { return base_type::operator[](n); }


/// A helper class for all terms that support layouts i.e., can be written
/// in source code.
///
/// The layout object is maintained indirectly in order to reduce the amount
/// of memory needed for internally constructed terms (e.g., canonical types).
struct locatable
{
  bool has_layout() const;
  const layout& get_layout() const;
  layout& get_layout();
 
  location get_location() const;
  location get_start_location() const;
  location get_end_location() const;

  template<typename... Args>
  void set_fixed_layout(Args... args);
  
  void set_variable_layout(const std::vector<location>&);
  void set_variable_layout(std::vector<location>&&);

  std::unique_ptr<layout> locs_ = nullptr;
};

/// Returns true if the derived term has a physical layout.
inline bool locatable::has_layout() const { return locs_ != nullptr; }

/// Returns the term's physical layout.
inline const layout& locatable::get_layout() const { return *locs_; }

/// Returns the term's physical layout.
inline layout& locatable::get_layout() { return *locs_; }

/// Returns the location of the term.
inline location locatable::get_location() const { return locs_->get_location(); }

/// Returns the starting location of a term.
inline location locatable::get_start_location() const { return locs_->get_start_location(); }

/// Returns the end location of a term.
inline location locatable::get_end_location() const { return locs_->get_end_location(); }

/// Sets the layout of a term to the locations in `args`.
template<typename... Args>
inline void
locatable::set_fixed_layout(Args... args)
{
  locs_.reset(new array_layout<sizeof...(Args)>({args...}));
}

/// Sets the layout of a term to a sequence of locations.
inline void
locatable::set_variable_layout(const std::vector<location>& locs)
{
  locs_.reset(new vector_layout(locs));
}

/// Sets the layout of a term to a sequence of locations.
inline void
locatable::set_variable_layout(std::vector<location>&& locs)
{
  locs_.reset(new vector_layout(std::move(locs)));
}

} // namespace beaker


#endif

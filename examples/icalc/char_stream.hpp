// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef ICALC_CHAR_STREAM_HPP
#define ICALC_CHAR_STREAM_HPP

#include "token.hpp"

#include <cassert>
#include <cctype>
#include <stdexcept>
#include <string>


namespace icalc {

/// The character stream interface provides a facility for extracting
/// characters from an input source.
struct char_stream
{
  virtual bool eof() const = 0;
  virtual char lookahead() const = 0;
  virtual char lookahead(int) const = 0;
  virtual char consume() = 0;
  virtual void ignore() = 0;
};


/// A string stream provides a streaming interface for a fully buffered string.
/// Note that this object does not own the string.
struct string_stream : char_stream
{
  string_stream(const std::string& str);

  virtual char lookahead() const override;
  virtual char lookahead(int) const;
  virtual char consume() override;
  virtual void ignore() override;

  const char* first;
  const char* limit;
};

inline
string_stream(const std::string& str)
  : buf(&str), first(str.c_str()), limit(first + str.size())
{ }

/// Returns true if the stream is at its end.
inline bool string_stream::eof() const { return curr == last; }

/// Returns the lookahead character.
inline char 
string_stream::lookahead() const
{
  if (eof())
    return 0;
  else
    return *curr;
}

/// Returns the nth character past the lookahead.
inline char
string_stream::lookahead(int n) const
{
  if (last - curr <= n)
    return 0;
  else
    return *(curr + n);
}

/// Buffers and returns the current character. Advances the stream.
inline char
string_stream::consume()
{
  if (eof())
    return 0;
  buf += *curr++;
  return buf.back();
}

/// Ingores the current character and advances the stream.
inline char
string_stream::ignore()
{
  if (eof())
    return 0;
  return *curr++;
}


struct istream_stream : char_stream
{
  istream_stream(std::istream& is)
    : is(is)
  { }

  virtual bool eof() const override;
  virtual char lookahead() const override;
  virtual char consume() override;
  virtual void ignore() override;

  std::istream& is;
};

inline bool istream_stream::eof() const { return is.eof(); }

inline char
istream_stream::lookahead() const
{
  return is.peek();
}

inline char
istream_stream::consume() const
{
  return is.get();
}

inline char
istream_stream::ignore() const
{
  return is.ignore();
}

} // namespace icalc

#endif


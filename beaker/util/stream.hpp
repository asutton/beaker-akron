// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_UTIL_CHAR_STREAM_HPP
#define BEAKER_UTIL_CHAR_STREAM_HPP

#include <iosfwd>
#include <deque>
#include <string>
#include <vector>


namespace beaker {

/// An input stream interface provides a facility for extracting characters 
/// from an input source. This is parameterized by the type of object being
/// extracted from the streams. This is required to be a semiregular type.
/// A default-initialized stream element shall indicate the end of input.
///
/// This is a simplified version standard iostreams; it does not require an
/// underlying stream buffer.
///
/// \todo Support an n-character get that fills a string buffer?
template<typename T>
struct input_stream
{
  /// Returns true if the stream has no more input.
  virtual bool eof() const = 0;
  
  /// Returns the current character in the stream or 0 if at the end of input.
  virtual const T& peek() const = 0;
  
  /// Returns the `n`th character in the stream or 0 if past the end of input.
  virtual const T& peek(int n) const = 0;
  
  /// Returns the current character and advances the stream or 0 if at the
  /// end of input.
  virtual T get() = 0;
  
  /// Advances the stream the next character. Has no behavior if past the end
  /// of input.
  virtual void ignore() = 0;

  /// Advances the stream `n` characters or all remaining input if fewer than
  /// `n` characters remain.
  virtual void ignore(int n) = 0;
};


// -------------------------------------------------------------------------- //
// String stream

/// A string stream provides a facility of extracting values from a sequence
/// of symbols. The template parameter `S` is the type of a forward container.
/// Note that elements returned by the stream can be copied, but not modified
/// in place.
template<typename S, typename T = typename S::value_type>
struct string_stream : input_stream<T>
{
  using value_type = typename S::value_type;
  using iterator = typename S::const_iterator;

  static T end;

  string_stream(const S& str);

  bool eof() const override;
  const T& peek() const override;
  const T& peek(int) const override;
  T get() override;
  void ignore() override;
  void ignore(int) override;

  iterator first; // Current position
  iterator limit; // Past the end
};

// An element representing the end of input.
template<typename S, typename T>
T string_stream<S, T>::end{};

template<typename S, typename T>
string_stream<S, T>::string_stream(const S& str)
  : first(str.begin()), limit(str.end())
{ }

/// Returns true if there is no more input.
template<typename S, typename T>
bool 
string_stream<S, T>::eof() const
{ 
  return first == limit; 
}

/// Returns the lookahead character.
template<typename S, typename T>
const T&
string_stream<S, T>::peek() const
{
  if (eof())
    return end;
  else
    return *first;
}

/// Returns the nth character past the lookahead.
template<typename S, typename T>
const T&
string_stream<S, T>::peek(int n) const
{
  if (limit - first <= n)
    return end;
  else
    return *(first + n);
}

/// Buffers and returns the current character. Advances the stream.
template<typename S, typename T>
T
string_stream<S, T>::get()
{
  if (eof())
    return end;
  else
    return *first++;
}

/// Ignores the current character and advances the stream.
template<typename S, typename T>
void
string_stream<S, T>::ignore()
{
  if (!eof())
    ++first;
}

/// Ignores up to n characters.
template<typename S, typename T>
void
string_stream<S, T>::ignore(int n)
{
  if (limit - first <= n)
    first = limit;
  else
    first += n;
}

/// Returns a stream for the given string.
template<typename C, typename T>
string_stream<std::basic_string<C, T>> 
make_stream(const std::basic_string<C, T>& str)
{
  return string_stream<std::basic_string<C, T>>(str);
}

/// Returns a stream over the given vector.
///
/// \todo Use concepts.
template<typename T, typename A>
string_stream<std::vector<T, A>>
make_stream(const std::vector<T, A>& seq)
{
  return string_stream<std::vector<T, A>>(seq);
}

// -------------------------------------------------------------------------- //
// Input stream

/// Adapts the std::istream interface for character stream. 
struct istream_stream : input_stream<char>
{
  istream_stream(std::istream& is)
    : is(&is)
  { }

  bool eof() const override;
  const char& peek() const override;
  const char& peek(int) const override;
  char get() override;
  void ignore() override;
  void ignore(int) override;

  std::istream* is;
  std::string buf;
};


// -------------------------------------------------------------------------- //
// Lex stream

/// A lex stream provides a stream interface for a generator function. This
/// can be used, for example, to create a token stream on top of a lexing
/// function.
///
/// In order to support peeking, this class maintains a deque of extracted
/// characters. The front is always the current element. 
template<typename G, typename T = std::result_of_t<G()>>
struct generator_stream : input_stream<T>
{
  static T end;

  template<typename... Args>
  generator_stream(Args&&...);

  bool eof() const override;
  const T& peek() const override;
  const T& peek(int) const override;
  T get() override;
  void ignore() override;
  void ignore(int) override;

  const T& front();
  const T& back();
  void fetch();
  T extract();

  mutable G gen;  // The generating function
  mutable std::deque<T> buf; // Extracted elements
};

template<typename G, typename T>
T generator_stream<G, T>::end{};

/// Initialize the generator stream by constructing the underlying generator
/// in place. 
template<typename G, typename T>
template<typename... Args>
generator_stream<G, T>::generator_stream(Args&&... args)
  : gen(std::forward<Args>(args)...)
{
  fetch();
}

/// Returns the front element of the buffer.
template<typename G, typename T>
const T& generator_stream<G, T>::front() { return buf.front(); }

/// Returns the back element of the buffer.
template<typename G, typename T>
inline const T& generator_stream<G, T>::back() { return buf.back(); }

/// Puts a generated element into the buffer.
template<typename G, typename T>
inline void
generator_stream<G, T>::fetch()
{
  buf.push_back(gen());
}

/// Extracts the first element of the buffer.
template<typename G, typename T>
inline T
generator_stream<G, T>::extract()
{
  T ret = std::move(front());
  buf.pop_front();
  return ret;
}

/// Returns true when the generator is out of data. This is the case when the
/// current element does not convert to `true` (i.e., we are peeking at an
/// eof character or token).
template<typename G, typename T>
bool
generator_stream<G, T>::eof() const
{
  return front();
}

/// Returns the current element.
template<typename G, typename T>
const T&
generator_stream<G, T>::peek() const
{
  return front();
}

/// Returns the nth element past the front of the stream.
template<typename G, typename T>
const T&
generator_stream<G, T>::peek(int n) const
{
  if (n < buf.size()) 
    return buf[n];
  n -= buf.size() - 1;
  while (!eof() && n) {
    fetch();
    --n;
  }
  if (eof())
    return end;
  else
    return back();
}

/// Gets the next element from the generator, returning the previous element.
template<typename G, typename T>
T
generator_stream<G, T>::get() 
{
  T ret = extract();
  if (buf.empty())
    fetch();
  return ret;
}

template<typename G, typename T>
void
generator_stream<G, T>::ignore() 
{
  buf.pop_front();
  if (buf.empty())
    fetch();
}

template<typename G, typename T>
void
generator_stream<G, T>::ignore(int n) 
{
  // Burn through the buffer.
  while (!buf.empty() && n != 0) {
    buf.pop_front();
    --n;
  }

  // Burn any remaining elements.
  while (!eof() && n != 0)
    gen();

  // Pre-load the next element if needed; this may load an eof.
  if (buf.empty())
    fetch();
}

} // namespace beaker


#endif

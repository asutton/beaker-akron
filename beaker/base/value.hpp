// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_VALUE_HPP
#define BEAKER_BASE_VALUE_HPP

#include <cassert>
#include <cstdint>
#include <cstring>
#include <iosfwd>
#include <vector>


namespace beaker {

// Kinds of values.
enum value_kind 
{
  void_value_kind,
  int_value_kind,
  float_value_kind,
};

// Represents the absence of value.
struct void_value { };

// Represents integer values.
using integer_value = std::intmax_t;

// Represents floating point values.
using float_value = double;


// Representation of values.
union value_data
{
  explicit value_data() : v() { }
  explicit value_data(integer_value n) : z(n) { }
  explicit value_data(float_value n) : f(n) { }
  
  void_value v;
  integer_value z;
  float_value f;
};


// Represents the compile-time value of an object.
//
// Values are used to represent literals, constants, and to perform
// compile-time evaluation. A value can be one of several different 
// kinds: integers, reals, and composites.
struct value
{
  value();
  value(const value&);
  value(value&&);
  explicit value(int);
  explicit value(integer_value);
  explicit value(float_value);

  value& operator=(const value&);
  value& operator=(value&&);

  value_kind get_kind() const;
  bool is_void() const;
  bool is_integer() const;
  bool is_float() const;

  integer_value get_int() const;
  float_value get_float() const;

  value_kind kind_;
  value_data data_;
};

inline 
value::value() 
  : kind_(void_value_kind), data_()
{ }

inline 
value::value(const value& v)
  : kind_(v.kind_)
{
  std::memcpy(&data_, &v.data_, sizeof(value_data));
}

inline 
value::value(value&& v)
  : kind_(v.kind_)
{
  std::memcpy(&data_, &v.data_, sizeof(value_data));
}

// Initialize the value from the integer n.
//
// This allows initialization of the form value(0). Lookup would be ambiguous
// without this constructor.
inline
value::value(int n)
  : kind_(int_value_kind), data_(integer_value(n))
{ }

inline
value::value(integer_value n)
  : kind_(int_value_kind), data_(n)
{ }

inline
value::value(float_value n)
  : kind_(float_value_kind), data_(n)
{ }

inline value&
value::operator=(const value& v) 
{
  kind_ = v.kind_;
  std::memcpy(&data_, &v.data_, sizeof(value_data));
  return *this;
}

inline value&
value::operator=(value&& v) 
{
  kind_ = v.kind_;
  std::memcpy(&data_, &v.data_, sizeof(value_data));
  return *this;
}

/// Returns the kind of value.
inline value_kind value::get_kind() const { return kind_; }

/// Returns true if this is the void value.
inline bool value::is_void() const { return kind_ == void_value_kind; }

/// Returns the true if the value is an integer.
inline bool value::is_integer() const { return kind_ == int_value_kind; }

/// Returns true if the value is a floating point value.
inline bool value::is_float() const { return kind_ == float_value_kind; }

/// Returns the integer representation of the value.
inline integer_value
value::get_int() const 
{ 
  assert(is_integer());
  return data_.z;
}

// Returns the floating point representation of the value.
inline float_value
value::get_float() const 
{ 
  assert(is_float());
  return data_.f;
}


// -------------------------------------------------------------------------- //
// Equality comparison

/// Two values are when they have the same kind and underlying value.
inline bool
operator==(const value& a, const value& b)
{
  if (a.get_kind() != b.get_kind())
    return false;
  switch (a.get_kind()) {
    case void_value_kind:
      return true;
    case int_value_kind:
      return a.get_int() == b.get_int();
    case float_value_kind:
      return a.get_float() == b.get_float();
    default:
      break;
  }
  assert(false && "invalid value kind");
}

inline bool
operator!=(const value& a, const value& b)
{
  return !(a == b);
}


// -------------------------------------------------------------------------- //
// Hashing

struct hasher;

void hash(hasher&, const value&);


// -------------------------------------------------------------------------- //
// Streaming

std::ostream& operator<<(std::ostream&, const value&);

} // namespace beaker

#endif

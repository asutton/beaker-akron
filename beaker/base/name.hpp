// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_NAME_HPP
#define BEAKER_BASE_NAME_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/seq.hpp>
#include <beaker/util/cast.hpp>


namespace beaker {

// -------------------------------------------------------------------------- //
// Name base class

// Represents the set of declaration names.
struct name
{
  explicit name(int);
  virtual ~name() = default;

  int get_feature() const;
  int get_kind() const;

  int kind_;
};

// Construct a name with kind k.
inline name::name(int k) : kind_(k) { }

// Returns the language pack that defines the name.
inline int name::get_feature() const { return get_language(kind_); }

// Returns the kind of name.
inline int name::get_kind() const { return kind_; }

} // namespace beaker


#endif

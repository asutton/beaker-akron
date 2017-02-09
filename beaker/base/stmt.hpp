// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_STMT_HPP
#define BEAKER_BASE_STMT_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/seq.hpp>
#include <beaker/util/cast.hpp>


namespace beaker {

// -------------------------------------------------------------------------- //
// Statement base class

// Represents the set of statements in a language.
struct stmt 
{
  using node_set = stmt;

  explicit stmt(int);
  virtual ~stmt() = default;

  int get_feature() const;
  int get_kind() const;

  const int kind_;
};

// Construct an statement with kind k.
inline stmt::stmt(int k) : kind_(k) { }

// Returns the language pack that defines the statement.
inline int stmt::get_feature() const { return get_language(kind_); }

// Returns the kind of statement.
inline int stmt::get_kind() const { return kind_; }


// -------------------------------------------------------------------------- //
// Statement sequence

// A sequence of statements.
using stmt_seq = seq<stmt>;

} // namespace beaker


#endif

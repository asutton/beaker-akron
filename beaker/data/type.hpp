// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_DATA_TYPE_HPP
#define BEAKER_DATA_TYPE_HPP

#include <beaker/base/type.hpp>


namespace beaker {
namespace data {

enum 
{
  first_type_kind = data_lang,
#define def_type(t) t ## _type_kind,
#include "type.def"
  last_type_kind
};


/// Represents tuple types of the form tuple {t*}.
struct tuple_type : type
{
  static constexpr int node_kind = tuple_type_kind;

  tuple_type(const type_seq&);
  tuple_type(type_seq&&);

  const type_seq& get_element_types() const;
  type_seq& get_element_types();

  type_seq elems_;
};

/// Initialize the tuple type.
inline tuple_type::tuple_type(const type_seq& t)
  : type(node_kind), elems_(t)
{ }

/// Initialize the tuple type.
inline tuple_type::tuple_type(type_seq&& t)
  : type(node_kind), elems_(std::move(t))
{ }

/// Returns the sequence of element types.
inline const type_seq& tuple_type::get_element_types() const { return elems_; }

/// Returns the sequence of element types.
inline type_seq& tuple_type::get_element_types() { return elems_; }



/// Represents types of the form t[n].
struct array_type : type
{
  static constexpr int node_kind = array_type_kind;

  array_type(type&, int);

  const type& get_element_type() const;
  type& get_element_type();

  int get_extent() const;

  type* type_;
  int ext_;
};

/// Initialize the type.
inline 
array_type::array_type(type& t, int n)
  : type(node_kind), type_(&t), ext_(n)
{ }

/// Returns the element type of the array.
inline const type& array_type::get_element_type() const { return *type_; }

/// Returns the element type of the array.
inline type& array_type::get_element_type() { return *type_; }

/// Returns the extent of the array.
inline int array_type::get_extent() const { return ext_; }


/// Represents types of the form `t[]`.
///
/// A region of memory is essentially an array with no bound. The array bound
/// must be computed and maintained separately at run time.
struct seq_type : type
{
  static constexpr int node_kind = seq_type_kind;

  seq_type(type&);

  const type& get_element_type() const;
  type& get_element_type();

  type* type_;
};

/// Initialize the type.
inline seq_type::seq_type(type& t) : type(node_kind), type_(&t) { }

/// Returns the element type of the sequence.
inline const type& seq_type::get_element_type() const { return *type_; }

/// Returns the element type of the sequence.
inline type& seq_type::get_element_type() { return *type_; }


} // namespace data
} // namespace beaker


#endif

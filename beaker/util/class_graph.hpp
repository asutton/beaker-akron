// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_UTIL_CLASS_GRAPH_HPP
#define BEAKER_UTIL_CLASS_GRAPH_HPP

#include <cassert>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>


namespace beaker {

/// Represents the nodes in a class hierarchy (i.e., classes).
///
/// This currently allows multiple edges between base and derived classes;
/// as does C++.
///
/// \todo Enable multiple inheritance. See the assertion in the add_base.
///
/// \todo Support virtual inheritance. This may require edge types.
struct class_node
{
  using seq_type = std::vector<int>;

  class_node(std::type_index t)
    : ti(t)
  { }

  void add_derived(int);
  void add_base(int);

  /// Returns the indexes of base classes.
  const seq_type& bases() const { return base; }
  seq_type& bases() { return base; }

  /// Returns the number of base classes.
  int num_bases() const { return base.size(); }

  /// Returns true if this is the base class of a hierarchy.
  bool is_root() const { return num_bases() == 0; }

  /// Returns the indexes of inheriting classes.
  const seq_type& inheritors() const { return derived; }
  seq_type& inheritors() { return derived; }

  /// Returns the number of derived classes.
  int num_inheritors() const { return derived.size(); }

  /// Returns true if this is a leaf in a class hierarchy.
  int is_leaf() const { return num_inheritors() == 0; }

  std::type_index ti;
  seq_type base;
  seq_type derived;
  int id = -1;   // A computed index.
  int color = 0; // Scratch data used by algorithms
};

/// Add a reference to a derived class.
inline void 
class_node::add_derived(int n)
{
  derived.push_back(n);
}

/// Add a reference to  a base class.
inline void
class_node::add_base(int n)
{
  assert(num_bases() < 2);  // FIXME: Remove this.
  base.push_back(n);
}


/// Represents a graph describing a class hierarchy. This is used to provide
/// a framework for computing (among other things), a layout for dispatch
/// tables.
///
/// \todo Allow hierarchies to be merged?
///
/// \todo Make this sufficiently general that it can support class graphs
/// for source code analysis. That is, decouple it from the information used
/// to compute dispatch tables. In fact, refactor this as a small graph library
/// that we can re-purpose for any graph-related task.
struct inheritance_hierarchy 
{
  using seq_type = std::vector<class_node>;
  using map_type = std::unordered_map<std::type_index, int>;

  int size() const;
  
  // Vertex/edge creation
  void add_derivation(std::type_index, std::type_index);

  template<typename T, typename U> 
  void add_derivation();

  // Node access
  const class_node& node(int) const;
  class_node& node(int);

  const class_node& node(std::type_index) const;
  class_node& node(std::type_index);

  template<typename T> 
  const class_node& node() const;
  
  template<typename T> 
  class_node& node();

  // Class indexing.
  int index(class_node&) const;

  int require(std::type_index) const;
  int require(std::type_index);
  int get(std::type_index);

  seq_type::iterator begin() { return types.begin(); }
  seq_type::iterator end() { return types.end(); }
  seq_type::const_iterator begin() const { return types.begin(); }
  seq_type::const_iterator end() const { return types.end(); }

  seq_type types;
  map_type lookup;
};

/// Returns the size of the hierarchy (number of classes).
inline int inheritance_hierarchy::size() const { return types.size(); }

/// Inserts a new edge connecting the base class B to the derived class D.
template<typename B, typename D>
inline void 
inheritance_hierarchy::add_derivation()
{ 
  add_derivation(typeid(B), typeid(D)); 
}

/// Returns the node at the given index.
inline const class_node& inheritance_hierarchy::node(int n) const { return types[n]; }

/// Returns the node at the given index.
inline class_node& inheritance_hierarchy::node(int n) { return types[n]; }

/// Returns the class node for the given type index.
inline const class_node& inheritance_hierarchy::node(std::type_index ti) const { return node(require(ti)); }

/// Returns the class node for the given type index.
inline class_node& inheritance_hierarchy::node(std::type_index ti) { return node(require(ti)); }

/// Returns the class node for the type of the template argument T.
template<typename T> 
inline const class_node& inheritance_hierarchy::node() const { return node(typeid(T)); }

/// Returns the class node for the type of the template argument T.
template<typename T> 
inline class_node& inheritance_hierarchy::node() { return node(typeid(T)); }

/// Returns the index of a node.
inline int inheritance_hierarchy::index(class_node& c) const { return &c - types.data(); }

/// Returns the index of the node for the type information. 
inline int 
inheritance_hierarchy::require(std::type_index ti) const
{
  assert(lookup.count(ti) != 0);
  return lookup.find(ti)->second;
}

/// Returns the index of the node for the type information. 
inline int 
inheritance_hierarchy::require(std::type_index ti)
{
  assert(lookup.count(ti) != 0);
  return lookup.find(ti)->second;
}


void print(const inheritance_hierarchy&);


} // namespace beaker


#endif

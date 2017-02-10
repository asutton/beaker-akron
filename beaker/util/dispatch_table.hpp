// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_UTIL_DISPATCH_TABLE_HPP
#define BEAKER_UTIL_DISPATCH_TABLE_HPP

#include "class_graph.hpp"

#include <cassert>
#include <iosfwd>
#include <queue>
#include <vector>


namespace beaker {

/// Represents a dispatch table for an open multimethod with a single
/// polymorphic argument. The template parameter `F` is the signature of the
/// function being dispatched on.
///
/// This implementation strictly violates the type safety rules of C++, but is 
/// nonetheless morally correct, under certain assumptions. The type `F` is the 
/// subtype of all possible overriders: the "virtual" parameter is `F` is the 
/// superclass of all "virtual" parameters stored in the table. 
///
/// The type of all polymorphic arguments must have the base class as the first 
/// in their base-class lists. Trying to dispatch when the base class is not
/// first will not correctly adjust the object pointer on dispatch.
///
/// \todo Support multiple dispatch (this is harder).
///
/// \todo Implement a facility to appropriately downcast objects regardless
/// of inheritance order. This might be done by simply storing the address
/// offsets in the class hierarchy (we don't have to store it here).
template<typename F>
struct dispatch_table
{
  using fn_type = F;
  using table_type = std::vector<F*>;
  
  dispatch_table(inheritance_hierarchy&);

  template<typename T> fn_type* get_overrider(const T*) const;
 template<typename T> fn_type* get_overrider(const T&) const;

  template<typename T, typename G>
  void add_overrider(G*);

  template<typename G>
  void install_overrider(class_node&, G*);

  fn_type* fetch_overrider(const class_node&) const;

  inheritance_hierarchy* hier;
  table_type tab;
};

/// Initialize the dispatch table for the completed class hierarchy. Initially,
/// no entries have overriders.
template<typename F>
dispatch_table<F>::dispatch_table(inheritance_hierarchy& h)
  : hier(&h), tab(h.size(), nullptr)
{ }

/// Returns the overrider for the polymorphic argument.
template<typename F>
template<typename T>
inline auto
dispatch_table<F>::get_overrider(const T* arg) const -> fn_type*
{
  class_node& c = hier->node(typeid(*arg));
  fn_type* fn = fetch_overrider(c);
  assert(fn && "no overrider installed");
  return fn;
}

/// Returns the overrider for the polymorphic argument.
template<typename F>
template<typename T>
inline auto
dispatch_table<F>::get_overrider(const T& arg) const -> fn_type*
{
  return get_overrider(&arg);
}

/// Add an overrider for given type argument, and propagate that overrider to
/// all derived classes that have no current overrider.
///
/// The type of the "virtual" parameter in the type of `fn` must be a subclass 
/// of the "virtual" parameter in `fn_type`. That is, the type of `fn` must
/// be a subtype of `fn_type`.
///
/// \todo This not particularly efficient. It might be worthwhile to compute
/// the transitive closure of the inheritance relation so that we can
/// propagate overrides with a single loop.
template<typename F>
template<typename T, typename G>
void 
dispatch_table<F>::add_overrider(G* fn) 
{
  class_node& s = hier->node(typeid(T));
  std::queue<class_node*> q;
  q.push(&s);
  install_overrider(s, fn);

  while (!q.empty()) {
    class_node* u = q.front();
    q.pop();
    for (int d : u->inheritors()) {
      class_node* v = &hier->node(d);
      if (!fetch_overrider(*v)) {
        q.push(v);
        install_overrider(*v, fn);
      }
    }
  }
}

template<typename F>
template<typename G>
void
dispatch_table<F>::install_overrider(class_node& c, G* fn)
{
  assert(0 <= c.id && c.id < tab.size());
  tab[c.id] = reinterpret_cast<fn_type*>(fn);
}

/// \todo Failing to find an overrider should always trigger an error, not
/// just in debug mode.
template<typename F>
auto
dispatch_table<F>::fetch_overrider(const class_node& c) const -> fn_type*
{
  assert(0 <= c.id && c.id < tab.size());
  return tab[c.id];
}

template<typename C, typename T, typename F>
void 
print(std::basic_ostream<C, T>& os, const dispatch_table<F>& dt)
{
  for (std::size_t i = 0; i < dt.tab.size(); ++i)
    os << dt.hier->node(i).ti.name() << ' ' << (void*)dt.tab[i] << '\n';
}

} // namespace beaker


#endif

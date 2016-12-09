// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_COMMON_VISITATION_HPP
#define BEAKER_COMMON_VISITATION_HPP

#include <beaker/common/cast.hpp>
#include <beaker/common/type.hpp>
#include <beaker/common/expression.hpp>
#include <beaker/common/declaration.hpp>
#include <beaker/terms/logic/visitation.hpp>

// #include <beaker/terms/numeric/visitation.hpp>



namespace beaker {

// Provides a base class from which dispatch functions must be derived.
//
// The template parameter T is the return type of the function. This
// is also the type of the result_type member.
//
// This class guarantees that a dispatch target will always be found,
// although the behavior of those functions is undefined. 
template<typename T>
struct generic_fn
{
  using type = T;

  [[noreturn]] T operator()(const type&) { 
    assert(false && "unsupported type"); 
  }
  [[noreturn]] T operator()(type&) { 
    assert(false && "unsupported type"); 
  }
};


// Tag classes used to support dispatch.
struct void_tag { };
struct non_void_tag { };


// A facility used to dispatch to a function object.
template<typename F, typename T>
struct generic_visitor
{
  generic_visitor(F f)
    : fn(f), r()
  { }

  // Dispatch to the wrapped function object.  
  template<typename U> 
  void invoke(U const& u) { r = fn(u); }
  
  template<typename U> 
  void invoke(U& u) { r = fn(u); }

  T result() const { return r; }

  static non_void_tag tag() { return {}; }

  F fn;
  T r;
};

// Specialization for reference results.
template<typename F, typename T>
struct generic_visitor<F, T&>
{
  generic_visitor(F f)
    : fn(f), r()
  { }

  // Dispatch to the wrapped function object.
  template<typename U>
  void invoke(U const* u) { r = &fn(u); }

  template<typename U>
  void invoke(U const& u) { r = &fn(u); }

  T& result() const { return *r; }

  // Enable tag dispatch.
  static non_void_tag tag() { return {}; }

  F fn;
  T* r;
};

// Specialization for void results.
template<typename F>
struct generic_visitor<F, void>
{
  generic_visitor(F f)
    : fn(f)
  { }

  // Dispatch to the wrapped function object.
  template<typename U>
  void invoke(U const* u) { fn(u); }

  template<typename U>
  void invoke(U const& u) { fn(u); }

  static void_tag tag() { return {}; }

  F fn;
};


// Invoke the visitor and return a value.
template<typename T, typename V>
inline void
visit_type(T&& t, V& vis)
{
  switch (t.get_kind()) {
    case void_type_kind: 
      return vis.invoke(cast<void_type>(t));
    case ref_type_kind:
      return vis.invoke(cast<ref_type>(t));
    case fn_type_kind:
      return vis.invoke(cast<fn_type>(t));
    default:
      break;
  }
  assert(false && "type not supported");
}

/// Dispatch to a language handler for the given type.
///
/// \note This function must be updated when adding a new note.
template<typename T, typename V>
inline void
apply_type(T&& t, V& vis)
{
  switch (t.get_pack()) {
    case common_lang:
      return beaker::visit_type(t, vis);
    case logic_lang:
      return logic::visit_type(t, vis);
    case numeric_lang:
      // return numeric::visit_type(t, vis);
    case functional_lang:
      // return functional::visit_type(t, vis);
    case data_lang:
      // return data::visit_type(gen, t);
    default:
      break;
  }
  assert(false && "type not supported");
}

namespace detail {

template<typename V>
inline void
apply(const type& t, V& vis, void_tag)
{
  apply_type(t, vis);
}

template<typename V>
inline auto
apply(const type& t, V& vis, non_void_tag) -> decltype(vis.result())
{
   apply_type(t, vis);
   return vis.result();
}

} // namespace detail


template<typename F>
inline decltype(auto)
apply(const type& t, F fn)
{
  generic_visitor<F, typename F::type> vis(fn);
  return detail::apply(t, vis, vis.tag());
}

template<typename F>
inline decltype(auto)
apply(type& t, F fn)
{
  generic_visitor<F, typename F::type> vis(fn);
  return detail::apply(t, vis, vis.tag());
}

} // namespace beaker


#endif

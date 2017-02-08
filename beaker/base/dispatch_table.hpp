// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_UTIL_DISPATCH_TABLE_HPP
#define BEAKER_UTIL_DISPATCH_TABLE_HPP

#include <unordered_map>

namespace beaker {

struct name;
struct type;
struct expr;
struct decl;
struct stmt;


/// A dispatch table associates node a with a dispatch function.
struct dispatch_table
{
  using key_type = std::pair<char, int>;
  using map_type = std::unordered_map<key_type, void(*)()>;
  using fn_type = void(*)();

  enum {
    name_node, type_node, expr_node, decl_node, stmt_node
  };

  void add_target(const name&, fn_type);
  void add_target(const type&, fn_type);
  void add_target(const expr&, fn_type);
  void add_target(const decl&, fn_type);
  void add_target(const stmt&, fn_type);

  template<typename F> fn_type get_function(const name&);
  template<typename F> fn_type get_function(const type&);
  template<typename F> fn_type get_function(const expr&);
  template<typename F> fn_type get_function(const decl&);
  template<typename F> fn_type get_function(const stmt&);

  map_type map_;
};

inline void
add_target(const name& n, fn_type fn)
{
  map_.emplace({name_node, n.get_kind()}, fn);
}

/// Add a new dispatch method 
inline void
add_target(const type& t, fn_type fn)
{
  key_type key(type_node, t.get_kind());
  assert(!map_.count(key));
  map_.emplace(key, fn);
}

inline void
add_target(const expr& e, fn_type fn)
{
  map_.emplace({expr_node, e.get_kind()}, fn);
}

inline void
add_target(const decl& d, fn_type fn)
{
  map_.emplace({decl_node, d.get_kind()}, fn);
}

inline void
add_target(const stmt& s, fn_type fn)
{
  map_.emplace({stmt_node, s.get_kind()}, fn);
}

} // namespace beaker


#endif

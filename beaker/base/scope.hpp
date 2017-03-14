// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_SCOPE_HPP
#define BEAKER_BASE_SCOPE_HPP

#include <beaker/base/decl.hpp>

#include <cassert>
#include <memory>
#include <unordered_map>
#include <vector>


namespace beaker {

struct name;
struct decl;


// -------------------------------------------------------------------------- //
// Scope

/// Represents the sequence declared names within a region of text. A language
/// can derive from this class to provide specialized behaviors related to
/// the declaration and lookup. The kind member is used as a descriminator,
/// allowing those languages to determine properties of the scope.
struct scope : decl_seq
{
  scope();
  explicit scope(int);

  virtual ~scope() = default;

  int get_kind() const;

  void add(decl&);

  int kind;
};

/// Initialize a general purpose scope.
inline scope::scope() : kind(-1) { }

/// Initialize a scope of the given kind.
inline scope::scope(int k) : kind(k) { }

/// Returns the kind of scope.
inline int scope::get_kind() const { return kind; }


/// Add this declaration to the current scope.
inline void
scope::add(decl& d)
{
  push_back(d);
}



// -------------------------------------------------------------------------- //
// Lexcical environment

/// The lexical environment associates a name with its lexical binding (i.e.,
/// its declaration and some other information). This allows the language
/// to support declaration shadowing: the recycling of names for declarations
/// in narrow scopes. For example:
///
///     int x = 0;
///     {
///       int x = 1; // shadows #1
///       cout << x; // prints 1
///     }
///     cout << x; // prints 0
///
///
/// Note that the declaration may in fact be a declaration set or an overload
/// set. The semantics of these types must handled by the source language.
struct lexical_environment
{
  /// The list of declarations bound to a given name.
  struct bindings : std::vector<decl*>
  {
    const decl& top() const;
    decl& top();

    void push(decl&);
    void pop();
  };

  /// The stack of scopes, which maintain the list of declarations in each
  /// context.
  struct stack : std::vector<std::unique_ptr<scope>>
  {
    ~stack();

    void push(int k);
    void pop();

    const scope& top() const;
    scope& top();
  };

  using map_type = std::unordered_map<const name*, bindings>;

  void add(decl&);
  void remove(const name&);
  void remove(const decl&);

  const bindings* get_bindings(const name&) const;
  bindings* get_bindings(const name&);

  const decl* lookup(const name&) const;
  decl* lookup(const name&);

  void enter_scope(int k);
  void leave_scope();
  const scope& current_scope() const;
  scope& current_scope();

  map_type map;
  stack ss;
};

/// Returns the top (innermost) bindings for the name.
inline const decl& lexical_environment::bindings::top() const { return *back(); }

/// Returns the top (innermost) bindings for the name.
inline decl& lexical_environment::bindings::top() { return *back(); }

/// Push a new binding onto the stack.
inline void lexical_environment::bindings::push(decl& d) { push_back(&d); }

/// Pop the outermost binding from the stack.
inline void lexical_environment::bindings::pop() { pop_back(); }


inline 
lexical_environment::stack::~stack()
{
  assert(empty() && "imbalanced scope stack");
}

/// Push a new empty scope stack.
inline void
lexical_environment::stack::push(int k)
{
  push_back(std::make_unique<scope>(k));
}

/// Pop the current scope from the stack and remove all of the bindings
/// in that contour.
inline void
lexical_environment::stack::pop()
{
  assert(!empty() && "unbalanced scope stack");
  pop_back();
}

/// Returns the current scope.
inline const scope& 
lexical_environment::stack::top() const 
{ 
  return *back().get(); 
}

/// Returns the current scope.
inline scope& 
lexical_environment::stack::top() 
{ 
  return *back().get(); 
}

/// Returns the current scope.
inline const scope& lexical_environment::current_scope() const { return ss.top(); }

/// Returns the current scope.
inline scope& lexical_environment::current_scope() { return ss.top(); }

} // namespace beaker


#endif

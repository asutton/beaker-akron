// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_SCOPE_HPP
#define BEAKER_BASE_SCOPE_HPP

#include <cassert>
#include <memory>
#include <vector>


namespace beaker {

struct name;
struct decl;
struct type;
struct symbol;


// -------------------------------------------------------------------------- //
// Scope chain

/// A scope entry associates an entity with a declared name. This essentially
/// refers to the declaration introduced by the given name.
///
/// In general a language's scope entry will contain a name binding: the
/// the declaration of the name as a particular kind of entity.
///
/// FIXME: Do I need a reference to the enclosing contour?
struct scope_entry
{
  scope_entry(decl&);

  const decl& get_declaration() const;
  decl& get_declaration();

  decl* decl_;
};

inline scope_entry::scope_entry(decl& d) : decl_(&d) { }

/// Returns the declaration introduced by the symbol.
inline const decl& scope_entry::get_declaration() const { return *decl_; }

/// Returns the declaration introduced by the symbol.
inline decl& scope_entry::get_declaration() { return *decl_; }


/// The scope chain is stack of records that associates meaning with a symbol
/// in a symbol table.
struct scope_chain : std::vector<std::unique_ptr<scope_entry>>
{
  const scope_entry& top() const;
  scope_entry& top();

  template<typename T, typename... Args>
  T& push(Args&&... args);

  void pop();
};

/// Returns the top of the stack.
inline const scope_entry&
scope_chain::top() const
{
  assert(!empty());
  return *back();
}

/// Returns the top of the stack.
inline scope_entry&
scope_chain::top()
{
  assert(!empty());
  return *back();
}

/// Push a new scope entry of type T (derived from scope_entry) with the given 
/// arguments.
template<typename T, typename... Args>
inline T& 
scope_chain::push(Args&&... args)
{
  emplace_back(new T(std::forward<Args>(args)...));
}

/// Pops the entry at the top of the stack.
inline void
scope_chain::pop()
{
  assert(!empty());
  pop_back();
}


// -------------------------------------------------------------------------- //
// Scope

/// Represents the sequence declared names within a region of text. A language
/// can derive from this class to provide specialized behaviors related to
/// the declaration and lookup. The kind member is used as a descriminator,
/// allowing those languages to determine properties of the scope.
struct scope : std::vector<const symbol*>
{
  scope();
  explicit scope(int);

  virtual ~scope();

  int get_kind() const;

  template<typename T, typename... Args>
  T& add(const symbol& sym, Args&&... args);

  void remove(const symbol& sym);

  scope_chain& get_bindings(const symbol&);

  int kind;
};

/// Initialize a general purpose scope.
inline scope::scope() : kind(-1) { }

/// Initialize a scope of the given kind.
inline scope::scope(int k) : kind(k) { }

/// Remove all bindings added to this contour during its lifetime.
inline 
scope::~scope()
{
  for (const symbol* sym : *this)
    remove(*sym);
}

/// Returns the kind of scope.
inline int scope::get_kind() const { return kind; }

/// Add a new binding to this contour.
template<typename T, typename... Args>
inline T&
scope::add(const symbol& sym, Args&&... args)
{
  scope_chain& chain = get_bindings(sym);
  return chain.push<T>(std::forward<Args>(args)...);
}

/// Remove a binding from this contour.
inline void
scope::remove(const symbol& sym)
{
  scope_chain& chain = get_bindings(sym);
  chain.pop();
}


// -------------------------------------------------------------------------- //
// Scope stack

/// The scope stack maintains a stack of contours, which maintains the list
/// of names declared within a region of text.
///
/// \todo Support the allocation different kinds of scopes. These should
/// have a nice object-oriented flavor to them, and they can be specialized
/// to enforce language-specific declaration policies.
struct scope_stack : std::vector<scope*>
{
  ~scope_stack();

  void push(int k);
  void pop();

  const scope& top() const;
  scope& top();
};

inline 
scope_stack::~scope_stack()
{
  assert(empty() && "imbalanced scope stack");
}

/// Push a new empty scope stack.
inline void
scope_stack::push(int k)
{
  push_back(new scope(k));
}

/// Pop the current scope from the stack and remove all of the bindings
/// in that contour.
inline void
scope_stack::pop()
{
  assert(!empty() && "unbalanced scope stack");
  scope* s = back();
  pop_back();
  delete s;
}

/// Returns the current scope.
inline const scope& scope_stack::top() const { return *back(); }

/// Returns the current scope.
inline scope& scope_stack::top() { return *back(); }


} // namespace beaker


#endif

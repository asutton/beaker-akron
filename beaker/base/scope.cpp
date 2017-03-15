// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "scope.hpp"
#include "name.hpp"

#include "printing/print.hpp"

#include <iostream>


namespace beaker {

/// Adds a new innermost binding to the named declaration d.
///
/// \todo What if we want to merge d into the current binding level (i.e.,
/// generate a declaration or overload set?).
void
lexical_environment::add(decl& d)
{
  named_decl& nd = *d.as_named();

  // Insert d into the current scope.
  scope& s = current_scope();
  s.add(d);

  // Push the declaration onto the bindings.
  auto result = map.emplace(&nd.get_name(), bindings{});
  bindings& decls = result.first->second;
  decls.push(s, d);

}

/// Removes the innermost binding for n.
void
lexical_environment::remove(const name& n)
{
  bindings* decls = get_bindings(n);
  decls->pop();
}

/// Removes the innermost binding for d's name. This does not remove the
/// name from the scope.
///
/// \todo Should we also remove the declaration from the current scope. That
/// seems drastic and unreasonable. This is only ever called (I hope) from
/// leave_scope().
void
lexical_environment::remove(const decl& d)
{
  return remove(d.as_named()->get_name());
}

/// Returns the bindings for the given name.
auto
lexical_environment::get_bindings(const name& n) const -> const bindings*
{
  auto iter = map.find(&n);
  if (iter != map.end())
    return &iter->second;
  else
    return nullptr;
}

/// Returns the bindings for the given name.
auto
lexical_environment::get_bindings(const name& n) -> bindings*
{
  auto iter = map.find(&n);
  if (iter != map.end())
    return &iter->second;
  else
    return nullptr;
}


/// Returns the innermost declaration for the name.
auto
lexical_environment::lookup(const name& n) const -> const entry*
{
  if (const bindings* decls = get_bindings(n))
    return &decls->top();
  else
    return nullptr;
}

/// Returns the innermost declaration for the name.
auto
lexical_environment::lookup(const name& n) -> entry*
{
  if (bindings* decls = get_bindings(n))
    return &decls->top();
  else
    return nullptr;
}

/// Enter a new scope.
///
/// \todo: Allow this to dynamically allocate derived scopes.
void
lexical_environment::enter_scope(int k)
{
  ss.push(k);
}

/// Leave the current scope. This removes the innermost binding for each
/// declaration in the scope.
void
lexical_environment::leave_scope()
{
  scope& s = current_scope();
  for (decl& d : s)
    remove(d);
  ss.pop();
}

} // namespace beaker

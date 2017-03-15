// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_DECL_HPP
#define BEAKER_BASE_DECL_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/seq.hpp>
#include <beaker/util/cast.hpp>

#include <iosfwd>


namespace beaker {

struct name;
struct type;
struct expr;
struct decl;
struct named_decl;
struct typed_decl;
struct stmt;

/// The type of a unique identifier.
using uid = int;

/// A declaration's context is the declaration in which the declaration
/// appears. The semantic and lexical contexts of a module are both null,
/// indicating that it is the root declaration context.
struct dc
{
  dc();
  dc(decl&);
  dc(decl&, decl&);

  const decl* get_context() const;
  decl* get_context();

  const decl* get_semantic_context() const;
  decl* get_semantic_context();

  const decl* get_lexical_context() const;
  decl* get_lexical_context();

  decl* sema_;
  decl* lex_;
};

/// Initialize an empty declaration context. This should only be used by the
/// constructor of the `module` class.
inline dc::dc() : sema_(), lex_() { }

/// Initialize this declaration context with the same semantic and lexical
/// contexts.
inline dc::dc(decl& d) : sema_(&d), lex_(&d) { }

/// Initialize this declaration context with different semantic and lexical 
/// contexts.
inline dc::dc(decl& s, decl& l) : sema_(&s), lex_(&l) { }

/// Returns the semantic declaration context, or nullptr if this is the root
/// declaration context.
inline const decl* dc::get_context() const { return sema_; }

/// Returns the semantic declaration context.
inline decl* dc::get_context() { return sema_; }

/// Returns the semantic declaration context.
inline const decl* dc::get_semantic_context() const { return sema_; }

/// Returns the semantic declaration context.
inline decl* dc::get_semantic_context() { return sema_; }

/// Returns the lexical declaration context.
inline const decl* dc::get_lexical_context() const { return lex_; }

/// Returns the lexical declaration context.
inline decl* dc::get_lexical_context() { return lex_; }


/// Represents the set of all declarations.
///
/// Every declaration has an id that uniquely identifies it within a module.
/// This is assigned at the time of creation.
///
/// Each declaration is made within the context of some other context, except 
/// modules which are not owned by any other declaration. For example, a global 
/// function is declared within the context of a module; a local variable is
/// declared within the context of a function (even though it may be scoped
/// within a sequence of nested functions).
struct decl
{
  using node_set = decl;

  explicit decl(int);
  explicit decl(int, uid, dc);
  virtual ~decl() = default;

  int get_feature() const;  
  int get_kind() const;

  int get_id() const;

  const decl* get_context() const;
  decl* get_context();
  
  const decl* get_semantic_context() const;
  decl* get_semantic_context();
  
  const decl* get_lexical_context() const;
  decl* get_lexical_context();

  const module& get_module() const;
  module& get_module();

  const named_decl* as_named() const;
  named_decl* as_named();

  const typed_decl* as_typed() const;
  typed_decl* as_typed();
  
  int kind_;
  int id_;
  dc cxt_;
};

/// Initialize the declaration with kind k and a null context. This should 
/// only ever be used by the constructor of the `module` class.
inline decl::decl(int k) : kind_(k), id_(0), cxt_() { }

/// Initialize the declaration.
inline decl::decl(int k, uid id, dc cxt) : kind_(k), id_(id), cxt_(cxt) { }

/// Returns the language pack of the declaration.
inline int decl::get_feature() const { return get_language(kind_); }

/// Returns the declaration's kind.
inline int decl::get_kind() const { return kind_; }

/// Returns the unique id of the declaration.
inline int decl::get_id() const { return id_; }

/// Returns the semantic context of the declaration.
inline const decl* decl::get_context() const { return cxt_.get_context(); }

/// Returns the semantic context of the declaration.
inline decl* decl::get_context() { return cxt_.get_context(); }

/// Returns the semantic context of the declaration.
inline const decl* decl::get_semantic_context() const { return cxt_.get_semantic_context(); }

/// Returns the semantic context of the declaration.
inline decl* decl::get_semantic_context() { return cxt_.get_semantic_context(); }

/// Returns the lexical context of the declaration.
inline const decl* decl::get_lexical_context() const { return cxt_.get_lexical_context(); }

/// Returns the lexical context of the declaration.
inline decl* decl::get_lexical_context() { return cxt_.get_lexical_context(); }


// -------------------------------------------------------------------------- //
// Declaration sequences

// A sequence of declarations.
using decl_seq = seq<decl>;


// -------------------------------------------------------------------------- //
// Named declarations

// Determine how names are linked between translation units.
enum linkage {
  /// The name has no linkage
  no_link,
  // The name is available between translation units
  external_link,
  // The name is available only within the defining translation
  internal_link,
  // The name is not available outside the defining translation
  private_link,
};

// Represents the set of declarations that bind a name to an entity.
struct named_decl : decl
{
  named_decl(int, uid, dc, linkage, name&);

  void set_linkage(linkage);
  linkage get_linkage() const;
  bool has_linkage() const;
  bool has_external_linkage() const;
  bool has_internal_linkage() const;
  bool has_private_linkage() const;

  const name& get_name() const;
  name& get_name();

  linkage link_;
  name* name_;
};

/// Initialize the named declaration.
inline 
named_decl::named_decl(int k, uid id, dc cxt, linkage l, name& n) 
  : decl(k, id, cxt), link_(l), name_(&n) 
{ }

/// Returns the declaration's name.
inline const name& named_decl::get_name() const { return *name_; }

/// Returns the declaration's name.
inline name& named_decl::get_name() { return *name_; }

/// Set the linkage for the declaration.
inline void named_decl::set_linkage(linkage l) { link_ = l; }

/// Returns the linkage for the declaration.
inline linkage named_decl::get_linkage() const { return link_; }

/// Returns true if the name has any kind of linkage.
inline bool named_decl::has_linkage() const { return link_ != no_link; }

/// Returns true if the name has external linkage.
inline bool named_decl::has_external_linkage() const { return link_ == external_link; }

/// Returns true if the name has internal linkage.
inline bool named_decl::has_internal_linkage() const { return link_ == internal_link; }

/// Returns true if the name has private linkage.
inline bool named_decl::has_private_linkage() const { return link_ == private_link; }


// A helper class for declaring named declarations.
template<int K>
struct named_decl_impl : named_decl
{
  static constexpr int node_kind = K;

  named_decl_impl(uid, dc, linkage, name&);
};

template<int K>
inline
named_decl_impl<K>::named_decl_impl(uid id, dc cxt, linkage l, name& n)
  : named_decl(K, id, cxt, l, n)
{ }


// -------------------------------------------------------------------------- //
// Typed declarations

/// Represents the set of declarations of a name as a typed entity.
///
/// Typed entities associate a value either directly (e.g., a constant) or
/// indirectly (i.e., stored in a variable or accessed by a reference), or
/// are functions.
struct typed_decl : named_decl
{
  typed_decl(int, uid, dc, linkage, name&, type&);

  const type& get_type() const;
  type& get_type();

  type* type_;
};

/// Initialize the typed declaration.
inline
typed_decl::typed_decl(int k, uid id, dc cxt, linkage l, name& n, type& t)
  : named_decl(k, id, cxt, l, n), type_(&t)
{ }

/// Returns the type of the declaration.
inline const type& typed_decl::get_type() const { return *type_; }

/// Returns the type of the declaration.
inline type& typed_decl::get_type() { return *type_; }


/// A helper class for defining typed declarations.
template<int K>
struct typed_decl_impl : typed_decl
{
  static constexpr int node_kind = K;

  typed_decl_impl(uid, dc, linkage, name&, type&);
};

template<int K>
inline
typed_decl_impl<K>::typed_decl_impl(uid id, dc cxt, linkage l, name& n, type& t)
  : typed_decl(K, id, cxt, l, n, t)
{ }


// -------------------------------------------------------------------------- //
// Value declarations

/// The storage class of a value declaration determines the time that an
/// object's storage persists.
enum storage {
  /// Storage is allocated at the beginning of a function call and is 
  /// deallocated when that function ends.
  automatic_storage,
  /// Storage is allocated when the program begins and is deallocated when
  /// the program terminates.
  static_storage,
  /// Storage is allocated when a thread begins and is deallocated when the
  /// thread is ends.
  thread_storage,
};

/// Returns a default linkage specifier based on a storage class. Objects
/// with automatic storage do not have linkage.
inline linkage
get_default_linkage(storage s)
{
  if (s == automatic_storage)
    return no_link;
  else
    return external_link;
}


/// A value declaration associates a name with a value.
///
/// \todo Merge the storage for the value class with the storage for the
/// linkage specifier to save memory.
struct value_decl : typed_decl
{
  value_decl(int, uid, dc, storage, name&, type&);
  value_decl(int, uid, dc, linkage, storage, name&, type&, expr&);

  bool has_initializer() const;
  const expr& get_initializer() const;
  expr& get_initializer();

  void set_storage(storage);
  storage get_storage() const;
  bool has_static_storage() const;
  bool has_automatic_storage() const;
  bool has_thread_storage() const;

  storage storage_;
  expr* init_;
};

/// Initialize this variable with no definition and default linkage. Note
/// that this allows the construction of undefined automatic variables, which
/// is useful for declaring parameters.
inline
value_decl::value_decl(int k, uid id, dc cxt, storage s, name& n, type& t)
  : typed_decl(k, id, cxt, get_default_linkage(s), n, t), storage_(s), init_()
{ }

/// Initialize this value declaration. Values with automatic storage shall
/// have no linkage.
inline
value_decl::value_decl(int k, uid id, dc cxt, linkage l, storage s, name& n, type& t, expr& e)
  : typed_decl(k, id, cxt, l, n, t), storage_(s), init_(&e)
{
  assert(s == automatic_storage ? l == no_link : true);
}

/// Returns true when declaration has a local definition.
inline bool value_decl::has_initializer() const { return init_; }

/// Returns the initializer of the value declaration. This is defined only
/// when has_initializer() is true.
inline const expr& value_decl::get_initializer() const { return *init_; }

/// Returns the initializer of the value declaration. This is defined only
/// when has_initializer() is true.
inline expr& value_decl::get_initializer() { return *init_; }

/// Set the storage class of the declaration.
inline void value_decl::set_storage(storage s) { storage_ = s; }

/// Returns the storage class of the declaration.
inline storage value_decl::get_storage() const { return storage_; }

/// Returns true when the declaration has static storage.
inline bool value_decl::has_static_storage() const { return storage_ == static_storage; }

/// Returns true when the declaration has automatic storage.
inline bool value_decl::has_automatic_storage() const { return storage_ == automatic_storage; }

/// Returns true when the declaration has thread storage.
inline bool value_decl::has_thread_storage() const { return storage_ == thread_storage; }


// A helper class for declarations bound to values.
//
// Examples include variables and constants.
template<int K>
struct value_decl_impl : value_decl
{
  static constexpr int node_kind = K;

  value_decl_impl(uid, dc, storage, name&, type&);
  value_decl_impl(uid, dc, linkage, storage, name&, type&, expr&);
};

template<int K>
inline
value_decl_impl<K>::value_decl_impl(uid id, dc cxt, storage s, name& n, type& t)
  : value_decl(K, id, cxt, s, n, t)
{ }

template<int K>
inline
value_decl_impl<K>::value_decl_impl(uid id, dc cxt, linkage l, storage s, name& n, type& t, expr& e)
  : value_decl(K, id, cxt, l, s, n, t, e)
{ }


// -------------------------------------------------------------------------- //
// Mapping declarations

/// Represents the mapping of inputs to outputs. Examples of mappings are 
/// functions and axioms.
///
/// Mappings can be defined by either expressions (to support functional
/// programming) or statements (to support imperative programming).
struct mapping_decl : typed_decl
{
  mapping_decl(int, uid, dc, name&, type&, const decl_seq&, decl&);
  mapping_decl(int, uid, dc, name&, type&, decl_seq&&, decl&);
  mapping_decl(int, uid, dc, linkage, name&, type&, const decl_seq&, decl&, stmt&);
  mapping_decl(int, uid, dc, linkage, name&, type&, decl_seq&&, decl&, stmt&);

  const decl_seq& get_parameters() const;
  decl_seq& get_parameters();
  
  const decl& get_return() const;
  decl& get_return();

  const type& get_return_type() const;
  type& get_return_type();

  bool has_definition() const;
  const stmt& get_definition() const;
  stmt& get_definition();

private:
  void finish_parms();
public:

  decl_seq parms_;
  decl* ret_;
  stmt* def_;
};

// Parameters are generally created without contexts. Set the semantic context
// of each parameter to this functions.
inline void
mapping_decl::finish_parms()
{
  for (decl& p : parms_) {
    assert(!p.get_context());
    p.cxt_ = dc(*this);
  }
  ret_->cxt_ = dc(*this);
}

/// Initialize this mapping declaration, having no definition. When not
/// defined, the mapping has external linkage by default.
inline
mapping_decl::mapping_decl(int k, uid id, dc cxt, name& n, type& t, const decl_seq& p, decl& r)
  : typed_decl(k, id, cxt, external_link, n, t), parms_(p), ret_(&r), def_()
{
  finish_parms();
}

/// Initialize this mapping declaration, having no definition. When not
/// defined, the mapping has external linkage by default.
inline
mapping_decl::mapping_decl(int k, uid id, dc cxt, name& n, type& t, decl_seq&& p, decl& r)
  : typed_decl(k, id, cxt, external_link, n, t), parms_(std::move(p)), ret_(&r), def_()
{ 
  finish_parms();
}

/// Initialize this mapping declaration.
inline
mapping_decl::mapping_decl(int k, uid id, dc cxt, linkage l, name& n, type& t, const decl_seq& p, decl& r, stmt& s)
  : typed_decl(k, id, cxt, l, n, t), parms_(p), ret_(&r), def_(&s)
{ 
  finish_parms();
}

/// Initialize this mapping declaration.
inline
mapping_decl::mapping_decl(int k, uid id, dc cxt, linkage l, name& n, type& t, decl_seq&& p, decl& r, stmt& s)
  : typed_decl(k, id, cxt, l, n, t), parms_(std::move(p)), ret_(&r), def_(&s)
{ 
  finish_parms();
}

/// Returns the mapping's parameters.
inline const decl_seq& mapping_decl::get_parameters() const { return parms_; }

/// Returns the mapping's parameters.
inline decl_seq& mapping_decl::get_parameters() { return parms_; }

/// Returns the mappings return declaration.
inline const decl& mapping_decl::get_return() const { return *ret_; }

/// Returns the mappings return declaration.
inline decl& mapping_decl::get_return() { return *ret_; }

/// Returns true if the function has a definition.
inline bool mapping_decl::has_definition() const { return def_; }

/// Returns the definition of the mapping. This is valid only if the function
/// has_definition() returns true.
inline const stmt& mapping_decl::get_definition() const { return *def_; }

/// Returns the definition of the mapping. This is valid only if the function
/// has_definition() returns true.
inline stmt& mapping_decl::get_definition() { return *def_; }


// A helper class for defining mapping declarations.
template<int K>
struct mapping_decl_impl : mapping_decl
{
  static constexpr int node_kind = K;

  mapping_decl_impl(uid, dc, name&, type&, const decl_seq&, decl&);
  mapping_decl_impl(uid, dc, name&, type&, decl_seq&&, decl&);
  mapping_decl_impl(uid, dc, linkage, name&, type&, const decl_seq&, decl&, stmt&);
  mapping_decl_impl(uid, dc, linkage, name&, type&, decl_seq&&, decl&, stmt&);
};

template<int K>
inline
mapping_decl_impl<K>::mapping_decl_impl(uid id, dc cxt, name& n, type& t, const decl_seq& p, decl& r)
  : mapping_decl(K, id, cxt, n, t, p, r)
{ }

template<int K>
inline
mapping_decl_impl<K>::mapping_decl_impl(uid id, dc cxt, name& n, type& t, decl_seq&& p, decl& r)
  : mapping_decl(K, id, cxt, n, t, std::move(p), r)
{ }

template<int K>
inline
mapping_decl_impl<K>::mapping_decl_impl(uid id, dc cxt, linkage l, name& n, type& t, const decl_seq& p, decl& r, stmt& s)
  : mapping_decl(K, id, cxt, l, n, t, p, r, s)
{ }

template<int K>
inline
mapping_decl_impl<K>::mapping_decl_impl(uid id, dc cxt, linkage l, name& n, type& t, decl_seq&& p, decl& r, stmt& s)
  : mapping_decl(K, id, cxt, l, n, t, std::move(p), r, s)
{ }


// -------------------------------------------------------------------------- //
// Operations

/// Returns the declaration name for `d`. Requires that `d` is a named 
/// declaration.
inline const name&
get_declaration_name(const decl& d)
{
  return d.as_named()->get_name();
}

/// Returns the declaration name for `d`. Requires that `d` is a named 
/// declaration.
inline name&
get_declaration_name(decl& d)
{
  return d.as_named()->get_name();
}

/// Returns the declared type `d`. Requires that `d` is a typed declaration.
inline const type&
get_declared_type(const decl& d)
{
  return d.as_typed()->get_type();
}

/// Returns the declared type `d`. Requires that `d` is a typed declaration.
inline type&
get_declared_type(decl& d)
{
  return d.as_typed()->get_type();
}


} // namespace beaker


#endif

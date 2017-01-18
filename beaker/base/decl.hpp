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
struct stmt;
struct named_decl;
struct typed_decl;

// -------------------------------------------------------------------------- //
// Definitions

/// Represents an optional definition of a declaration.
///
/// Most declarations have an associated definition (i.e., the precise meaning 
/// associated with a name). The definition can be local or remote to the
/// current module. Remote definitions must be supplied at link time. 
///
/// Additionally, some declarations may support multiple kinds of declaration. 
/// For example, a function can be defined by a compound statement or an
/// expression. In general, we expect that a language will normalize the 
/// definitions so that each declaration has only a single kind. That is,
/// functions defined by an expression e can be easily rewritten as a function
/// defined by a compound statement that simply returns e.
struct defn
{
  defn();
  defn(int, void*);

  bool is_absent() const;
  bool is_present() const;

  int get_kind() const;

  template<typename T> const T& get_as() const;
  template<typename T> T& get_as();
  
  int kind_;
  void* term_;
};

inline defn::defn() : kind_(-1), term_() { }

inline defn::defn(int k, void* p) : kind_(k), term_(p) { }

/// Returns true if the definition is absent.
inline bool defn::is_absent() const { return term_ == nullptr; }

/// Returns true if the definition is present.
inline bool defn::is_present() const { return term_ != nullptr; }

/// Returns the kind of definition.
///
/// This is valid only when the declaration is internal.
inline int defn::get_kind() const { return kind_; }

/// Returns the definition cast as T.
///
/// This is valid only when the declaration is internal.
template<typename T>
inline const T& defn::get_as() const { return *reinterpret_cast<const T*>(term_); }

/// Returns the definition cast as T.
///
/// This is valid only when the declaration is internal.
template<typename T>
inline T& defn::get_as() { return *reinterpret_cast<T*>(term_); }


// -------------------------------------------------------------------------- //
// Declaration base class

/// Represents the set of all declarations.
///
/// Each declaration is made within the context of some other context, except 
/// modules which are not owned by any other declaration. For example, a global 
/// function is declared within the context of a module; a local variable is
/// declared within the context of a function (even though it may be scoped
/// within a sequence of nested functions).
///
/// \todo Support the notion of semantic and lexical contexts as in C++.
struct decl
{
  explicit decl(int);
  explicit decl(int, decl&);
  virtual ~decl() = default;

  int get_feature() const;  
  int get_kind() const;

  const decl* get_context() const;
  decl* get_context();

  const module& get_module() const;
  module& get_module();

  const named_decl* as_named() const;
  named_decl* as_named();

  const typed_decl* as_typed() const;
  typed_decl* as_typed();
  
  int kind_;
  decl* cxt_;
};

/// Initialize the declaration with kind k and a null context. This should 
/// only ever be used by the module class.
///
/// \todo Require opting in to this constructor using a fake argument.
inline decl::decl(int k) : kind_(k), cxt_() { }

/// Initialize the declaration with kind k and context c.
inline decl::decl(int k, decl& c) : kind_(k), cxt_(&c) { }

/// Returns the language pack of the declaration.
inline int decl::get_feature() const { return get_language(kind_); }

/// Returns the declaration's kind.
inline int decl::get_kind() const { return kind_; }

/// Returns the context of the declaration, or nullptr if this is a module.
inline const decl* decl::get_context() const { return cxt_; }

/// Returns the context of the declaration, or nullptr if this is a module.
inline decl* decl::get_context() { return cxt_; }


// -------------------------------------------------------------------------- //
// Declaration sequences

// A sequence of declarations.
using decl_seq = seq<decl>;


// -------------------------------------------------------------------------- //
// Named declarations

// Determine how names are linked between translation units.
enum {
  // Standard forms of linkage
  no_link,        // The name has no linkage
  external_link,  // The symbol is visible between translation units
  internal_link,  // The symbol is visible only within the defining translation

  // Non-standard forms of linkage
  private_link,   // The symbol is not visible outside the defining translation
};

// Represents the set of declarations that bind a name to an entity.
struct named_decl : decl
{
  named_decl(int, name&);

  void set_linkage(int);
  int get_linkage() const;
  bool has_linkage() const;
  bool has_external_linkage() const;
  bool has_internal_linkage() const;
  bool has_private_linkage() const;

  const name& get_name() const;
  name& get_name();

  int link_;
  name* name_;
};

// Initialize the named declaration with kind k and name n.
inline 
named_decl::named_decl(int k, name& n) 
  : decl(k), link_(external_link), name_(&n) 
{ }

// Returns the declaration's name.
inline const name& named_decl::get_name() const { return *name_; }

// Returns the declaration's name.
inline name& named_decl::get_name() { return *name_; }

// Set the linkage for the declaration.
inline void named_decl::set_linkage(int l) { link_ = l; }

inline int named_decl::get_linkage() const { return link_; }

// Returns true if the name has any kind of linkage.
inline bool named_decl::has_linkage() const { return link_ != no_link; }

// Returns true if the name has external linkage.
inline bool named_decl::has_external_linkage() const { return link_ != no_link; }

// Returns true if the name has internal linkage.
inline bool named_decl::has_internal_linkage() const { return link_ != no_link; }

//Returns true if the name has private linkage.
inline bool named_decl::has_private_linkage() const { return link_ != no_link; }


// A helper class for declaring named declarations.
template<int K>
struct generic_named_decl : named_decl
{
  static constexpr int node_kind = K;

  generic_named_decl(name&);
};

template<int K>
inline
generic_named_decl<K>::generic_named_decl(name& n)
  : named_decl(K, n)
{ }


// -------------------------------------------------------------------------- //
// Typed declarations

/// Represents the set of declarations of a name as a typed entity.
///
/// Typed entities associate a value either directly (e.g., a constant) or
/// indirectly (i.e., stored in a variable or accessed by a reference), or
/// are functions.
///
/// The typed entity has an associated storage class.
struct typed_decl : named_decl
{
  typed_decl(int, name&, type&);

  const type& get_type() const;
  type& get_type();

  type* type_;
};

// Initialize the typed declaration with kind k, name n, and type t.
//
// The storage class is initially constant.
inline
typed_decl::typed_decl(int k, name& n, type& t)
  : named_decl(k, n), type_(&t)
{ }

// Returns the type of the declaration.
inline const type& typed_decl::get_type() const { return *type_; }

// Returns the type of the declaration.
inline type& typed_decl::get_type() { return *type_; }


// A helper class for declaring named declarations.
template<int K>
struct generic_typed_decl : typed_decl
{
  static constexpr int node_kind = K;

  generic_typed_decl(name&, type&);
};

template<int K>
inline
generic_typed_decl<K>::generic_typed_decl(name& n, type& t)
  : typed_decl(K, n, t)
{ }


// -------------------------------------------------------------------------- //
// Value declarations


// Storage classes.
enum {
  constant_storage,
  static_storage,
  automatic_storage,
  thread_storage,
};


/// A value declaration associates a name with a value.
///
/// Examples include variables and constants.
///
/// \todo Can an initializer be a non-expression? It could be a constant.
struct value_decl : typed_decl
{
  enum {
    /// The value is initialized by an expression, very specifically by
    /// an initialization expression.
    expr_defn,
  };

  value_decl(int, name&, type&);
  value_decl(int, name&, type&, expr&);

  bool has_initializer() const;
  defn get_initializer() const;

  void set_storage(int);
  int get_storage() const;
  bool has_storage() const;
  bool has_static_storage() const;
  bool has_automatic_storage() const;
  bool has_thread_storage() const;

  defn def_;
  int storage_;
};

inline
value_decl::value_decl(int k, name& n, type& t)
  : typed_decl(k, n, t), def_(), storage_(automatic_storage)
{ }

inline
value_decl::value_decl(int k, name& n, type& t, expr& e)
  : typed_decl(k, n, t), def_(expr_defn, &e), storage_(automatic_storage)
{ }

/// Returns true when declaration has a local definition.
inline bool value_decl::has_initializer() const { return def_.is_present(); }

// Returns the initializer of the value declaration.
inline defn value_decl::get_initializer() const { return def_; }

// Set the storage class of the declaration.
inline void value_decl::set_storage(int sc) { storage_ = sc; }

// Returns the storage class of the declaration.
inline int value_decl::get_storage() const { return storage_; }

// Returns true when the declaration has constant storage.
inline bool value_decl::has_storage() const { return storage_ != constant_storage; }

// Returns true when the declaration has static storage.
inline bool value_decl::has_static_storage() const { return storage_ & static_storage; }

// Returns true when the declaration has automatic storage.
inline bool value_decl::has_automatic_storage() const { return storage_ & automatic_storage; }

// Returns true when the declaration has thread storage.
inline bool value_decl::has_thread_storage() const { return storage_ & thread_storage; }


// A helper class for declarations bound to values.
//
// Examples include variables and constants.
template<int K>
struct generic_value_decl : value_decl
{
  static constexpr int node_kind = K;

  generic_value_decl(name&, type&);
  generic_value_decl(name&, type&, expr&);
};

template<int K>
inline
generic_value_decl<K>::generic_value_decl(name& n, type& t)
  : value_decl(K, n, t)
{ }

template<int K>
inline
generic_value_decl<K>::generic_value_decl(name& n, type& t, expr& e)
  : value_decl(K, n, t, e)
{ }


// -------------------------------------------------------------------------- //
// Mapping declarations

/// Represents the mapping of inputs to outputs.
///
/// Examples of mappings are functions and axioms.
///
/// Mappings can be defined by either expressions (to support functional
/// programming) or statements (to support imperative programming).
struct mapping_decl : typed_decl
{
  enum {
    stmt_defn, // All mappings can be defined by a statements
    expr_defn  // All mappings can be defined by an expression
  };

  mapping_decl(int, name&, type&, const decl_seq&, decl&);
  mapping_decl(int, name&, type&, decl_seq&&, decl&);
  mapping_decl(int, name&, type&, const decl_seq&, decl&, expr&);
  mapping_decl(int, name&, type&, decl_seq&&, decl&, expr&);
  mapping_decl(int, name&, type&, const decl_seq&, decl&, stmt&);
  mapping_decl(int, name&, type&, decl_seq&&, decl&, stmt&);

  const decl_seq& get_parameters() const;
  decl_seq& get_parameters();
  
  const decl& get_return() const;
  decl& get_return();

  const type& get_return_type() const;
  type& get_return_type();

  bool has_definition() const;
  defn get_definition() const;

  decl_seq parms_;
  decl* ret_;
  defn def_;
};

inline
mapping_decl::mapping_decl(int k, name& n, type& t, const decl_seq& p, decl& r)
  : typed_decl(k, n, t), parms_(p), ret_(&r), def_()
{ }

inline
mapping_decl::mapping_decl(int k, name& n, type& t, decl_seq&& p, decl& r)
  : typed_decl(k, n, t), parms_(std::move(p)), ret_(&r), def_()
{ }

inline
mapping_decl::mapping_decl(int k, name& n, type& t, const decl_seq& p, decl& r, expr& e)
  : typed_decl(k, n, t), parms_(p), ret_(&r), def_(expr_defn, &e)
{ }

inline
mapping_decl::mapping_decl(int k, name& n, type& t, decl_seq&& p, decl& r, expr& e)
  : typed_decl(k, n, t), parms_(std::move(p)), ret_(&r), def_(expr_defn, &e)
{ }

inline
mapping_decl::mapping_decl(int k, name& n, type& t, const decl_seq& p, decl& r, stmt& e)
  : typed_decl(k, n, t), parms_(p), ret_(&r), def_(stmt_defn, &e)
{ }

inline
mapping_decl::mapping_decl(int k, name& n, type& t, decl_seq&& p, decl& r, stmt& e)
  : typed_decl(k, n, t), parms_(std::move(p)), ret_(&r), def_(stmt_defn, &e)
{ }

// Returns the mapping's parameters.
inline const decl_seq& mapping_decl::get_parameters() const { return parms_; }

// Returns the mapping's parameters.
inline decl_seq& mapping_decl::get_parameters() { return parms_; }

// Returns the mappings return declaration.
inline const decl& mapping_decl::get_return() const { return *ret_; }

// Returns the mappings return declaration.
inline decl& mapping_decl::get_return() { return *ret_; }

/// Returns true if the function has a definition.
inline bool mapping_decl::has_definition() const { return def_.is_present(); }

// Returns the definition of the mapping.
inline defn mapping_decl::get_definition() const { return def_; }


// A helper class for defining mapping declarations.
template<int K>
struct mapping_decl_impl : mapping_decl
{
  static constexpr int node_kind = K;

  mapping_decl_impl(name&, type&, const decl_seq&, decl&);
  mapping_decl_impl(name&, type&, decl_seq&&, decl&);
  mapping_decl_impl(name&, type&, const decl_seq&, decl&, expr&);
  mapping_decl_impl(name&, type&, decl_seq&&, decl&, expr&);
  mapping_decl_impl(name&, type&, const decl_seq&, decl&, stmt&);
  mapping_decl_impl(name&, type&, decl_seq&&, decl&, stmt&);
};


template<int K>
inline
mapping_decl_impl<K>::mapping_decl_impl(name& n, type& t, const decl_seq& p, decl& r)
  : mapping_decl(K, n, t, p, r)
{ }

template<int K>
inline
mapping_decl_impl<K>::mapping_decl_impl(name& n, type& t, decl_seq&& p, decl& r)
  : mapping_decl(K, n, t, std::move(p), r)
{ }

template<int K>
inline
mapping_decl_impl<K>::mapping_decl_impl(name& n, type& t, const decl_seq& p, decl& r, expr& e)
  : mapping_decl(K, n, t, p, r, e)
{ }

template<int K>
inline
mapping_decl_impl<K>::mapping_decl_impl(name& n, type& t, decl_seq&& p, decl& r, expr& e)
  : mapping_decl(K, n, t, std::move(p), r, e)
{ }

template<int K>
inline
mapping_decl_impl<K>::mapping_decl_impl(name& n, type& t, const decl_seq& p, decl& r, stmt& s)
  : mapping_decl(K, n, t, p, r, s)
{ }

template<int K>
inline
mapping_decl_impl<K>::mapping_decl_impl(name& n, type& t, decl_seq&& p, decl& r, stmt& s)
  : mapping_decl(K, n, t, std::move(p), r, s)
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

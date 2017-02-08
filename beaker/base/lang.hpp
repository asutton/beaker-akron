// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_LANGUAGE_HPP
#define BEAKER_BASE_LANGUAGE_HPP

#include <beaker/base/node.hpp>
#include <beaker/util/class_graph.hpp>
#include <beaker/util/dispatch_table.hpp>

#include <cassert>
#include <typeindex>
#include <unordered_map>
#include <vector>


namespace beaker {

struct symbol_table;
struct language;
struct module;
struct name;
struct type;
struct expr;
struct decl;
struct stmt;
struct value;
struct builder;


/// Represents an algorithm that operates on an AST. This is the abstract base 
/// class for concrete algorithms. Algorithms are collections of dispatch 
/// tables that associate specific behaviors with different kinds of nodes.
/// The structure of the algorithm depends on the nodes for which the algorithm
/// is defined. 
struct algorithm
{
  virtual ~algorithm() = default;
};

/// A container associating concrete algorithms with their type index.
struct algorithm_set
{
  using set_type = std::unordered_map<std::type_index, algorithm*>;

  algorithm_set() = default;
  ~algorithm_set();

  template<typename T>
  T& add_algorithm(language&);

  template<typename T>
  const T& get_algorithm() const;

  template<typename T>
  T& get_algorithm();

  set_type algos_;
};

/// Ensure that all algorithms are deleted.
inline
algorithm_set::~algorithm_set()
{
  for (auto p : algos_) 
    delete p.second; 
}

/// Adds a new algorithm of the given type.
template<typename T>
T&
algorithm_set::add_algorithm(language& lang)
{
  assert(algos_.count(typeid(T)) == 0);
  T* algo = new T(lang);
  algos_.emplace(typeid(T), algo);
  return *algo;
}

/// Returns the algorithm object for type `T`.
template<typename T>
inline const T&
algorithm_set::get_algorithm() const
{
  assert(algos_.count(typeid(T)) != 0);
  algorithm* algo = algos_.find(typeid(T))->second;
  return *static_cast<const T*>(algo);
}

/// Returns the algorithm object for type `T`.
template<typename T> 
inline T&
algorithm_set::get_algorithm()
{
  assert(algos_.count(typeid(T)) != 0);
  algorithm* algo = algos_.find(typeid(T))->second;
  return *static_cast<T*>(algo);
}

// -------------------------------------------------------------------------- //
// Features

/// A (language) feature defines a subset of terms within a language. This class
/// provides a simple protocol, used by a language object to define the terms
/// and their semantics. The addition of semantics usually corresponds to
/// the addition of overriders in dispatch sets.
struct feature
{
  virtual ~feature() = default;

  virtual void add_terms(language&) = 0;
  virtual void add_semantics(language&) = 0;
  virtual builder& make_builder(module&) const = 0;
};


/// Represents the feature set of  language.
struct feature_set
{
  using feature_list = std::vector<feature*>;
  using feature_map = std::unordered_map<std::type_index, feature*>;

  feature_set(const feature_list&);
  ~feature_set();

  const feature_list& get_features() const;
  feature_list& get_features();

  template<typename T> const feature& get_feature() const;
  template<typename T> feature& get_feature();

  feature_list list_;
  feature_map map_;
};

inline 
feature_set::feature_set(const feature_list& feats)
  : list_(feats)
{
  for (feature* f : list_)
    map_[typeid(*f)] = f;
}

inline
feature_set::~feature_set()
{
  for (feature * f : list_)
    delete f;
}

/// Returns the list of features for the language.
inline auto feature_set::get_features() const -> const feature_list& { return list_; }

/// Returns the list of features for the language.
inline auto feature_set::get_features() -> feature_list& { return list_; }

/// Returns the feature corresponding to the type T.
template<typename T>
inline const feature& feature_set::get_feature() const { return *map_.find(typeid(T))->second; }

/// Returns the feature corresponding to the type T.
template<typename T>
inline feature& feature_set::get_feature() {return *map_.find(typeid(T))->second; }


// -------------------------------------------------------------------------- //
// Language

/// Represents a set of language features.
///
/// This class is a singleton. Exactly one object must be created (typically
/// in main(), and that can be used throughout a program.
///
/// A client program must register the set of features needed by the language
/// at program startup.
///
/// \todo Should the language really own the symbol table, or is this something
/// that should be globally allocated (as a kind of string table) and passed
/// into the language?
///
/// \todo A f
struct language : algorithm_set, feature_set, node_store
{
  language(symbol_table&, const feature_list&);

  const symbol_table& get_symbol_table() const;
  symbol_table& get_symbol_table();

  const inheritance_hierarchy& get_names() const;
  inheritance_hierarchy& get_names();
  
  const inheritance_hierarchy& get_types() const;
  inheritance_hierarchy& get_types();
  
  const inheritance_hierarchy& get_expressions() const;
  inheritance_hierarchy& get_expressions();
  
  const inheritance_hierarchy& get_declarations() const;
  inheritance_hierarchy& get_declarations();
  
  const inheritance_hierarchy& get_statements() const;
  inheritance_hierarchy& get_statements();

  symbol_table* syms_;
  
  inheritance_hierarchy names_;
  inheritance_hierarchy types_;
  inheritance_hierarchy exprs_;
  inheritance_hierarchy decls_;
  inheritance_hierarchy stmts_;
};

/// Returns the symbols for the language.
inline const symbol_table& language::get_symbol_table() const { return *syms_; }

/// Returns the symbols for the language.
inline symbol_table& language::get_symbol_table() { return *syms_; }

/// Returns the set of name classes defined by the language.
inline const inheritance_hierarchy& language::get_names() const { return names_; }

/// Returns the set of name classes defined by the language.
inline inheritance_hierarchy& language::get_names() { return names_; }

/// Returns the set of name classes defined by the language.
inline const inheritance_hierarchy& language::get_types() const { return types_; }

/// Returns the set of name classes defined by the language.
inline inheritance_hierarchy& language::get_types() { return types_; }

/// Returns the set of name classes defined by the language.
inline const inheritance_hierarchy& language::get_expressions() const { return exprs_; }

/// Returns the set of name classes defined by the language.
inline inheritance_hierarchy& language::get_expressions() { return exprs_; }

/// Returns the set of name classes defined by the language.
inline const inheritance_hierarchy& language::get_declarations() const { return decls_; }

/// Returns the set of name classes defined by the language.
inline inheritance_hierarchy& language::get_declarations() { return decls_; }

/// Returns the set of name classes defined by the language.
inline const inheritance_hierarchy& language::get_statements() const { return stmts_; }

/// Returns the set of name classes defined by the language.
inline inheritance_hierarchy& language::get_statements() { return stmts_; }


// -------------------------------------------------------------------------- //
// Language and term identification

/// Assign a unique identifier to each language.
enum {
#define def_lang(l) l##_lang,
#include "lang.def"
  last_lang
};

/// Maximum number of terms per kind per language subset. Note that 100 is a 
/// very generous allocation.
constexpr int lang_block_size = 100;

/// Subsets of languages are uniquely identified by one of these values.
/// Nodes (of different kind) are grouped into blocks of 100 (for now).
enum {
#define def_lang(l) l##_lang_block = l##_lang * lang_block_size,
#include "lang.def"
};

/// Returns the language classifier for the kind.
constexpr int
get_language(int k) { return k / lang_block_size; }

} // namespace beaker


#endif

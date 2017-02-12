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
struct factory;


// -------------------------------------------------------------------------- //
// Features

/// A (language) feature defines a subset of terms within a language. This class
/// provides a simple protocol, used by a language object to define the terms
/// and their semantics. The addition of semantics usually corresponds to
/// the addition of overriders in dispatch sets.
struct feature
{
  virtual ~feature() = default;
  virtual factory& make_builder(module&) const = 0;
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
/// \todo The language should own a string table.
struct language : feature_set, node_store
{
  language(symbol_table&, const feature_list&);

  const symbol_table& get_symbol_table() const;
  symbol_table& get_symbol_table();

  symbol_table* syms_;
};

inline language::language(symbol_table& syms, const feature_list& feats)
  : feature_set(feats), syms_(&syms)
{ }

/// Returns the symbols for the language.
inline const symbol_table& language::get_symbol_table() const { return *syms_; }

/// Returns the symbols for the language.
inline symbol_table& language::get_symbol_table() { return *syms_; }


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

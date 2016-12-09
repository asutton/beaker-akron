// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_LANGUAGE_HPP
#define BEAKER_BASE_LANGUAGE_HPP

#include <beaker/base/generation/generation.hpp>

#include <iosfwd>
#include <string>
#include <unordered_map>


namespace beaker {

struct name;
struct type;
struct expr;
struct decl;
struct stmt;
struct hasher;
struct generator;

namespace cg {

struct type;
struct value;

} // namespace cg

/// A feature defines an extensible set of terms that can be used with the
/// beaker language runtime. In particular, this exposes a core set of 
/// algorithms and facilities needed to operate on those terms.
struct feature
{
  feature(int);
  virtual ~feature() = default;

  int get_id() const;

  virtual bool eq(const name&, const name&);
  virtual bool eq(const type&, const type&);
  virtual bool eq(const expr&, const expr&);

  virtual void hash(hasher&, const name&);
  virtual void hash(hasher&, const type&);
  virtual void hash(hasher&, const expr&);

  virtual void print(std::ostream&, const name&);
  virtual void print(std::ostream&, const type&);
  virtual void print(std::ostream&, const expr&);
  virtual void print(std::ostream&, const decl&);
  virtual void print(std::ostream&, const stmt&);

  virtual std::string gen(generator&, const name&);
  virtual cg::type gen(generator&, const type&);
  virtual cg::value gen(generator&, const expr&);
  virtual cg::value gen(generator&, const decl&);
  virtual void gen(generator&, const stmt&);

  int id_;
};

inline feature::feature(int id) : id_(id) { }

/// Returns the id 
inline int feature::get_id() const { return id_; }


/// A helper class that provides the feature id as an integer constant.
template<int N>
struct basic_feature : feature
{
  static constexpr int feature_id = N;

  basic_feature();
};

template<int N>
inline basic_feature<N>::basic_feature() : feature(N) { }


// -------------------------------------------------------------------------- //
// Language

/// Represents a set of language features.
///
/// This class is a singleton. Exactly one object must be created (typically
/// in main(), and that can be used throughout a program.
///
/// A client program must register the set of features needed by the language
/// at program startup.
struct language
{
  using feature_set = std::unordered_map<int, feature*>;

  language();
  ~language();

  static language& get_instance();
  static feature& get_feature(int);

  template<typename T>
  void add_feature();

  feature_set feat_;
};

/// Add the feature to the language. If the feature has already been added
/// then no action is taken.
template<typename T>
inline void
language::add_feature()
{
  feature *f = new T();
  feat_.emplace(f->get_id(), f);
}

// -------------------------------------------------------------------------- //
// Language and term identification

// Maximum number of terms per kind per language subset. Note that
// 100 is a very generous allocation.
constexpr int lang_block_size = 100;


// Assign a unique identifier to each language.
enum {
#define def_lang(l) l##_lang,
#include "lang.def"
  last_lang
};


// Subsets of languages are uniquely identified by one of these values.
// Nodes (of different kind) are grouped into blocks of 100 (for now).
enum {
#define def_lang(l) l##_lang_block = l##_lang * lang_block_size,
#include "lang.def"
};


/// Returns the language classifier for the kind.
constexpr int
get_language(int k) { return k / lang_block_size; }


} // namespace


#endif

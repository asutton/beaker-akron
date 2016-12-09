// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_LANGUAGE_HPP
#define BEAKER_BASE_LANGUAGE_HPP


namespace beaker {


// Assign a unique identifier to each language.
enum {
#define def_lang(l) l##_lang,
#include "lang.def"
  last_lang
};

// Maximum number of terms per kind per language subset. Note that
// 100 is a very generous allocation.
constexpr int lang_block_size = 100;

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

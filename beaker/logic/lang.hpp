// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_LOGIC_LANG_HPP
#define BEAKER_LOGIC_LANG_HPP

#include <beaker/base/lang.hpp>

#include <iosfwd>
#include <string>
#include <unordered_map>


namespace beaker {
namespace logic {

/// A feature defines an extensible set of terms that can be used with the
/// beaker language runtime. In particular, this exposes a core set of 
/// algorithms and facilities needed to operate on those terms.
struct feature : basic_feature<logic_lang>
{
  bool eq(const name&, const name&) override;
  bool eq(const type&, const type&) override;
  bool eq(const expr&, const expr&) override;

  void hash(hasher&, const name&) override;
  void hash(hasher&, const type&) override;
  void hash(hasher&, const expr&) override;

  void print(std::ostream&, const name&) override;
  void print(std::ostream&, const type&) override;
  void print(std::ostream&, const expr&) override;
  void print(std::ostream&, const decl&) override;
  void print(std::ostream&, const stmt&) override;

  std::string gen(generator&, const name&) override;
  cg::type gen(generator&, const type&) override;
  cg::value gen(generator&, const expr&) override;
  cg::value gen(generator&, const decl&) override;
  void gen(generator&, const stmt&) override;
};

} // namespace logic
} // namespace


#endif

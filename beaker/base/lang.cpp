// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include "name.hpp"
#include "type.hpp"
#include "expr.hpp"
#include "decl.hpp"
#include "stmt.hpp"

#include <cassert>


namespace beaker {

bool 
feature::eq(const name&, const name&)
{ 
  assert(false && "not implemented");
}

bool 
feature::eq(const type&, const type&)
{ 
  assert(false && "not implemented");
}

bool 
feature::eq(const expr&, const expr&)
{ 
  assert(false && "not implemented");
}


void
feature::hash(hasher&, const name&)
{ 
  assert(false && "not implemented");
}

void
feature::hash(hasher&, const type&)
{ 
  assert(false && "not implemented");
}

void
feature::hash(hasher&, const expr&)
{ 
  assert(false && "not implemented");
}


void
feature::print(std::ostream&, const name&)
{ 
  assert(false && "not implemented"); 
}

void
feature::print(std::ostream&, const type&)
{ 
  assert(false && "not implemented"); 
}

void
feature::print(std::ostream&, const expr&)
{ 
  assert(false && "not implemented"); 
}

void
feature::print(std::ostream&, const decl&)
{ 
  assert(false && "not implemented"); 
}

void
feature::print(std::ostream&, const stmt&)
{ 
  assert(false && "not implemented"); 
}


std::string
feature::gen(generator&, const name&)
{
  assert(false && "not implemented");
}

cg::type
feature::gen(generator&, const type&)
{
  assert(false && "not implemented");
}

cg::value
feature::gen(generator&, const expr&)
{
  assert(false && "not implemented");
}

cg::value
feature::gen(generator&, const decl&)
{
  assert(false && "not implemented");
}

void
feature::gen(generator&, const stmt&)
{
  assert(false && "not implemented");
}


// -------------------------------------------------------------------------- //

static language* lang_ = nullptr;

language::language()
{
  assert(!lang_);
  lang_ = this;
}

language::~language()
{
  for (auto p : feat_)
    delete p.second;
}

/// Returns the global language instance.
language& 
language::get_instance() 
{
  assert(lang_);
  return *lang_;
}

feature&
language::get_feature(int id)
{
  language& lang = get_instance();
  assert(lang.feat_.count(id) != 0);
  return *lang.feat_.find(id)->second;
}

/// Returns the feature that defines n.
feature&
language::get_feature(const name& n)
{
  return get_feature(n.get_feature());
}

/// Returns the feature that defines t.
feature&
language::get_feature(const type& t)
{
  return get_feature(t.get_feature());
}

/// Returns the feature that defines e.
feature&
language::get_feature(const expr& e)
{
  return get_feature(e.get_feature());
}

/// Returns the feature that defines d.
feature&
language::get_feature(const decl& d)
{
  return get_feature(d.get_feature());
}

/// Returns the feature that defines s.
feature&
language::get_feature(const stmt& s)
{
  return get_feature(s.get_feature());
}

} // namespace beaker

// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "json_reader.hpp"
#include <beaker/base/module.hpp>

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>

#include <iostream>
#include <fstream>
#include <stdexcept>


namespace beaker {

#define def_lang(l) \
namespace l { \
  extern void initialize_reader(json_reader&); \
}
#include "../lang.def"


json_reader::json_reader(module& m)
  : mod_(&m)
{ 
#define def_lang(l) l::initialize_reader(*this);
#include "../lang.def"
}

/// Load information from the module into the 
void
json_reader::load(const char* path)
{
  std::ifstream ifs(path);
  json::IStreamWrapper isw(ifs);
  json::Document doc;
  doc.ParseStream(isw);
  if (doc.HasParseError()) {
    // TODO: Handle this in a more sane way.
    std::cerr << "error: " << GetParseError_En(doc.GetParseError()) << '\n';
    throw std::runtime_error("parse error\n");
  }

  // Load all type values.
  //
  // TODO: Do this in two passes so we don't require json_readerrs to topologically 
  // sort type dependencies when building the json_reader. Note that this means we 
  // have to construct empty types and then populate them later -- that's 
  // going to be a big change to the AST.
  if (doc.HasMember("types")) {
    for (const json::Value& val : doc["types"].GetArray())
      get_type(val);
  }

  // Load all functions.
  //
  // TODO: What about other kinds of declarations? or just values?
  //
  // TODO: Should we also do this in two passes? It's less likely to be
  // necessary.
  if (doc.HasMember("functions")) {
    for (const json::Value& val : doc["functions"].GetArray()) {
      decl& d = get_declaration(val);
      mod_->add_declaration(d);
    }
  }
}

/// Returns the truth value of val.
bool
json_reader::get_bool(const json::Value& val)
{
  if (val.IsTrue())
    return true;
  else if (val.IsFalse())
    return false;
  else
    assert(false && "invalid value kind");
}

/// Returns the integer value of val.
int
json_reader::get_int(const json::Value& val)
{
  return val.GetInt();
}

/// Returns a name corresponding to a JSON value.
name&
json_reader::get_name(const json::Value& val)
{
  if (val.IsObject()) {
    // Parse and store the declaration.
    json_reader::name_parser parse = get_name_parser(val["kind"].GetString());
    return parse(*this, val);  
  }
  else {
    name_parser parse = get_name_kind(val.GetType());
    return parse(*this, val);
  }
}

// Register a type corresponding to a JSON value.
//
// The JSON value must be an integer.
void
json_reader::register_type(const json::Value& val, type& t)
{
  register_type(val.GetInt(), t);
}

// Returns a type corresponding to a JSON value.
//
// The value can be a string, int, or object. If the value is a string, the
// type is an elementary type whose name is that of the string. If the value
// is an id, then the type is that corresponding to the id. It shall have been
// previously parsed and registered. Otherwise, if the the value is an object
// the definition of the type is within the object; it shall contain "kind"
// and "id" attributes which can be used to parse and register the type.
type&
json_reader::get_type(const json::Value& val)
{
  if (val.IsString())
    // Returns the elementary type.
    return get_type(val.GetString());
  else if (val.IsInt())
    // Returns an existing compound type.
    return get_type(val.GetInt());
  else if (val.IsObject()) {
    // The type is defined in place. It must have a kind and id field.
    //
    // Note that a topologically flattened module will not arbitrarily
    // define types within types.
    type_parser parse = get_type_parser(val["kind"].GetString());
    type& t = parse(*this, val);
    register_type(val["id"].GetInt(), t);
    return t;
  }
  else
    assert(false && "invalid type");
}

// Returns a sequence of types.
type_seq
json_reader::get_type_seq(const json::Value& val)
{
  type_seq types;
  for (const json::Value& type : val.GetArray())
    types.push_back(get_type(type));
  return types;
}

// Returns an expression for the given value.
//
// The value is either an object or null. If the expression is an object,
// this returns the expression determined by the "kind" attribute. Otherwise,
// the null value corresponds to the nop expression.
expr&
json_reader::get_expression(const json::Value& val)
{
  if (val.IsObject()) {
    // Parse and store the declaration.
    json_reader::expr_parser parse = get_expr_parser(val["kind"].GetString());
    return parse(*this, val);  
  }
  else {
    expr_parser parse = get_expr_kind(val.GetType());
    return parse(*this, val);
  }
}

// Returns the declaration stored in the JSON value.
//
// The value can be an integer or an object. If the value is an integer, then
// it is a reference to a previously parsed and registered declaration. 
// Otherwise, value shall be an object containing "kind" and "id" fields. 
//
// TODO: We could also accept strings as references to builtin declarations.
decl&
json_reader::get_declaration(const json::Value& val)
{
  if (val.IsInt())
    // The declaration has been previously parsed and stored.
    return get_declaration(val.GetInt());
  else if (val.IsObject()) {
    // Parse and store the declaration.
    decl_parser parse = get_decl_parser(val["kind"].GetString());
    decl& d = parse(*this, val);
    register_declaration(val["id"].GetInt(), d);
    return d;
  }
  assert(false && "invalid declaration");
}

// Get a sequence of declarations.
decl_seq
json_reader::get_declaration_seq(const json::Value& val)
{
  decl_seq decls;
  for (const json::Value& decl : val.GetArray())
    decls.push_back(get_declaration(decl));
  return decls;
}


} // namespace beaker


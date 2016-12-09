// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_SERIALIZATION_JSON_READER_HPP
#define BEAKER_BASE_SERIALIZATION_JSON_READER_HPP

#include <beaker/base/serialization/json.hpp>
#include <beaker/base/type.hpp>
#include <beaker/base/expr.hpp>
#include <beaker/base/decl.hpp>
#include <beaker/base/stmt.hpp>

#include <cassert>
#include <array>
#include <string>
#include <unordered_map>
#include <vector>

#include <rapidjson/document.h>


namespace beaker
{

struct allocator;
struct symbol_table;
struct module;
struct name;
struct type;
struct expr;
struct decl;

namespace json = rapidjson;

// Used to load fragments of a program.
struct json_reader
{
  using type_name_map = std::unordered_map<std::string, type*>;
  using type_id_map = std::unordered_map<int, type*>;
  using decl_id_map = std::unordered_map<int, decl*>;

  using name_parser = name&(*)(json_reader&, const json::Value&);
  using name_parser_set = std::array<name_parser, json::last_value>;
  using name_parser_map = std::unordered_map<std::string, name_parser>;

  using type_parser = type&(*)(json_reader&, const json::Value&);
  using type_parser_map = std::unordered_map<std::string, type_parser>;

  using expr_parser = expr&(*)(json_reader&, const json::Value&);
  using expr_parser_set = std::array<expr_parser, json::last_value>;
  using expr_parser_map = std::unordered_map<std::string, expr_parser>;

  using decl_parser = decl&(*)(json_reader&, const json::Value&);
  using decl_parser_map = std::unordered_map<std::string, decl_parser>;

  json_reader(module&);

  module& get_module();

  // Miscellaneous values
  bool get_bool(const json::Value&);
  int get_int(const json::Value&);

  // Names
  name& get_name(const json::Value&);

  void register_name_parser(const char*, name_parser);
  name_parser get_name_parser(const char*);

  void register_name_kind(int, name_parser);
  name_parser get_name_kind(int);
  
  // Types
  void register_type(const char*, type&);
  void register_type(int, type&);
  void register_type(const json::Value&, type&);
  type& get_type(const char*);
  type& get_type(int);
  type& get_type(const json::Value&);
  type_seq get_type_seq(const json::Value&);

  void register_type_parser(const char*, type_parser);
  type_parser get_type_parser(const char*);

  // Expressions
  expr& get_expression(const json::Value&);

  void register_expr_parser(const char*, expr_parser);
  expr_parser get_expr_parser(const char*);
  
  void register_expr_kind(int, expr_parser);
  expr_parser get_expr_kind(int);

  // Declarations
  void register_declaration(int, decl&);
  decl& get_declaration(int);
  decl& get_declaration(const json::Value&);
  decl_seq get_declaration_seq(const json::Value&);
  
  void register_decl_parser(const char*, decl_parser);
  decl_parser get_decl_parser(const char*);

  void load(const char*);

  module* mod_;

  type_name_map type_names_;     // Mapping of name to type
  type_id_map type_ids_;         // Mapping of id to type
  decl_id_map decl_ids_;         // Mapping of id to declaration
  
  name_parser_set val_to_name_; // Associates a parser with value kinds
  name_parser_map str_to_name_; // Mapping of names to name parsers
  type_parser_map str_to_type_; // Mapping of names to type parsers
  expr_parser_map str_to_expr_; // Mapping of names to expression parsers
  expr_parser_set val_to_expr_; // Associates a parser with value kinds
  decl_parser_map str_to_decl_; // Mapping of names to declaration parsers
};

/// Returns the module being constructed.
///
/// This is defined only when the module is being loaded.
///
/// TODO: Make this private and make all reader serializers friends.
inline module& json_reader::get_module() { return *mod_; }

// Register p as a parser for names of kind str.
inline void 
json_reader::register_name_parser(const char* str, name_parser p)
{
  assert(str_to_name_.count(str) == 0);
  str_to_name_.emplace(str, p);
}

// Returns the parser for names of kind str.
inline auto
json_reader::get_name_parser(const char* str) -> name_parser
{
  auto iter = str_to_name_.find(str);
  assert(iter != str_to_name_.end());
  return iter->second;
}

// Registers p as a parser for names of value kind k.
inline void
json_reader::register_name_kind(int k, name_parser p)
{
  assert(val_to_name_[k] == nullptr);
  val_to_name_[k] = p;
}

// Returns the name parser for value kinds k.
inline auto
json_reader::get_name_kind(int k) -> name_parser
{
  assert(val_to_name_[k] != nullptr);
  return val_to_name_[k];
}

// Associate the elementary type t with the then name in str.
inline void 
json_reader::register_type(const char* str, type& t)
{
  assert(type_names_.count(str) == 0);
  type_names_.emplace(str, &t);
}

// Associate composite type t with the id.
inline void
json_reader::register_type(int id, type& t)
{
  assert(type_ids_.count(id) == 0);
  type_ids_.emplace(id, &t);
}

// Returns the type with the name in str.
inline type&
json_reader::get_type(const char* str)
{
  auto iter = type_names_.find(str);
  assert(iter != type_names_.end());
  return *iter->second;
}

// Returns the type corresponding to id.
inline type&
json_reader::get_type(int id)
{
  auto iter = type_ids_.find(id);
  assert(iter != type_ids_.end());
  return *iter->second;
}

// Register p as a parser for compound types whose kind is str.
inline void 
json_reader::register_type_parser(const char* str, type_parser p)
{
  assert(str_to_type_.count(str) == 0);
  str_to_type_.emplace(str, p);
}

// Returns the parser for compound types of kind str.
inline auto
json_reader::get_type_parser(const char* str) -> type_parser
{
  auto iter = str_to_type_.find(str);
  assert(iter != str_to_type_.end());
  return iter->second;
}

// Register p as a parser for expressions of kind str.
inline void 
json_reader::register_expr_parser(const char* str, expr_parser p)
{
  assert(str_to_expr_.count(str) == 0);
  str_to_expr_.emplace(str, p);
}

// Returns the parser for expressions of kind str.
inline auto
json_reader::get_expr_parser(const char* str) -> expr_parser
{
  auto iter = str_to_expr_.find(str);
  assert(iter != str_to_expr_.end());
  return iter->second;
}

// Registers p as a parser for expressions of value kind k.
inline void
json_reader::register_expr_kind(int k, expr_parser p)
{
  assert(val_to_expr_[k] == nullptr);
  val_to_expr_[k] = p;
}

// Returns the expression parser for k.
inline auto
json_reader::get_expr_kind(int k) -> expr_parser
{
  assert(val_to_expr_[k] != nullptr);
  return val_to_expr_[k];
}

// Associate declaration d with id.
inline void
json_reader::register_declaration(int id, decl& d)
{
  assert(decl_ids_.count(id) == 0);
  decl_ids_.emplace(id, &d);
}

// Returns the declaration corresponding to id.
inline decl&
json_reader::get_declaration(int id)
{
  auto iter = decl_ids_.find(id);
  assert(iter != decl_ids_.end());
  return *iter->second;
}

// Register p as a parser for declarations of kind str.
inline void 
json_reader::register_decl_parser(const char* str, decl_parser p)
{
  assert(str_to_decl_.count(str) == 0);
  str_to_decl_.emplace(str, p);
}

// Returns the parser for declarations of kind str.
inline auto
json_reader::get_decl_parser(const char* str) -> decl_parser
{
  auto iter = str_to_decl_.find(str);
  assert(iter != str_to_decl_.end());
  return iter->second;
}

} // namespace beaker


#endif

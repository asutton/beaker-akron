// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/core/name.hpp>
#include <beaker/core/type.hpp>
#include <beaker/core/expr.hpp>
#include <beaker/core/decl.hpp>
#include <beaker/core/construction/builder.hpp>
#include <beaker/base/module.hpp>
#include <beaker/base/serialization/json_reader.hpp>

namespace beaker {
namespace core {

static builder&
get_builder(json_reader& r)
{
  return r.get_module().get_builder<builder>(core_lang);
}

// Returns a name from the given value.
//
// If the string is empty, then name will be an internal name.
name&
parse_name(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  const char* str = val.GetString();
  if (*str)
    return b.get_name(str);
  else
    return b.get_name();
}

// Parse reference types.
//
// Reference types have the following properties:
//
//    - object-type: a type field
static type&
parse_ref_type(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  return b.get_ref_type(
    r.get_type(val["object-type"])
  );
}

// Parse function types.
//
// Function types have the following properties:
//
//    - return-type: a type field
//    - parameter-types: an array of type fields
static type&
parse_fn_type(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  return b.get_fn_type(
    r.get_type_seq(val["parameter-types"]),
    r.get_type(val["return-type"]),
    r.get_int(val["specifiers"])
  );
}

static expr&
parse_nop_expr(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  return b.make_nop_expr(
    r.get_type(val["type"])
  );
}

static expr&
parse_void_expr(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  return b.make_void_expr(
    r.get_type(val["type"]),
    r.get_expression(val["operand"])
  );
}


/// FIXME: Read the reference. It must be an id referring to another function
/// or global variable.
static expr&
parse_ref_expr(json_reader& r, const json::Value& val)
{
  assert(false && "not implemented");
}

static expr&
parse_copy_expr(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  const json::Value& ops = val["operands"];
  return b.make_copy_expr(
    r.get_type(val["type"]),
    r.get_expression(ops[0]),
    r.get_expression(ops[1])
  );
}

static expr&
parse_call_expr(json_reader& r, const json::Value& val)
{
  assert(false && "not implemented");
}

// Parse a function declaration.
//
// Function declarations have the following fields:
//
//    - name: an id field
//    - parameters: a list of variable declarations
//    - return: a return variable declaration
//    - definition: an expression
//
// The function type is determined by the parameters and return.
static decl&
parse_fn_decl(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  return b.make_fn_decl(
    r.get_name(val["name"]),
    r.get_type(val["type"]),
    r.get_declaration_seq(val["parameters"]),
    r.get_declaration(val["return"]),
    r.get_expression(val["definition"])
  );
}

// Parse a parameter declaration.
//
// Parameters declarations have the following fields:
//
//    - "name": name
//    - "type": type
//
// Note that the parameters are essentially variables with no initializer.
static decl&
parse_parm_decl(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  return b.make_var_decl(
    r.get_name(val["name"]),
    r.get_type(val["type"])
  );
}

// Parse a variable declaration.
//
// Variable declarations have the following fields:
//
//    - "name": name
//    - "type": type
//    - "initializer": expression
//
// A null initializer means that no expression is given. This will be
// replaced by a nop expression.
static decl&
parse_var_decl(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  return b.make_var_decl(
    r.get_name(val["name"]),
    r.get_type(val["type"]),
    r.get_expression(val["initializer"])
  );
}

// Initialize an json_reader to handle common types.
void
initialize_reader(json_reader& r)
{
  builder& b = get_builder(r);

  r.register_name_kind(json::string_value, parse_name);
  
  r.register_type("void", b.get_void_type());
  r.register_type_parser("ref", parse_ref_type);
  r.register_type_parser("fn", parse_fn_type);

  r.register_expr_parser("nop", parse_nop_expr);
  r.register_expr_parser("void", parse_void_expr);
  r.register_expr_parser("ref", parse_ref_expr);
  r.register_expr_parser("init", parse_copy_expr);
  r.register_expr_parser("call", parse_call_expr);
  
  r.register_decl_parser("fn", parse_fn_decl);
  r.register_decl_parser("parm", parse_parm_decl);
  r.register_decl_parser("var", parse_var_decl);

  // FIXME: Add parsers for statements.
}

} // namespace core
} // namespace beaker

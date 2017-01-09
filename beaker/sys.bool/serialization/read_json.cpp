// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/logic/type.hpp>
#include <beaker/logic/expr.hpp>
#include <beaker/logic/construction/builder.hpp>
#include <beaker/base/module.hpp>
#include <beaker/base/serialization/json_reader.hpp>


namespace beaker {
namespace logic {

static inline builder& 
get_builder(json_reader& r) 
{
  return r.get_module().get_builder<builder>(logic_lang);
}

// Returns the literal `true`.
static expr&
parse_true_as_expr(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  return b.get_true_expr();
}

// Returns the literal `false`.
static expr&
parse_false_as_expr(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  return b.get_false_expr();
}

// Parse a boolean expression.
//
// This has the following attributes:
//
//    - "value" : true or false
static expr&
parse_bool_expr(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  const json::Value& lit = val["value"];
  if (lit.IsTrue())
    return b.get_true_expr();
  else if (lit.IsFalse())
    return b.get_false_expr();
  else
    assert(false && "invalid boolean expression");
}

// Parse a logical and expression.
static expr&
parse_and_expr(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  const json::Value& ops = val["operands"];
  return b.make_and_expr(
    r.get_type(val["type"]),
    r.get_expression(ops[0]),
    r.get_expression(ops[1])
  );
}

// Parse a logical or expression.
static expr&
parse_or_expr(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  const json::Value& ops = val["operands"];
  return b.make_or_expr(
    r.get_type(val["type"]),
    r.get_expression(ops[0]),
    r.get_expression(ops[1])
  );
}

// Parse a logical not expression.
static expr&
parse_not_expr(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  const json::Value& ops = val["operands"];
  return b.make_not_expr(
    r.get_type(val["type"]),
    r.get_expression(ops[0])
  );
}

void
initialize_reader(json_reader& r)
{
  builder& b = get_builder(r);
  r.register_type("bool", b.get_bool_type());
  r.register_expr_kind(json::true_value, parse_true_as_expr);
  r.register_expr_kind(json::false_value, parse_false_as_expr);
  r.register_expr_parser("bool", parse_bool_expr);
  r.register_expr_parser("and", parse_and_expr);
  r.register_expr_parser("or", parse_or_expr);
  r.register_expr_parser("not", parse_not_expr);
}

} // namespace logic
} // namespace beaker


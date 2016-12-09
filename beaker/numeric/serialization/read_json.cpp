// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/numeric/type.hpp>
#include <beaker/numeric/expr.hpp>
#include <beaker/numeric/construction/builder.hpp>
#include <beaker/base/module.hpp>
#include <beaker/base/serialization/json_reader.hpp>


namespace beaker {
namespace numeric {

static builder&
get_builder(json_reader& r)
{
  return r.get_module().get_builder<builder>(numeric_lang);
}

// Parse an integer literal.
static expr&
parse_int_expr(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  const json::Value& lit = val["value"];
  if (lit.IsInt())
    return b.make_int_expr(r.get_type(val["type"]), lit.GetInt());
  else
    assert(false && "invalid boolean expression");
}

// Parse an addition expression.
static expr&
parse_add_expr(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  const json::Value& ops = val["operands"];
  return b.make_add_expr(
    r.get_type(val["type"]),
    r.get_expression(ops[0]),
    r.get_expression(ops[1])
  );
}

// Parse a subtraction expression.
static expr&
parse_sub_expr(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  const json::Value& ops = val["operands"];
  return b.make_sub_expr(
    r.get_type(val["type"]),
    r.get_expression(ops[0]),
    r.get_expression(ops[1])
  );
}

// Parse an multiplication expression.
static expr&
parse_mul_expr(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  const json::Value& ops = val["operands"];
  return b.make_mul_expr(
    r.get_type(val["type"]),
    r.get_expression(ops[0]),
    r.get_expression(ops[1])
  );
}

// Parse a subtraction expression.
static expr&
parse_div_expr(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  const json::Value& ops = val["operands"];
  return b.make_div_expr(
    r.get_type(val["type"]),
    r.get_expression(ops[0]),
    r.get_expression(ops[1])
  );
}

// Parse a remainder expression.
static expr&
parse_rem_expr(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  const json::Value& ops = val["operands"];
  return b.make_rem_expr(
    r.get_type(val["type"]),
    r.get_expression(ops[0]),
    r.get_expression(ops[1])
  );
}

// Initialize numeric components for the json_reader.
void
initialize_reader(json_reader& r)
{
  builder& b = get_builder(r);
  
  r.register_type("nat8", b.get_nat8_type());
  r.register_type("nat16", b.get_nat16_type());
  r.register_type("nat32", b.get_nat32_type());
  r.register_type("nat64", b.get_nat64_type());
  r.register_type("int8", b.get_int8_type());
  r.register_type("int16", b.get_int16_type());
  r.register_type("int32", b.get_int32_type());
  r.register_type("int64", b.get_int64_type());
  r.register_type("mod8", b.get_mod8_type());
  r.register_type("mod16", b.get_mod16_type());
  r.register_type("mod32", b.get_mod32_type());
  r.register_type("mod64", b.get_mod64_type());
  r.register_type("float32", b.get_float32_type());
  r.register_type("float64", b.get_float64_type());

  r.register_expr_parser("int", parse_int_expr);
  r.register_expr_parser("add", parse_add_expr);
  r.register_expr_parser("sub", parse_sub_expr);
  r.register_expr_parser("mul", parse_mul_expr);
  r.register_expr_parser("div", parse_div_expr);
  r.register_expr_parser("rem", parse_rem_expr);
}

} // namespace numeric
} // namespace beaker


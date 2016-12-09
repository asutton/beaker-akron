// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/variadic/type.hpp>
#include <beaker/variadic/expr.hpp>
#include <beaker/variadic/construction/builder.hpp>
#include <beaker/base/module.hpp>
#include <beaker/base/serialization/json_reader.hpp>

namespace beaker {
namespace variadic {

// A useful function.
static builder&
get_builder(json_reader& r)
{
  return r.get_module().get_builder<builder>(variadic_lang);
}

/// Parse the expression `va.start(e)`.
static expr&
parse_start_expr(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  const json::Value& ops = val["operands"];
  return b.make_start_expr(
    r.get_type(val["type"]),
    r.get_expression(ops[0])
  );
}

/// Parse the expression `va.end(e)`.
static expr&
parse_end_expr(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  const json::Value& ops = val["operands"];
  return b.make_end_expr(
    r.get_type(val["type"]),
    r.get_expression(ops[0])
  );
}

/// Parse the expression `va.arg(t, e)`.
///
/// The type is parsed as that of the expression.
static expr&
parse_arg_expr(json_reader& r, const json::Value& val)
{
  builder& b = get_builder(r);
  const json::Value& ops = val["operands"];
  return b.make_arg_expr(
    r.get_type(val["type"]),
    r.get_expression(ops[0])
  );
}

/// Parse the expression `va.copy(e1, e2)`.
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

void
initialize_reader(json_reader& r)
{
  builder& b = get_builder(r);
  r.register_type("va.list", b.get_list_type());
  r.register_expr_parser("va.start", parse_start_expr);
  r.register_expr_parser("va.end", parse_end_expr);
  r.register_expr_parser("va.arg", parse_arg_expr);
  r.register_expr_parser("va.copy", parse_copy_expr);
}

} // namespace variadic
} // namespace beaker

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "write.hpp"
#include "../type.hpp"
#include "../expr.hpp"
#include "../decl.hpp"
#include "../stmt.hpp"


namespace beaker {
namespace sys_fn {

// Append the hash of t's parameter and return types to h.
static void
write_fn_type(archive_writer& ar, const fn_type& t)
{
  write_seq(ar, t.get_parameter_types());
  write_type(ar, t.get_return_type());
}

void
write_algo::operator()(archive_writer& ar, const type& t) const
{
  assert(is<fn_type>(t));
  write_fn_type(ar, cast<fn_type>(t));
}


void
write_call_expr(archive_writer& ar, const call_expr& e)
{
  write_expr(ar, e.get_function());
  write_seq(ar, e.get_arguments());
}

void
write_algo::operator()(archive_writer& ar, const expr& e) const
{
  switch (e.get_kind()) {
    case call_expr_kind:
      return write_call_expr(ar, cast<call_expr>(e));
    case eq_expr_kind:
    case ne_expr_kind:
      return write_binary_expr(ar, static_cast<const binary_expr&>(e));
    default:
      break;
  }
  assert(false && "not a function expression");
}


void
write_fn_decl(archive_writer& ar, const fn_decl& d)
{
  write_name(ar, d.get_name());
  write_type(ar, d.get_type());
  write_seq(ar, d.get_parameters());
  write_decl(ar, d.get_return());

  defn def = d.get_definition();
  write_bool(ar, def.is_present());
  if (def.is_present()) {
    write_int(ar, def.get_kind());
    if (def.get_kind() == fn_decl::stmt_defn) {
      const stmt& s = def.get_as<stmt>();
      write_stmt(ar, s);
    }
    else {
      const expr& e = def.get_as<expr>();
      write_expr(ar, e);
    }
  }
}

void
write_parm_decl(archive_writer& ar, const parm_decl& d)
{
  write_name(ar, d.get_name());
  write_type(ar, d.get_type());
}

void
write_algo::operator()(archive_writer& ar, const decl& d) const
{
  switch (d.get_kind()) {
    case fn_decl_kind:
      return write_fn_decl(ar, cast<fn_decl>(d));
    case parm_decl_kind:
      return write_parm_decl(ar, cast<parm_decl>(d));
    default:
      assert(false && "not a function declaration");
  }
}


static void
write_block_stmt(archive_writer& ar, const block_stmt& s)
{
  write_seq(ar, s.get_statements());
}

static void
write_expr_stmt(archive_writer& ar, const expr_stmt& s)
{
  write_expr(ar, s.get_expression());
}

/// A declaration statement is actually a reference to a declaration. This
/// is encoded this way so that we can recover declaration order as needed.
static void
write_decl_stmt(archive_writer& ar, const decl_stmt& s)
{
  write_ref(ar, s.get_declaration());
}


static void
write_ret_stmt(archive_writer& ar, const ret_stmt& s)
{
  write_expr(ar, s.get_return());
}

void
write_algo::operator()(archive_writer& ar, const stmt& s) const
{
  switch (s.get_kind()) {
    case block_stmt_kind:
      return write_block_stmt(ar, cast<block_stmt>(s));
    case expr_stmt_kind:
      return write_expr_stmt(ar, cast<expr_stmt>(s));
    case decl_stmt_kind:
      return write_decl_stmt(ar, cast<decl_stmt>(s));
    case ret_stmt_kind:
      return write_ret_stmt(ar, cast<ret_stmt>(s));
    default:
      assert(false && "not a function statement");
  }
}

} // namespace sys_fn
} // namespace beaker

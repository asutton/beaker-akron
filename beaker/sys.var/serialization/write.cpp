// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "write.hpp"
#include "../type.hpp"
#include "../expr.hpp"
#include "../decl.hpp"


namespace beaker {
namespace sys_var {

static void
write_ref_type(archive_writer& ar, const ref_type& t)
{
  write_type(ar, t.get_object_type());
}

void
write_algo::operator()(archive_writer& ar, const type& t) const
{
  assert(is<ref_type>(t));
  write_ref_type(ar, cast<ref_type>(t));
}


void
write_ref_expr(archive_writer& ar, const ref_expr& e)
{
  write_ref(ar, e.get_declaration());
}

void
write_deref_expr(archive_writer& ar, const deref_expr& e)
{
  write_expr(ar, e.get_source());
}

void
write_copy_init(archive_writer& ar, const copy_init& e)
{
  write_expr(ar, e.get_expression());
}

void
write_ref_init(archive_writer& ar, const ref_init& e)
{
  write_expr(ar, e.get_expression());
}

void
write_algo::operator()(archive_writer& ar, const expr& e) const
{
  switch (e.get_kind()) {
    case ref_expr_kind:
      return write_ref_expr(ar, cast<ref_expr>(e));
    case deref_expr_kind:
      return write_deref_expr(ar, cast<deref_expr>(e));
    case assign_expr_kind:
      return write_binary_expr(ar, static_cast<const assign_expr&>(e));
    case nop_init_kind:
    case zero_init_kind:
      // The object type is already encoded by the expression.
      return;
    case copy_init_kind:
      return write_copy_init(ar, cast<copy_init>(e));
    case ref_init_kind:
      return write_ref_init(ar, cast<ref_init>(e));
    default:
      break;
  }
  assert(false && "not a variable expression");
}


void
write_algo::operator()(archive_writer& ar, const decl& d) const
{
  assert(is<var_decl>(d));
  write_value_decl(ar, cast<var_decl>(d));
}

} // namespace sys_var
} // namespace beaker

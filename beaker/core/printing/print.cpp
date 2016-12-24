// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/core/printing/print.hpp>
#include <beaker/core/name.hpp>
#include <beaker/core/type.hpp>
#include <beaker/core/expr.hpp>
#include <beaker/core/decl.hpp>
#include <beaker/core/stmt.hpp>
#include <beaker/util/symbol.hpp>

#include <iostream>


namespace beaker {
namespace core {

void
print_algo::operator()(std::ostream& os, const name& n) const
{
  switch (n.get_kind()) {
    case basic_name_kind:
      os << cast<basic_name>(n).get_symbol().get_spelling();
      return;
    case internal_name_kind:
      os << "<internal>";
      return;
    default:
      break;
  }
  assert(false && "not a core name");
}


// Pretty print the void type.
static void
print_void_type(std::ostream& os, const void_type& t)
{
  os << "void";
}

// Pretty print a reference type.
template<typename T>
static void
print_reference_type(std::ostream& os, const T& t, const char* str)
{
  os << str << ' ';
  print(os, t.get_object_type());
}

// Pretty print a function type.
static void
print_fn_type(std::ostream& os, const fn_type& t)
{
  os << '(';
  const type_seq& parms = t.get_parameter_types();
  for (auto iter = parms.begin(); iter != parms.end(); ++iter) {
    print(os, *iter);
    if (std::next(iter) != parms.end())
      os << ',';
  }
  os << ')';
  os << "->";
  print(os, t.get_return_type());
}

// Pretty print a common type.
void
print_algo::operator()(std::ostream& os, const type& t) const
{
  switch (t.get_kind()) {
    case void_type_kind:
      return print_void_type(os, cast<void_type>(t));
    case ref_type_kind:
      return print_reference_type(os, cast<ref_type>(t), "ref");
    case in_type_kind:
      return print_reference_type(os, cast<in_type>(t), "in");
    case out_type_kind:
      return print_reference_type(os, cast<out_type>(t), "out");
    case fn_type_kind:
      return print_fn_type(os, cast<fn_type>(t));
    default:
      break;
  }
  assert(false && "not a core type");
}


static void
print_call_expr(std::ostream& os, const call_expr& e)
{
  print(os, e.get_function());
  os << '(';
  const expr_seq& args = e.get_arguments();
  for (auto iter = args.begin(); iter != args.end(); ++iter) {
    print(os, *iter);
    if (std::next(iter) != args.end())
      os << ',';
  }
  os << ')';
}

static void
print_zero_init(std::ostream& os, const zero_init& e)
{
  os << "= zero";
}

static void
print_copy_init(std::ostream& os, const copy_init& e)
{
  os << "= " << "copy ";
  print(os, e.get_operand());
}

// FIXME: This is incomplete.
void
print_algo::operator()(std::ostream& os, const expr& e) const
{
  switch (e.get_kind()) {
    case nop_expr_kind:
      os << "nop";
      return;
    case ref_expr_kind:
      return print(os, cast<ref_expr>(e).get_name());
    case call_expr_kind:
      return print_call_expr(os, cast<call_expr>(e));

    case zero_init_kind:
      return print_zero_init(os, cast<zero_init>(e));
    case copy_init_kind:
      return print_copy_init(os, cast<copy_init>(e));
    default:
      break;
  }
  assert(false && "not a core expression");
}


// Pretty print a value declaration.
template<typename T>
static void
print_value_decl(std::ostream& os, const T& d, const char* str)
{
  os << str << ' ';
  print(os, d.get_type());
  os << ' ';
  print(os, d.get_name());
  if (d.has_initializer())
    print(os, d.get_initializer().template get_as<expr>());
}

// Pretty print a function declaration.
static void
print_fn_decl(std::ostream& os, const fn_decl& d)
{
  os << "fn" << ' ';
  print(os, d.get_name());
  os << '(';
  decl_seq const& parms = d.get_parameters();
  for (auto iter = parms.begin(); iter != parms.end(); ++iter) {
    print(os, *iter);
    if (std::next(iter) != parms.end())
      os << ',';
  }
  os << ')';
  os <<  " -> ";
  print(os, d.get_return());

  if (d.has_definition()) {
    defn def = d.get_definition();
    if (def.get_kind() == fn_decl::expr_defn) {
      os << ' ';
      print(os, def.get_as<expr>());
      os << ';';
    }
    else
      print(os, def.get_as<stmt>());
  }
  os << '\n';
}

void
print_parm_decl(std::ostream& os, const parm_decl& d)
{
  print(os, d.get_type());
  os << ' ';
  print(os, d.get_name());
}

// FIXME: Allow language packs to add declarations.
void
print_algo::operator()(std::ostream& os, const decl& d) const
{
  switch (d.get_kind()) {
    case var_decl_kind:
      return print_value_decl(os, cast<var_decl>(d), "var");
    case fn_decl_kind:
      return print_fn_decl(os, cast<fn_decl>(d));
    case parm_decl_kind:
      return print_parm_decl(os, cast<parm_decl>(d));
    default:
      break;
  }
  assert(false && "not a core declaration");
}


static void
print_block_stmt(std::ostream& os, const block_stmt& s)
{
  os << "{\n";
  for (const stmt& s1 : s.get_statements())
    print(os, s1);
  os << "}\n";
}

static void
print_decl_stmt(std::ostream& os, const decl_stmt& s)
{
  print(os, s.get_declaration());
  os << '\n';
}

static void
print_expr_stmt(std::ostream& os, const expr_stmt& s)
{
  print(os, s.get_expression());
  os << ";\n";
}

static void
print_ret_stmt(std::ostream& os, const ret_stmt& s)
{
  os << "return ";
  print(os, s.get_return());
  os << ";\n";
}

void
print_algo::operator()(std::ostream& os, const stmt& s) const
{
  switch (s.get_kind()) {
    case block_stmt_kind:
      return print_block_stmt(os, cast<block_stmt>(s));
    case decl_stmt_kind:
      return print_decl_stmt(os, cast<decl_stmt>(s));
    case expr_stmt_kind:
      return print_expr_stmt(os, cast<expr_stmt>(s));
    case ret_stmt_kind:
      return print_ret_stmt(os, cast<ret_stmt>(s));
    default:
      break;
  }
  assert(false && "not a core statement");
}

} // namespace core
} // namespace beaker

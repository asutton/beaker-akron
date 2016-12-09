


void
print(std::ostream& os, const name& n)
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
  assert(false && "not a common name");
}

// Pretty print the void type.
static void
print_type(std::ostream& os, const void_type& t)
{
  os << "void";
}

// Pretty print a reference type.
static void
print_type(std::ostream& os, const ref_type& t)
{
  os << "ref" << ' ';
  print(os, t.get_referent_type());
}

// Pretty print a function type.
static void
print_type(std::ostream& os, const fn_type& t)
{
  os << '(';
  const type_seq& parms = t.get_parameter_types();
  for (auto iter = parms.begin(); iter != parms.end(); ++iter) {
    print(os, *iter);
    if (std::next(iter) == parms.end())
      os << ',';
  }
  os << ')';
  os << "->";
  print(os, t.get_return_type());
}

// Pretty print a common type.
static void
print_type(std::ostream& os, const type& t)
{
  switch (t.get_kind()) {
    case void_type_kind:
      return print_type(os, cast<void_type>(t));
    case ref_type_kind:
      return print_type(os, cast<ref_type>(t));
    case fn_type_kind:
      return print_type(os, cast<fn_type>(t));
    default:
      break;
  }
  assert(false && "not a common type");
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

void
print_expr(std::ostream& os, const expr& e) 
{
  switch (e.get_kind()) {
    case nop_expr_kind:
      os << "nop";
      return;
    case ref_expr_kind:
      return print(os, cast<ref_expr>(e).get_name());
    case obj_expr_kind:
      return print(os, cast<obj_expr>(e).get_source());
    case call_expr_kind:
      return print_call_expr(os, cast<call_expr>(e));
    default:
      break;
  }
  assert(false && "not a common expression");
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
  // FIXME: Finish this. 
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
  os <<  "-> ";
  print(os, d.get_return());
  os << " = ";
  print(os, d.get_definition());
}

// FIXME: Allow language packs to add declarations.
void
print(std::ostream& os, const decl& d)
{
  switch (d.get_kind()) {
    case var_decl_kind:
      return print_value_decl(os, cast<var_decl>(d), "var");
    case ref_decl_kind:
      return print_value_decl(os, cast<ref_decl>(d), "ref");
    case reg_decl_kind:
      return print_value_decl(os, cast<reg_decl>(d), "reg");
    case const_decl_kind:
      return print_value_decl(os, cast<const_decl>(d), "const");
    case fn_decl_kind:
      return print_fn_decl(os, cast<fn_decl>(d));
    default:
      break;
  }
  assert(false && "declaration not supported");
}


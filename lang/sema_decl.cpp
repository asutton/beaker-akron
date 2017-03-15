// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

namespace bpl {

/// Insert d as a declaration in m. Returns the declaration d.
static decl&
declare_in(module& m, decl& d)
{
  m.decls_.push_back(d);
  return d;
}

decl&
semantics::declare(scope& s, decl& d)
{
  named_decl& nd = *d.as_named();
  
  // Detemrine if the declaration is valid.
  if (auto* ent = env.lookup(nd.get_name())) {
    if (ent->s == &s) {
      // FIXME: This could be a redeclaration.
      //
      // FIXME: Implement overloading.
      //
      // FIXME: Add the declaration's source code location and generate good 
      // error messages.
      throw decl_error(location(), "name already declared in this scope");
    }
  }

  // Add d to the ennvironment.
  env.add(d);

  // Add the declaration to the current context, if needed.
  decl& cxt = current_context();
  switch (cxt.get_kind()) {
    case beaker::module_decl_kind:
      return declare_in(cast<module>(cxt), d);
    default:
      break;
  }
  assert(false && "invalid context for declaration");
}

decl&
semantics::declare(decl& d)
{
  named_decl& nd = *d.as_named();
  
  // Detemrine if the declaration is valid.
  if (auto* ent = env.lookup(nd.get_name())) {
    scope& s = current_scope();
    if (ent->s == &s) {
      // FIXME: This could be a redeclaration.
      //
      // FIXME: Implement overloading.
      //
      // FIXME: Add the declaration's source code location and generate good 
      // error messages.
      throw decl_error(location(), "name already declared in this scope");
    }
  }

  // Add d to the ennvironment.
  env.add(d);

  // Add the declaration to the current context, if needed.
  decl& cxt = current_context();
  switch (cxt.get_kind()) {
    case beaker::module_decl_kind:
      return declare_in(cast<module>(cxt), d);
    default:
      break;
  }
  assert(false && "invalid context for declaration");
}


/// Simply return the module.
decl&
semantics::on_start_module()
{
  return mod;
}

/// Simply return the module.
///
/// \todo Apply all deferred actions here.
decl&
semantics::on_finish_module()
{
  return mod;
}

/// Construct and declare a function.
decl&
semantics::on_start_function(name& id, decl_seq&& parms, type& rty, locations<4> locs)
{
  decl& ret = build_fn.make_parm_decl(rty);
  type& fty = build_fn.get_fn_type(parms, ret);
  decl& cxt = current_context();
  decl& fn = build_fn.make_fn_decl(cxt, id, fty, std::move(parms), ret);

  // Declare the function. Note that we've entered function parameter scope
  // so we declare the function just outsie of that.
  scope& s = *current_scope().get_parent();
  declare(s, fn);

  return fn;
}

/// Returns a function declaration.
decl& 
semantics::on_finish_function(decl& d, location semi)
{
  return current_context();
}

/// Attach the body to the current function definition.
///
/// \todo Build a set of lowering functions to iteratively lower the definition
/// until we can emit IR code. Note that we can also release all nodes
/// allocated by those passes once we've finished the definition. This will
/// help keep memory low.
decl& 
semantics::on_finish_function(decl& d, stmt& body)
{
  assert(&current_function() == &d);
  sys_fn::fn_decl& fn = current_function();
  fn.def_ = &body;

  // TODO: Lower or analyze the function?

  return fn;
}

/// Construct and declare the function parameter.
decl&
semantics::on_function_parameter(type& t, name& n)
{
  if (sys_void::is_void_type(t))
    throw type_error(location(), "void parameter");

  decl& parm = build_fn.make_parm_decl(n, t);
  declare(parm);

  return parm;
}


/// \todo The kindd of variable being declared on the scope in which it
/// was written and its declaration specifiers.
decl&
semantics::on_start_variable(type& t, name& n, location start)
{
  if (sys_void::is_void_type(t))
    throw type_error(location(), "void variable");

  decl& cxt = current_context();
  decl& var = build_fn.make_var_decl(cxt, n, t);

  return var;
}

/// \todo Actually implement initialization semantics. This needs to
/// determine an initialization procedure for the given declaration based
/// on the expression (and type) given.
void
semantics::initialize(decl& d, expr& e)
{
  value_decl& vd = dynamic_cast<value_decl&>(d);
  if (!equal(vd.get_type(), e.get_type()))
    throw type_error(location(), "initialization with wrong type");
  vd.init_ = &e;
}


decl&
semantics::on_finish_variable(decl& d, expr& e, locations<2>)
{
  sys_fn::var_decl& var = cast<sys_fn::var_decl>(d);
  initialize(d, e);
  return var;
}

} // namespace bpl

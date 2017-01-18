// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "decl.hpp"
#include "module.hpp"


namespace beaker
{

module&
decl::get_module()
{
  decl* p = this;
  while (p->cxt_)
    p = p->cxt_;
  return cast<module>(*p);
}

const module& 
decl::get_module() const
{
  return const_cast<decl*>(this)->get_module();
}

// Returns this as a named declaration.
const named_decl*
decl::as_named() const 
{ 
  return dynamic_cast<const named_decl*>(this);
}

// Returns this as a named declaration.
named_decl*
decl::as_named() 
{ 
  return dynamic_cast<named_decl*>(this);
}

// Returns this as a typed declaration.
const typed_decl*
decl::as_typed() const
{
  return dynamic_cast<const typed_decl*>(this);
}

// Returns this as a typed declaration.
typed_decl*
decl::as_typed() 
{ 
  return dynamic_cast<typed_decl*>(this);
}


// Returns the return type of a mapping declaration.
const type& 
mapping_decl::get_return_type() const
{ 
  return get_return().as_typed()->get_type(); 
}

// Returns the return type of a mapping declaration. 
type& mapping_decl::get_return_type()
{
  return get_return().as_typed()->get_type();
}

} // namespace beaker

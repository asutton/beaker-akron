// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_GENERATION_FUNCTION_HPP
#define BEAKER_BASE_GENERATION_FUNCTION_HPP

#include <beaker/base/generation/note.hpp>

#include <cassert>
#include <vector>


namespace beaker {
namespace cg {

enum {
  direct_parm,   // The parameter is passed directly
  indirect_parm, // The parameter is passed indirectly
};

/// Stores information for needed for parameter passing. Parameters have
/// a primary kind, which determines whether they are passed directly (in
/// registers) or indirectly (as an address). 
///
/// That a parameter is passed indirectly is an artifact of translation. The
/// indirect parameter could represent the return value, or it could represent
/// a parameter that was intended to be passed by value.
///
/// Note that direct parameters can be neither return values nor passed by
/// value.
struct parm_info
{
  parm_info(int k, bool, bool);

  int get_kind() const;
  bool is_direct() const;
  bool is_indirect() const;

  bool is_ret_value() const;
  bool is_by_value() const;

  int kind_;
  bool retval_ : 1; // True if a return value
  bool byval_ : 1; // True if passed by value
};

inline 
parm_info::parm_info(int k, bool r, bool b) 
  : kind_(k), retval_(r), byval_(b) 
{
  assert(!(retval_ && byval_));
  assert(is_direct() ? !(retval_ || byval_) : true);
}

/// Returns the kind of parameter.
inline int parm_info::get_kind() const { return kind_; }

/// Returns true if the parameter is passed directly.
inline bool parm_info::is_direct() const { return kind_ == direct_parm; }

/// Returns true if the parameter is passed indirectly.
inline bool parm_info::is_indirect() const { return kind_ == indirect_parm; }

/// Returns true if the return value is passed indirectly.
inline bool parm_info::is_ret_value() const { return retval_; }

/// Returns true if the parameter is passed by value.
inline bool parm_info::is_by_value() const { return byval_; }


/// A sequence of parameters.
using parm_seq = std::vector<parm_info>;


/// Represents additional information about functions for code generation.
///
/// \todo Associate more information about functions?
struct fn_info : note
{
  fn_info(const parm_seq&);
  fn_info(parm_seq&&);

  const parm_seq& get_parameters() const;
  parm_seq& get_parameters();

  bool has_return_parameter() const;

  parm_seq parms_;
};

inline fn_info::fn_info(const parm_seq& p) : parms_(p) { }

inline fn_info::fn_info(parm_seq&& p) : parms_(std::move(p)) { }

/// Returns parameter information.
inline const parm_seq& fn_info::get_parameters() const { return parms_; }

/// Returns parameter information.
inline parm_seq& fn_info::get_parameters() { return parms_; }

/// Returns true if the function's return value is passed as a parameter.
///
/// Note that the return value must be the first parameter.
///
/// \todo Not all ABIs place the return value in the first parameter.
inline bool 
fn_info::has_return_parameter() const 
{ 
  return !parms_.empty() && parms_.front().is_ret_value();
}

} // namespace cg
} // namespace beaker


#endif

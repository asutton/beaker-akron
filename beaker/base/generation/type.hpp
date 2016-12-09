// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_GENERATION_TYPE_HPP
#define BEAKER_BASE_GENERATION_TYPE_HPP

#include <beaker/base/generation/note.hpp>


namespace llvm {

class Type;

} // namespace llvm


namespace beaker {
namespace cg {

/// A wrapper around llvm::Types that provides additional information.
struct type : annotated
{
  type(llvm::Type*);
  type(llvm::Type*, bool);

  operator llvm::Type*() const;

  llvm::Type* get_type();
  llvm::Type* operator->() const;

  bool is_direct() const;
  bool is_indirect() const;

  llvm::Type* type_;
  bool direct_;
};

/// Initialize this as a direct type.
inline type::type(llvm::Type* t) : type_(t), direct_(true) { }

/// Initialize the type.
inline type::type(llvm::Type* t, bool d) : type_(t), direct_(d) { }

/// Converts to an LLVM type.
inline type::operator llvm::Type*() const { return type_; }

/// Returns a pointer to the underlying LLVM type.
inline llvm::Type* type::operator->() const { return type_; }

/// Returns the underlying LLVM statically cast as T.
inline llvm::Type* type::get_type() { return type_; }

/// Returns true if the type is passed directly.
inline bool type::is_direct() const { return direct_; }

/// Returns true if the type is passed indirectly (i.e., by reference).
inline bool type::is_indirect() const { return !direct_; }

} // namespace cg
} // namespace beaker


#endif

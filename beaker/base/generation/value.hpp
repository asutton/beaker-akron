// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_GENERATION_VALUE_HPP
#define BEAKER_BASE_GENERATION_VALUE_HPP

#include <beaker/base/generation/note.hpp>


namespace llvm {

class Value;

} // namespace llvm


namespace beaker {
namespace cg {

/// A wrapper around llvm::Value that provides additional information.
struct value : annotated
{
  value();
  value(llvm::Value*);
  
  explicit operator bool() const;
  operator llvm::Value*() const;
  llvm::Value* operator->() const;

  llvm::Value* val_;
};

/// Initialize an empty value.
inline value::value() : val_(nullptr) { }

/// Initialize this as a direct type.
inline value::value(llvm::Value* v) : val_(v) { }

/// Converts to an true when the value is non-null.
inline value::operator bool() const { return val_; }

/// Converts to an LLVM type.
inline value::operator llvm::Value*() const { return val_; }

/// Returns a pointer to the underlying LLVM value.
inline llvm::Value* value::operator->() const { return val_; }

} // namespace cg
} // namespace beaker


#endif

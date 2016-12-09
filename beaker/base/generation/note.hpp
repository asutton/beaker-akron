// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_GENERATION_NOTE_HPP
#define BEAKER_BASE_GENERATION_NOTE_HPP

#include <cassert>


namespace beaker {
namespace cg {

/// A note is an additional piece of information associated with a type or
/// value. Language features provide additional annotations during the
/// construction of types and values.
///
/// The code generator is responsible for allocating and deallocating these
/// objects.
struct note
{
  virtual ~note() { }
};


/// A base class for annotated codegen elements.
///
/// \todo Allow saving multiple notes of different kinds in e.g., a vector.
struct annotated
{
  annotated();

  template<typename T> const T* get_note() const;
  template<typename T> T* get_note();
  
  void annotate(note&);
  
  note* note_;
};

inline annotated::annotated() : note_() { }

/// Returns the optional note.
template<typename T>
inline const T* annotated::get_note() const 
{
  return dynamic_cast<const T*>(note_);
}

/// Returns the optional note.
template<typename T>
inline T* annotated::get_note() 
{
  return dynamic_cast<T*>(note_);
}

/// Save a note with the object.
inline void 
annotated::annotate(note& n)
{
  assert(note_ == nullptr);
  note_ = &n; 
}

} // namespace cg
} // namespace beaker


#endif

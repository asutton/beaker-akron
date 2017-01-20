// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_SERIALIZATION_WRITER_HPP
#define BEAKER_BASE_SERIALIZATION_WRITER_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/name.hpp>
#include <beaker/base/type.hpp>
#include <beaker/base/expr.hpp>
#include <beaker/base/decl.hpp>
#include <beaker/base/stmt.hpp>
#include <beaker/base/comparison/eq.hpp>
#include <beaker/base/comparison/hash.hpp>

#include <cstdint>
#include <string>


namespace beaker {

struct symbol;

/// \todo Rename this to archive writer.
struct archive_writer
{
  using byte_stream = std::vector<std::uint8_t>;
  using stream_table = std::vector<byte_stream>;

  // A reference to a stream.
  struct stream_ref 
  {
    stream_table* table = nullptr;
    int index = -1;

    const byte_stream& get() const { return (*table)[index]; }
    byte_stream& get() { return (*table)[index]; }
  };

  template<typename T>
  struct encoding
  {
    using map = std::unordered_map<T, std::uint32_t>;
    using table = stream_table;
    map ids;
    table bytes;
  };

  using type_map = std::unordered_map<const type*, std::uint32_t>;
  using decl_map = std::unordered_map<const decl*, std::uint32_t>;

  void write_id(std::uint32_t);
  void write_bool(bool);
  void write_int(std::int32_t);
  void write_int(std::uint32_t);
  void write_string(const char*);
  void write_string(const symbol&);

  void write_module(const module&);
  void write_name(const name&);
  void write_type(const type&);
  void write_expr(const expr&);
  void write_decl(const decl&);
  void write_ref(const decl&);
  void write_stmt(const stmt&);

  void save(const char*);

  void save_type(const type&);
  void save_decl(const decl&);

  byte_stream& get_active_stream();

  encoding<const type*> types;
  encoding<const decl*> decls;
  encoding<std::string> strings;
  stream_ref active; // The active stream

  struct activate_stream;
};

/// Returns the active stream. Behavior is undefined if there is no active
/// stream.
inline archive_writer::byte_stream& 
archive_writer::get_active_stream() { return active.get(); }


/// An RAII helper to activate a byte stream and restore the previous during
/// evaluation.
struct archive_writer::activate_stream
{
  /// Activates the last stream in the given table.
  activate_stream(archive_writer& ar, stream_table& t)
    : ar(ar), prev(ar.active)
  {
    ar.active.table = &t;
    ar.active.index = t.size() - 1;
  }

  ~activate_stream()
  {
    ar.active = prev;
  }

  archive_writer& ar;
  stream_ref prev;
};



/// Defines the dispatch signature for write serialization.
struct write_algorithm : algorithm
{
  struct tag { };
  virtual void operator()(archive_writer&, const name&) const;
  virtual void operator()(archive_writer&, const type&) const;
  virtual void operator()(archive_writer&, const expr&) const;
  virtual void operator()(archive_writer&, const decl&) const;
  virtual void operator()(archive_writer&, const stmt&) const;
};

inline void
write_bool(archive_writer& ar, bool b)
{ 
  ar.write_bool(b); 
}

inline void
write_int(archive_writer& ar, int n)
{ 
  ar.write_int(n); 
}

inline void
write_string(archive_writer& ar, const char* s)
{ 
  ar.write_string(s); 
}

inline void
write_string(archive_writer& ar, const symbol& s)
{ 
  ar.write_string(s); 
}

template<typename T>
inline void
write_seq(archive_writer& ar, const seq<T>& seq)
{
  write_int(ar, seq.size());
  for (const T& t : seq)
    write_term(ar, t);
}

inline void 
write_module(archive_writer& ar, const module& m)
{
  ar.write_module(m);
}

inline void
write_name(archive_writer& ar, const name& n)
{
  ar.write_name(n);
}

inline void
write_type(archive_writer& ar, const type& t)
{
  ar.write_type(t);
}

inline void
write_expr(archive_writer& ar, const expr& e)
{
  ar.write_expr(e);
}

/// Write the declaration to the archive.
inline void
write_decl(archive_writer& ar, const decl& d)
{
  ar.write_decl(d);
}

/// Write a reference to a declaration to the archive.
inline void
write_ref(archive_writer& ar, const decl& d)
{
  ar.write_ref(d);
}

/// Write a type to the archive.
inline void
write_stmt(archive_writer& ar, const stmt& s)
{
  ar.write_stmt(s);
}

inline void
write_term(archive_writer& ar, const name& n)
{
  write_name(ar, n);
}

inline void
write_term(archive_writer& ar, const type& t)
{
  write_type(ar, t);
}

inline void
write_term(archive_writer& ar, const expr& e)
{
  write_expr(ar, e);
}

inline void
write_term(archive_writer& ar, const decl& d)
{
  write_decl(ar, d);
}

inline void
write_term(archive_writer& ar, const stmt& s)
{
  write_stmt(ar, s);
}

/// Write the operand of a unary expression to the archive.
inline void
write_unary_expr(archive_writer& ar, const unary_expr& e)
{
  write_expr(ar, e.get_operand());
}

/// Write the operands of a binary expression to the archive.
inline void
write_binary_expr(archive_writer& ar, const binary_expr& e)
{
  write_expr(ar, e.get_lhs());
  write_expr(ar, e.get_rhs());
}

/// Write the operands of a ternary expression to the archive.
inline void
write_binary_expr(archive_writer& ar, const ternary_expr& e)
{
  write_expr(ar, e.get_first());
  write_expr(ar, e.get_second());
  write_expr(ar, e.get_third());
}

/// Writes a declaration's name and linkage to the current stream.
inline void
write_named_decl(archive_writer& ar, const named_decl& d)
{
  write_name(ar, d.get_name());
  write_int(ar, d.get_linkage());
}

/// Writes a type declaration to the current stream.
inline void
write_typed_decl(archive_writer& ar, const typed_decl& d)
{
  write_named_decl(ar, d);
  write_type(ar, d.get_type());
}

/// Writes a value declaration to the current stream.
inline void
write_value_decl(archive_writer& ar, const value_decl& d)
{
  write_typed_decl(ar, d);
  write_int(ar, d.get_storage());
  if (d.has_initializer()) {
    write_bool(ar, true);
    write_expr(ar, d.get_initializer());
  }
  else {
    write_bool(ar, false);
  }
}

// Write a mapping declaration to the current stream.
void
write_mapping_decl(archive_writer& ar, const mapping_decl& d)
{
  write_typed_decl(ar, d);
  write_seq(ar, d.get_parameters());
  write_decl(ar, d.get_return());
  if (d.has_definition()) {
    write_bool(ar, true);
    write_stmt(ar, d.get_definition());
  }
  else {
    write_bool(ar, false);
  }
}


} // namespace beaker


#endif

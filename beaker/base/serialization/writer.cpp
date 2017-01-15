// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "writer.hpp"

#include <beaker/base/module.hpp>
#include <beaker/base/printing/print.hpp>
#include <beaker/util/symbol.hpp>

#include <arpa/inet.h>

#include <iostream>
#include <iomanip>


namespace beaker {


// -------------------------------------------------------------------------- //
// Dispatch infrastructure

/// The default behavior is not defined.
void
write_algorithm::operator()(archive_writer&, const name&) const
{
  assert(false && "not defined");
}

/// The default behavior is not defined.
void
write_algorithm::operator()(archive_writer&, const type&) const
{
  assert(false && "not defined");
}

/// The default behavior is not defined.
void
write_algorithm::operator()(archive_writer&, const expr&) const
{
  assert(false && "not defined");
}

/// The default behavior is not defined.
void
write_algorithm::operator()(archive_writer&, const decl&) const
{
  assert(false && "not defined");
}

/// The default behavior is not defined.
void
write_algorithm::operator()(archive_writer&, const stmt&) const
{
  assert(false && "not defined");
}

// -------------------------------------------------------------------------- //
// Archive

// Returns the serialization algorithm associated with the node t.
template<typename T>
static inline const write_algorithm&
get_write(const T& t)
{
  feature& feat = language::get_feature(t);
  return feat.template get_algorithm<write_algorithm>();
}

/// Append a boolean value to the archive.
void
archive_writer::write_id(std::size_t id)
{
  id = htonll(id);
  const unsigned char* p = reinterpret_cast<const unsigned char*>(&id);
  byte_stream& stream = get_active_stream();
  stream.insert(stream.end(), p, p + sizeof(id));
}

/// Append a boolean value to the archive.
void
archive_writer::write_bool(bool b)
{
  get_active_stream().push_back(b);
}

/// Append an integer value to the archive.
void
archive_writer::write_int(int n)
{
  n = htonl(n);
  const unsigned char* p = reinterpret_cast<const unsigned char*>(&n);
  byte_stream& stream = get_active_stream();
  stream.insert(stream.end(), p, p + sizeof(int));
}

/// Writes the string into the archive.
///
/// \todo Write the string into a dedicated string table.
void
archive_writer::write_string(const char* s)
{
  std::size_t n = std::strlen(s);
  write_int(n);
  byte_stream& stream = get_active_stream();
  stream.insert(stream.end(), s, s + n);
}

/// Write the symbol into the current stream.
void
archive_writer::write_string(const symbol& s)
{
  write_string(s.get_spelling());
}

/// Write the contents of a module to the archive.
///
/// \todo Write language features and options into a metadata block.
void
archive_writer::write_module(const module& m)
{
  for (const decl& d : m.get_declarations())
    write_decl(d);
}

/// Write the name into the current stream.
void
archive_writer::write_name(const name& n)
{
  write_int(n.get_kind());
  get_write(n)(*this, n);
}

/// Actually save the type. This encodes the type's kind and dispatches
/// to the type for encoding.
void
archive_writer::save_type(const type& t)
{
  write_int(t.get_kind());
  get_write(t)(*this, t);
}

/// Write a reference to a type. Note that types are not top-level entities,
/// so the are always references.
///
/// \todo: If the type is non-canonical, then it can be written inline.
void
archive_writer::write_type(const type& t)
{
  assert(type_ids.size() == types.size());
  auto iter = type_ids.find(&t);
  if (iter == type_ids.end()) {
    // If the type hasn't been seen yet generate a new id and initialize
    // it's byte stream.
    std::size_t id = type_ids.size();
    type_ids.emplace(&t, id);
    types.emplace_back();
    {
      // Step outside of the current stream and generate the type.
      activate_stream s(*this, types);
      save_type(t);
    }
    // Write the id into the current stream.
    write_id(id);
  }
  else {
    // Just write the type id into the stream.
    write_id(iter->second);
  }
}

/// Write the expression into the stream. This writes the kind and type
/// prior to the details of the expression.
void
archive_writer::write_expr(const expr& e)
{
  write_int(e.get_kind());
  write_type(e.get_type());
  get_write(e)(*this, e);
}

/// Actually save the declaration into the current stream. Serialize the
/// node kind before its contents.
void
archive_writer::save_decl(const decl& d)
{
  write_int(d.get_kind());
  get_write(d)(*this, d);
}

/// Writes a declaration into the current stream. This must only be used
/// when writing declaration from declaration contexts, not types or
/// expressions. Use write_ref() in those cases.
///
/// In the case of forward references, we don't actually need to do anything.
void
archive_writer::write_decl(const decl& d)
{
  assert(decl_ids.size() == decls.size());
  std::size_t id = decl_ids.size();
  auto result = decl_ids.emplace(&d, id);
  if (result.second) {
    // We have not previously seen this declaration, so we can generate
    // a new byte stream for it and write it in place.
    decls.emplace_back();
    {
      activate_stream s(*this, decls);
      save_decl(d);
    }
  }
}

/// Write a reference to a declaration. If the declaration has not yet been
/// serialized, do so now.
void
archive_writer::write_ref(const decl& d)
{
  assert(decl_ids.size() == decls.size());
  auto iter = decl_ids.find(&d);
  if (iter == decl_ids.end()) {
    // If the declaration hasn't been seen yet generate a new id and initialize
    // it's byte stream.
    std::size_t id = decl_ids.size();
    decl_ids.emplace(&d, id);
    decls.emplace_back();
    {
      // Step outside of the current stream and generate the type.
      activate_stream s(*this, decls);
      save_decl(d);
    }
    // Write the id into the current stream.
    write_id(id);
  }
  else {
    // Just write the type id into the stream.
    write_id(iter->second);
  }
}

/// Write the statement into the stream.
void
archive_writer::write_stmt(const stmt& s)
{
  write_int(s.get_kind());
  get_write(s)(*this, s);
}

static void
print_bytes(const archive_writer::byte_stream& b)
{
  std::cout << std::hex;
  std::cout << std::setfill('0');
  unsigned int n = 0;
  for (auto c : b) {
    std::cout << std::setw(2) << (unsigned)c << ' ';
    if (++n == 16) {
      std::cout << '\n';
      n = 0;
    }
  }
  std::cout << '\n';
  std::cout << std::dec;
  std::cout << std::setfill(' ');
}

static const type&
get_type(const archive_writer& ar, int id) 
{
  for (auto p : ar.type_ids) {
    if (p.second == id)
      return *p.first;
  }
  assert(false && "unknown type id");
}

static const decl&
get_decl(const archive_writer& ar, int id) 
{
  for (auto p : ar.decl_ids) {
    if (p.second == id)
      return *p.first;
  }
  assert(false && "unknown type id");
}

/// Save the populated archive to a file.
void
archive_writer::save(const char*)
{
  std::cout << "--- types ---\n";
  for (std::size_t i = 0; i != types.size(); ++i) {
    const type& t = get_type(*this, i);
    std::cout << i << ": " << t.get_kind() << ": ";
    print(t);
    print_bytes(types[i]);
  }

  std::cout << "\n--- declarations ---\n";
  for (std::size_t i = 0; i != decls.size(); ++i) {
    const decl& d = get_decl(*this, i);
    std::cout << i << ": " << d.get_kind() << ": ";
    print(d);
    print_bytes(decls[i]);
  }
}

} // namespace beaker
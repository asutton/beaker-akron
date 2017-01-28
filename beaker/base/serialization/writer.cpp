// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "writer.hpp"

#include <beaker/base/module.hpp>
#include <beaker/base/printing/print.hpp>
#include <beaker/util/symbol.hpp>

// FIXME: Can we rely on an endian.h header instead of inet/byteswap?
//
// FIXME: Factor this into a sepaate module so we can use it for reading
// also.
#if defined(__linux__)
#  include <endian.h>
#  include <arpa/inet.h>
#elif defined(__APPLE__)
#  include <arpa/inet.h>
#else
#  error unknown operating system
#endif

#include <iostream>
#include <iomanip>


namespace beaker {

template<typename T, int N>
struct is_integral_size 
  : std::integral_constant<bool, std::is_integral<T>::value && sizeof(T) == N>
{ };

template<typename T>
inline std::enable_if_t<is_integral_size<T, 2>::value, T>
msbf(T n)
{
#if defined(__linux__)
  return (T)htons(n);
#elif defined(__APPLE__)
  return (T)htons(n);
#endif
}

template<typename T>
inline std::enable_if_t<is_integral_size<T, 4>::value, T>
msbf(T n)
{
#if defined(__linux__)
  return (T)htonl(n);
#elif defined(__APPLE__)
  return (T)htonl(n);
#endif
}

template<typename T>
inline std::enable_if_t<is_integral_size<T, 8>::value, T>
msbf(T n)
{
#if defined(__linux__)
  return htobe64(n);
#elif defined(__APPLE__)
  return htonll(n);
#endif
}


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
///
/// \todo This current writes in network byte order for debugging purposes.
/// It's not strictly necessary since modules are not intended to be shared
/// across architectures (but presumably, they could).
void
archive_writer::write_id(std::uint32_t id)
{
  id = msbf(id);
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

/// Append a 32-bit integer value to the archive.
void
archive_writer::write_int(std::int32_t n)
{
  return write_int(std::uint32_t(n));
}

/// Append a 32-bit integer value to the archive.
void
archive_writer::write_int(std::uint32_t n)
{
  n = msbf(n);
  const unsigned char* p = reinterpret_cast<const unsigned char*>(&n);
  byte_stream& stream = get_active_stream();
  stream.insert(stream.end(), p, p + sizeof(int));
}

/// Writes the string into the archive.
///
/// This does not require that the string is previously uniqued. Strings of
/// equal value will be unified in the string table.
void
archive_writer::write_string(const char* s)
{
  assert(strings.ids.size() == strings.ids.size());
  std::string str = s;
  auto iter = strings.ids.find(str);
  if (iter == strings.ids.end()) {
    std::size_t id = strings.ids.size();
    strings.ids.emplace(str, id);
    strings.bytes.emplace_back();
    {
      // Save the string into the string table.
      activate_stream s(*this, strings.bytes);
      byte_stream& bs = get_active_stream();
      std::uint32_t len = msbf(std::uint32_t(str.size()));
      const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&len);
      bs.insert(bs.end(), ptr, ptr + sizeof(len));
      bs.insert(bs.end(), str.begin(), str.end());
    }
    // Write the id into the current stream.
    write_id(id);
  }
  else {
    // Write the id of the string into the active stream.
    write_id(iter->second);
  }
}

/// Write the symbol into the current stream.
void
archive_writer::write_string(const symbol& s)
{
  write_string(s.get_spelling());
}


// Encode the module m and return its id.
std::uint32_t
archive_writer::save_module(const module& m)
{
  std::size_t id = mods.bytes.size();
  mods.bytes.emplace_back();
  activate_stream s(*this, mods.bytes);
  write_name(m.get_module_name());
  return id;
}

// Get the encoded id for the module m. This will encode the module if it
// has not already been encoded.
std::uint32_t
archive_writer::get_module(const module& m)
{
  assert(mods.ids.size() == mods.bytes.size());
  std::size_t id = mods.bytes.size();
  auto result = mods.ids.emplace(&m, id);
  if (result.second)
    return save_module(m);
  else
    return result.first->second;
}

/// Write the contents of a module to the archive.
///
/// \todo Write language features and options into a metadata block.
void
archive_writer::write_module(const module& m)
{
  assert(!mod_);
  mod_ = &m;

  // Ensure that this module name is the first in the module table. 
  save_module(m);

  // Encode the module. Note that all modules have id 0, so this will
  // always be the first declaration in the declaration table.
  decls.ids.emplace(&m, m.get_id());
  decls.bytes.emplace_back();
  activate_stream s(*this, decls.bytes);
  for (const decl& d : m.get_declarations())
    write_decl(d);

  mod_ = nullptr;
}

/// Write a reference to a module. Modules can be referenced by certain kinds
/// of declarations (e.g., imports) and references to imported declarations.
void
archive_writer::write_ref(const module& m)
{
  write_id(get_module(m));
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
std::uint32_t
archive_writer::save_type(const type& t)
{
  std::size_t id = types.bytes.size();
  types.bytes.emplace_back();
  activate_stream s(*this, types.bytes);
  write_int(t.get_kind());
  get_write(t)(*this, t);
  return id;
}

/// Returns the encoded id for a type. If the type has not been encoded, then
/// that is done before returning.
std::uint32_t
archive_writer::get_type(const type& t)
{
  assert(types.ids.size() == types.bytes.size());
  std::size_t id = types.bytes.size();
  auto result = types.ids.emplace(&t, id);
  if (result.second)
    return save_type(t);
  else
    return result.first->second;
}

/// Write a reference to a type. Note that types are not top-level entities,
/// so the are always references.
///
/// \todo: If the type is non-canonical, then it can be written inline.
void
archive_writer::write_type(const type& t)
{
  write_id(get_type(t));
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

/// Encode the declaration into a new stream. This assumes that the declaration
/// has already been inserted into the map.
std::uint32_t
archive_writer::save_decl(const decl& d)
{
  assert(decls.ids.count(&d));
  decls.bytes.emplace_back();
  write_int(d.get_kind());
  write_int(d.get_id());
  write_ref(*d.get_semantic_context());
  write_ref(*d.get_lexical_context());
  get_write(d)(*this, d); // Dispatch
  return d.get_id();
}

/// Returns the encoded id for the declaration. If the declaration has not
/// yet been encoded, it is encoded as needed.
std::uint32_t
archive_writer::get_decl(const decl& d)
{
  assert(decls.ids.size() == decls.bytes.size());
  auto result = decls.ids.emplace(&d, d.get_id());
  if (result.second)
    save_decl(d);
  return d.get_id();
}

/// Writes a declaration into the current stream. This must only be used
/// when writing declarations from declaration contexts, not references to
/// those declarations. Use write_ref() in those cases.
void
archive_writer::write_decl(const decl& d)
{
  assert(&d.get_module() == mod_);
  write_id(get_decl(d));
}

/// Write a reference to a declaration. If the declaration is defined in this
/// module and has not yet been serialized, this will cause the declaration
/// to be serialized. 
void
archive_writer::write_ref(const decl& d)
{
  assert(decls.ids.size() == decls.bytes.size());

  const module& mod = d.get_module();
  if (&mod == mod_) {
    // If the declaration is defined in this module, then encode the reference
    // as the pair (true, decl-id). 
    write_bool(true);
    write_id(get_decl(d));
  }
  else {
    // If the declaration is not in the module, then encode the reference
    // a the triple (false, module-id, decl-id).
    write_bool(false);
    write_id(get_module(mod));
    write_id(d.get_id());
  }
}

/// Write the statement into the stream.
void
archive_writer::write_stmt(const stmt& s)
{
  write_int(s.get_kind());
  get_write(s)(*this, s);
}

// Note that the offset of serialized entities uses 32 bits! We almost 
// certainly don't need 64 bits, but I won't guarantee that we can live with 
// only 16.
static archive_writer::byte_stream
serialize_block(const archive_writer::stream_table& table)
{
  // Serialize table into two output streams
  archive_writer::byte_stream data; // a concatenation of entity streams, and
  archive_writer::byte_stream off; // a mapping of indexes to relative offsets
  for (std::size_t i = 0; i != table.size(); ++i) {
    std::uint32_t pos = msbf(data.size()); 
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&pos);
    off.insert(off.end(), p, p + sizeof(pos));
    data.insert(data.end(), table[i].begin(), table[i].end());
  }

  // Serialize the type list into a new buffer, writing the number of entries
  // and the total length of the list.
  std::uint64_t len = msbf(table.size()); // Length of offset table.
  std::uint64_t end = msbf(data.size()); // Length of type list.
  archive_writer::byte_stream out;
  const unsigned char* lenp = reinterpret_cast<const unsigned char*>(&len);
  const unsigned char* endp = reinterpret_cast<const unsigned char*>(&end);
  out.insert(out.end(), lenp, lenp + sizeof(len));
  out.insert(out.end(), endp, endp + sizeof(end));
  out.insert(out.end(), off.begin(), off.end());
  out.insert(out.end(), data.begin(), data.end());
  return out;
}

/// Save the populated archive to a file.
void
archive_writer::save(const char* path)
{
  stream_table blocks {
    serialize_block(mods.bytes),
    serialize_block(types.bytes),
    serialize_block(decls.bytes),
    serialize_block(strings.bytes)
  };
  byte_stream out = serialize_block(blocks);

  // Write the file.
  std::FILE* f = std::fopen(path, "wb");
  std::fwrite(out.data(), out.size(), 1, f);
  std::fclose(f);
}

} // namespace beaker
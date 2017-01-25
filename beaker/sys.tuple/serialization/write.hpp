// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_TUPLE_SERIALIZATION_WRITE_HPP
#define BEAKER_SYS_TUPLE_SERIALIZATION_WRITE_HPP

#include <beaker/base/serialization/writer.hpp>


namespace beaker {
namespace sys_tuple {

struct write_algo : write_algorithm
{
  void operator()(archive_writer&, const type&) const override;
  void operator()(archive_writer&, const expr&) const override;
};

} // namespace sys_tuple
} // namespace beaker


#endif

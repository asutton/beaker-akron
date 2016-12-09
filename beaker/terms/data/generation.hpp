// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_TERMS_DATA_GENERATION_HPP
#define BEAKER_TERMS_DATA_GENERATION_HPP

#include <beaker/common/generation.hpp>


namespace beaker {
namespace data {

llvm::Type* generate_type(generator&, const type&);
llvm::Value* generate_expr(generator&, const expr&);

} // namespace data
} // namespace beaker


#endif

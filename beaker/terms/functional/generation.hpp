// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_TERMS_FUNCTIONAL_GENERATION_HPP
#define BEAKER_TERMS_FUNCTIONAL_GENERATION_HPP

#include <beaker/common/generation.hpp>


namespace beaker {
namespace functional {

llvm::Value* generate_expr(generator&, const expr&);

} // namespace functional
} // namespace beaker


#endif

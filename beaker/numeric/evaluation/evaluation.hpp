// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_TERM_NUMERIC_EVALUATION_HPP
#define BEAKER_TERM_NUMERIC_EVALUATION_HPP

#include <beaker/common/evaluation.hpp>

namespace beaker {
namespace numeric {

value evaluate_expr(evaluator&, const expr&);

} // namespace numeric
} // namespace beaker


#endif

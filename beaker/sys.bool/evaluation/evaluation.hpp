// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_TERM_LOGIC_EVALUATION_HPP
#define BEAKER_TERM_LOGIC_EVALUATION_HPP

#include <beaker/common/evaluation.hpp>

namespace beaker {
namespace logic {

value evaluate_expr(evaluator&, const expr&);

} // namespace logic
} // namespace beaker


#endif

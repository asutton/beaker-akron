// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_COMMON_EVALUATION_HPP
#define BEAKER_COMMON_EVALUATION_HPP

#include <beaker/common/value.hpp>


namespace beaker {

struct expr;


// Maintains context during evaluation.
struct evaluator
{

};

value evaluate(evaluator&, const expr&);

} // namespace beaker


#endif

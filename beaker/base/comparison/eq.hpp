// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_COMPARISON_EQ_HPP
#define BEAKER_BASE_COMPARISON_EQ_HPP

namespace beaker {

struct name;
struct type;
struct expr;
struct unary_expr;
struct binary_expr;

bool equivalent(const name&, const name&);
bool equivalent(const type&, const type&);
bool equivalent(const expr&, const expr&);

bool equivalent_unary_expr(const unary_expr&, const unary_expr&);
bool equivalent_binary_expr(const binary_expr&, const binary_expr&);

} // namespace beaker


#endif

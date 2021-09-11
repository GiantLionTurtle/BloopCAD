
#ifndef EXPRESSION_FORWARD_HPP_
#define EXPRESSION_FORWARD_HPP_

#include <memory>

class Expression_abstr;
class ExpVar;
class ExpEqu;
using exp_ptr = std::shared_ptr<Expression_abstr>; // TODO: change to exp_ptr
using var_ptr = std::shared_ptr<ExpVar>;
using equ_ptr = std::shared_ptr<ExpEqu>;

#endif

#include "expressionVector2.hpp"

expressionVector2::expressionVector2(expression_ptr x_, expression_ptr y_):
	x(x_),
	y(y_)
{

}

variableVector2::variableVector2(variable_ptr x_, variable_ptr y_):
	x(x_),
	y(y_)
{

}
variableVector2::variableVector2(glm::vec2 vec):
	x(new variable((double)vec.x)),
	y(new variable((double)vec.y))
{

}
expressionVector2_ptr variableVector2::expr()
{
	return std::make_shared<expressionVector2>(x->expr(), y->expr());
}
glm::vec2 variableVector2::get() const
{
	return glm::vec2(x->val(), y->val());
}
void variableVector2::set(glm::vec2 const& vec)
{
	x->set(vec.x);
	y->set(vec.y);
}
void variableVector2::operator=(glm::vec2 const& vec)
{
	set(vec);
}

void variableVector2::set_tmpConstant(bool const_)
{
	x->set_tmpConstant(const_);
	y->set_tmpConstant(const_);
}
void variableVector2::set_constant()
{
	x->set_constant();
	y->set_constant();
}


float length(expressionVector2_ptr vec)
{
	return glm::length(glm::vec2(vec->x->eval(), vec->y->eval()));
}
// expressionVector2_ptr normalize(expressionVector2_ptr vec)
// {
//    return vec / length(vec);
// }

expressionVector2_ptr operator+(expressionVector2_ptr left, expressionVector2_ptr right)
{
	return expressionVector2_ptr(new expressionVector2(left->x + right->x, left->y + right->y));
}
expressionVector2_ptr operator-(expressionVector2_ptr left, expressionVector2_ptr right)
{
	return expressionVector2_ptr(new expressionVector2(left->x - right->x, left->y - right->y));
}
expressionVector2_ptr operator*(expressionVector2_ptr left, expressionVector2_ptr right)
{
	return expressionVector2_ptr(new expressionVector2(left->x * right->x, left->y * right->y));
}
expressionVector2_ptr operator/(expressionVector2_ptr left, expressionVector2_ptr right)
{
	return expressionVector2_ptr(new expressionVector2(left->x / right->x, left->y / right->y));
}
expressionVector2_ptr operator+(expressionVector2_ptr exp)
{
	return expressionVector2_ptr(new expressionVector2(+exp->x, +exp->y));
}
expressionVector2_ptr operator-(expressionVector2_ptr exp)
{
	return expressionVector2_ptr(new expressionVector2(-exp->x, -exp->y));
}

expressionVector2_ptr pow(expressionVector2_ptr base, float power)
{
	return expressionVector2_ptr(new expressionVector2(pow(base->x, power), pow(base->y, power)));
}
expressionVector2_ptr sqrt(expressionVector2_ptr exp)
{
	return pow(exp, 0.5f);
}
expressionVector2_ptr sin(expressionVector2_ptr exp)
{
	return expressionVector2_ptr(new expressionVector2(sin(exp->x), sin(exp->y)));
}
expressionVector2_ptr asin(expressionVector2_ptr exp)
{
	return expressionVector2_ptr(new expressionVector2(asin(exp->x), asin(exp->y)));
}
expressionVector2_ptr csc(expressionVector2_ptr exp)
{
	return expressionVector2_ptr(new expressionVector2(csc(exp->x), csc(exp->y)));
}
expressionVector2_ptr cos(expressionVector2_ptr exp)
{
	return expressionVector2_ptr(new expressionVector2(cos(exp->x), cos(exp->y)));
}
expressionVector2_ptr acos(expressionVector2_ptr exp)
{
	return expressionVector2_ptr(new expressionVector2(acos(exp->x), acos(exp->y)));
}
expressionVector2_ptr sec(expressionVector2_ptr exp)
{
	return expressionVector2_ptr(new expressionVector2(sec(exp->x), sec(exp->y)));
}
expressionVector2_ptr tan(expressionVector2_ptr exp)
{
	return expressionVector2_ptr(new expressionVector2(tan(exp->x), tan(exp->y)));
}
expressionVector2_ptr atan2(expressionVector2_ptr left, expressionVector2_ptr right)
{
	return expressionVector2_ptr(new expressionVector2(atan2(left->x, right->x), atan2(left->y, left->y)));
}
expressionVector2_ptr cot(expressionVector2_ptr exp)
{
	return expressionVector2_ptr(new expressionVector2(cot(exp->x), cot(exp->y)));
}

expression_ptr dot(expressionVector2_ptr left, expressionVector2_ptr right)
{
	return left->x * right->x + left->y * right->y;
}
expressionVector2_ptr cross(expressionVector2_ptr left, expressionVector2_ptr right)
{
	// return expressionVector2_ptr(new expressionVector2(	left->y * right->z - right->y * left->z, 
	// 													left->z * right->x - right->z * left->x, 
	// 													left->x * right->y - right->x * left->y));
}
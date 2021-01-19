
#include "expressionVector.hpp"

expressionVector3::expressionVector3(expression_ptr x_, expression_ptr y_, expression_ptr z_):
	x(x_),
	y(y_),
	z(z_)
{

}

variableVector3::variableVector3(variable_ptr x_, variable_ptr y_, variable_ptr z_):
	x(x_),
	y(y_),
	z(z_)
{

}
variableVector3::variableVector3(glm::vec3 vec):
	x(new variable((double)vec.x)),
	y(new variable((double)vec.y)),
	z(new variable((double)vec.z))
{

}
expressionVector3_ptr variableVector3::expr()
{
	return std::make_shared<expressionVector3>(x->expr(), y->expr(), z->expr());
}
glm::vec3 variableVector3::get() const
{
	return glm::vec3(x->val(), y->val(), z->val());
}
void variableVector3::set(glm::vec3 const& vec)
{
	x->set(vec.x);
	y->set(vec.y);
	z->set(vec.z);
}
void variableVector3::operator=(glm::vec3 const& vec)
{
	set(vec);
}

void variableVector3::set_tmpConstant(bool const_)
{
	x->set_tmpConstant(const_);
	y->set_tmpConstant(const_);
	z->set_tmpConstant(const_);
}
void variableVector3::set_constant()
{
	x->set_constant();
	y->set_constant();
	z->set_constant();
}


float length(expressionVector3_ptr vec)
{
	return glm::length(glm::vec3(vec->x->eval(), vec->y->eval(), vec->z->eval()));
}
// expressionVector3_ptr normalize(expressionVector3_ptr vec)
// {
//    return vec / length(vec);
// }

expressionVector3_ptr operator+(expressionVector3_ptr left, expressionVector3_ptr right)
{
	return expressionVector3_ptr(new expressionVector3(left->x + right->x, left->y + right->y, left->z + right->z));
}
expressionVector3_ptr operator-(expressionVector3_ptr left, expressionVector3_ptr right)
{
	return expressionVector3_ptr(new expressionVector3(left->x - right->x, left->y - right->y, left->z - right->z));
}
expressionVector3_ptr operator*(expressionVector3_ptr left, expressionVector3_ptr right)
{
	return expressionVector3_ptr(new expressionVector3(left->x * right->x, left->y * right->y, left->z * right->z));
}
expressionVector3_ptr operator/(expressionVector3_ptr left, expressionVector3_ptr right)
{
	return expressionVector3_ptr(new expressionVector3(left->x / right->x, left->y / right->y, left->z / right->z));
}
expressionVector3_ptr operator+(expressionVector3_ptr exp)
{
	return expressionVector3_ptr(new expressionVector3(+exp->x, +exp->y, +exp->z));
}
expressionVector3_ptr operator-(expressionVector3_ptr exp)
{
	return expressionVector3_ptr(new expressionVector3(-exp->x, -exp->y, -exp->z));
}

expressionVector3_ptr pow(expressionVector3_ptr base, float power)
{
	return expressionVector3_ptr(new expressionVector3(pow(base->x, power), pow(base->y, power), pow(base->z, power)));
}
expressionVector3_ptr sqrt(expressionVector3_ptr exp)
{
	return pow(exp, 0.5f);
}
expressionVector3_ptr sin(expressionVector3_ptr exp)
{
	return expressionVector3_ptr(new expressionVector3(sin(exp->x), sin(exp->y), sin(exp->z)));
}
expressionVector3_ptr asin(expressionVector3_ptr exp)
{
	return expressionVector3_ptr(new expressionVector3(asin(exp->x), asin(exp->y), asin(exp->z)));
}
expressionVector3_ptr csc(expressionVector3_ptr exp)
{
	return expressionVector3_ptr(new expressionVector3(csc(exp->x), csc(exp->y), csc(exp->z)));
}
expressionVector3_ptr cos(expressionVector3_ptr exp)
{
	return expressionVector3_ptr(new expressionVector3(cos(exp->x), cos(exp->y), cos(exp->z)));
}
expressionVector3_ptr acos(expressionVector3_ptr exp)
{
	return expressionVector3_ptr(new expressionVector3(acos(exp->x), acos(exp->y), acos(exp->z)));
}
expressionVector3_ptr sec(expressionVector3_ptr exp)
{
	return expressionVector3_ptr(new expressionVector3(sec(exp->x), sec(exp->y), sec(exp->z)));
}
expressionVector3_ptr tan(expressionVector3_ptr exp)
{
	return expressionVector3_ptr(new expressionVector3(tan(exp->x), tan(exp->y), tan(exp->z)));
}
expressionVector3_ptr atan2(expressionVector3_ptr left, expressionVector3_ptr right)
{
	return expressionVector3_ptr(new expressionVector3(atan2(left->x, right->x), atan2(left->y, left->y), atan2(left->z, right->z)));
}
expressionVector3_ptr cot(expressionVector3_ptr exp)
{
	return expressionVector3_ptr(new expressionVector3(cot(exp->x), cot(exp->y), cot(exp->z)));
}

expression_ptr dot(expressionVector3_ptr left, expressionVector3_ptr right)
{
	return left->x * right->x + left->y * right->y + left->z * right->z;
}
expressionVector3_ptr cross(expressionVector3_ptr left, expressionVector3_ptr right)
{
	return expressionVector3_ptr(new expressionVector3(	left->y * right->z - right->y * left->z, 
														left->z * right->x - right->z * left->x, 
														left->x * right->y - right->x * left->y));
}
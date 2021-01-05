
#ifndef EXPRESSIONVECTOR_HPP_
#define EXPRESSIONVECTOR_HPP_

#include "expression.hpp"

#include <glm/glm.hpp>

class expressionVector3;
using expressionVector3_ptr = std::shared_ptr<expressionVector3>;

// TODO: make it a specilization of a glm vector?
class expressionVector3 {
public:
	expression_ptr x, y, z;
public:
	expressionVector3(expression_ptr x_, expression_ptr y_, expression_ptr z_);
};

// functions are declared outside so that the api resemble glm's
float length(expressionVector3_ptr vec);
expressionVector3_ptr normalize(expressionVector3_ptr vec);

expressionVector3_ptr operator+(expressionVector3_ptr left, expressionVector3_ptr right);
expressionVector3_ptr operator-(expressionVector3_ptr left, expressionVector3_ptr right);
expressionVector3_ptr operator*(expressionVector3_ptr left, expressionVector3_ptr right);
expressionVector3_ptr operator/(expressionVector3_ptr left, expressionVector3_ptr right);
expressionVector3_ptr operator+(expressionVector3_ptr exp);
expressionVector3_ptr operator-(expressionVector3_ptr exp);

expressionVector3_ptr pow(expressionVector3_ptr base, float power);
expressionVector3_ptr sqrt(expressionVector3_ptr exp);
expressionVector3_ptr sin(expressionVector3_ptr exp);
expressionVector3_ptr asin(expressionVector3_ptr exp);
expressionVector3_ptr csc(expressionVector3_ptr exp);
expressionVector3_ptr cos(expressionVector3_ptr exp);
expressionVector3_ptr acos(expressionVector3_ptr exp);
expressionVector3_ptr sec(expressionVector3_ptr exp);
expressionVector3_ptr tan(expressionVector3_ptr exp);
expressionVector3_ptr atan2(expressionVector3_ptr left, expressionVector3_ptr right);
expressionVector3_ptr cot(expressionVector3_ptr exp);

expression_ptr dot(expressionVector3_ptr left, expressionVector3_ptr right);
expressionVector3_ptr cross(expressionVector3_ptr left, expressionVector3_ptr right);

#endif
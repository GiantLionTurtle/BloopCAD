
#ifndef EXPRESSIONVECTOR2_HPP_
#define EXPRESSIONVECTOR2_HPP_

#include "expression.hpp"

#include <glm/glm.hpp>

class expressionVector2;
class variableVector2;
using expressionVector2_ptr = std::shared_ptr<expressionVector2>;
using variableVector2_ptr = std::shared_ptr<variableVector2>;

// TODO: make it a specilization of a glm vector?
class expressionVector2 {
public:
	expression_ptr x, y;
public:
	expressionVector2(expression_ptr x_, expression_ptr y_);
};

class variableVector2 {
public:
	variable_ptr x, y;
public:
	variableVector2(variable_ptr x_, variable_ptr y_);
	variableVector2(glm::vec2 vec);

	expressionVector2_ptr expr();

	glm::vec2 get() const;
	void set(glm::vec2 const& vec);

	void operator=(glm::vec2 const& vec);

	void set_tmpConstant(bool const_);
	void set_constant();
};

// functions are declared outside so that the api resemble glm's
float length(expressionVector2_ptr vec);
expressionVector2_ptr normalize(expressionVector2_ptr vec);

/* --------------------- Expression vec --------------------- */
expressionVector2_ptr operator+(expressionVector2_ptr left, expressionVector2_ptr right);
expressionVector2_ptr operator-(expressionVector2_ptr left, expressionVector2_ptr right);
expressionVector2_ptr operator*(expressionVector2_ptr left, expressionVector2_ptr right);
expressionVector2_ptr operator/(expressionVector2_ptr left, expressionVector2_ptr right);
expressionVector2_ptr operator+(expressionVector2_ptr exp);
expressionVector2_ptr operator-(expressionVector2_ptr exp);

expressionVector2_ptr pow(expressionVector2_ptr base, float power);
expressionVector2_ptr sqrt(expressionVector2_ptr exp);
expressionVector2_ptr sin(expressionVector2_ptr exp);
expressionVector2_ptr asin(expressionVector2_ptr exp);
expressionVector2_ptr csc(expressionVector2_ptr exp);
expressionVector2_ptr cos(expressionVector2_ptr exp);
expressionVector2_ptr acos(expressionVector2_ptr exp);
expressionVector2_ptr sec(expressionVector2_ptr exp);
expressionVector2_ptr tan(expressionVector2_ptr exp);
expressionVector2_ptr atan2(expressionVector2_ptr left, expressionVector2_ptr right);
expressionVector2_ptr cot(expressionVector2_ptr exp);

expression_ptr dot(expressionVector2_ptr left, expressionVector2_ptr right);
expressionVector2_ptr cross(expressionVector2_ptr left, expressionVector2_ptr right);

#endif

#include "expression.hpp"
#include <utils/errorLogger.hpp>
#include <cmath>

expression_ptr expConst::zero(new expression_const(0.0f));
expression_ptr expConst::one(new expression_const(1.0f));
expression_ptr expConst::two(new expression_const(2.0f));
expression_ptr expConst::pi(new expression_const(M_PI));
expression_ptr expConst::pi2(new expression_const(M_PI_2));

variable::variable()
{

}
variable::variable(std::string name_, float val_):
	mName(name_),
	mVal(val_)
{

}

expression_ptr variable::expr()
{
	return expression_ptr(new expression_variable(this->shared_from_this()));
}

expression::expression():
	mParam(nullptr)
{

}

expression_ptr expression::d()
{
	return derivative();
}

bool expression::unary(operationType op)
{
	return false;
}

unary_expression::unary_expression():
	mOperand(nullptr)
{

}
unary_expression::unary_expression(expression_ptr operand):
	mOperand(operand)
{

}

binary_expression::binary_expression():
	mLeft(nullptr),
	mRight(nullptr)
{

}
binary_expression::binary_expression(expression_ptr left, expression_ptr right):
	mLeft(left),
	mRight(right)
{

}

/* -------------- Const -------------- */
expression_const::expression_const(float val)
{
	mParam = variable_ptr(new variable("", val));
	mOp = operationType::CONST;
}

float expression_const::eval()
{
	return mParam->val();
}
expression_ptr expression_const::derivative()
{
	return expConst::zero;
}

std::string expression_const::to_string()
{
	return std::to_string(mParam->val());
}
/* -------------- End const -------------- */
/* -------------- Variable -------------- */
expression_variable::expression_variable(float val)
{
	mParam = variable_ptr(new variable);
	mOp = operationType::VARIABLE;
}
expression_variable::expression_variable(variable_ptr var)
{
	mParam = var;
	mOp = operationType::VARIABLE;
}

float expression_variable::eval()
{
	return mParam->val();
}
expression_ptr expression_variable::derivative()
{
	return expConst::one;
}

std::string expression_variable::to_string()
{
	return mParam->name();// + "[" + std::to_string(mParam->val()) + "]";
}
/* -------------- End variable -------------- */

/* -------------- Plus -------------- */
expression_plus::expression_plus(expression_ptr operand):
	unary_expression(operand)
{
	mOp = operationType::PLUS;
}

float expression_plus::eval()
{
	return mOperand->eval();
}
expression_ptr expression_plus::derivative()
{
	return mOperand->derivative();
}

std::string expression_plus::to_string()
{
	return  "+" + mOperand->to_string();
}
/* -------------- End plus -------------- */

/* -------------- Minus -------------- */
expression_minus::expression_minus(expression_ptr operand):
	unary_expression(operand)
{
	mOp = operationType::MINUS;
}

float expression_minus::eval()
{
	return -mOperand->eval();
}
expression_ptr expression_minus::derivative()
{
	return -mOperand->derivative();
}

std::string expression_minus::to_string()
{
	return  "-" + mOperand->to_string();
}
/* -------------- End minus -------------- */

/* -------------- Sin -------------- */
expression_sin::expression_sin(expression_ptr operand):
	unary_expression(operand)
{
	mOp = operationType::SIN;
}

float expression_sin::eval()
{
	return std::sin(mOperand->eval());
}
expression_ptr expression_sin::derivative()
{
	return cos(mOperand) * mOperand->derivative();
}

std::string expression_sin::to_string()
{
	return  "sin(" + mOperand->to_string() + ")";
}
/* -------------- End sin -------------- */
/* -------------- Asin -------------- */
expression_asin::expression_asin(expression_ptr operand):
	unary_expression(operand)
{
	mOp = operationType::ASIN;
}

float expression_asin::eval()
{
	return std::asin(mOperand->eval());
}
expression_ptr expression_asin::derivative()
{
	return expConst::one / sqrt(expConst::one - pow(mOperand, 2)) * mOperand->derivative();
}

std::string expression_asin::to_string()
{
	return  "asin(" + mOperand->to_string() + ")";
}
/* -------------- End asin -------------- */
/* -------------- Csc -------------- */
expression_csc::expression_csc(expression_ptr operand):
	unary_expression(operand)
{
	mOp = operationType::CSC;
}

float expression_csc::eval()
{
	return 1.0f / std::sin(mOperand->eval());
}
expression_ptr expression_csc::derivative()
{
	return -cot(mOperand) * csc(mOperand) * mOperand->derivative();
}

std::string expression_csc::to_string()
{
	return  "csc(" + mOperand->to_string() + ")";
}
/* -------------- End asin -------------- */

/* -------------- Cos -------------- */
expression_cos::expression_cos(expression_ptr operand):
	unary_expression(operand)
{
	mOp = operationType::COS;
}

float expression_cos::eval()
{
	return std::cos(mOperand->eval());
}
expression_ptr expression_cos::derivative()
{
	return -sin(mOperand) * mOperand->derivative();
}

std::string expression_cos::to_string()
{
	return  "cos(" + mOperand->to_string() + ")";
}
/* -------------- End cos -------------- */
/* -------------- Acos -------------- */
expression_acos::expression_acos(expression_ptr operand):
	unary_expression(operand)
{
	mOp = operationType::ACOS;
}

float expression_acos::eval()
{
	return std::acos(mOperand->eval());
}
expression_ptr expression_acos::derivative()
{
	return -expConst::one / sqrt(expConst::one - pow(mOperand, 2.0f)) * mOperand->derivative();
}

std::string expression_acos::to_string()
{
	return  "acos(" + mOperand->to_string() + ")";
}
/* -------------- End acos -------------- */
/* -------------- Sec -------------- */
expression_sec::expression_sec(expression_ptr operand):
	unary_expression(operand)
{
	mOp = operationType::SEC;
}

float expression_sec::eval()
{
	return 1.0f / std::cos(mOperand->eval());
}
expression_ptr expression_sec::derivative()
{
	return -sec(mOperand) * tan(mOperand) * mOperand->derivative();
}

std::string expression_sec::to_string()
{
	return  "sec(" + mOperand->to_string() + ")";
}
/* -------------- End sec -------------- */

/* -------------- Tan -------------- */
expression_tan::expression_tan(expression_ptr operand):
	unary_expression(operand)
{
	mOp = operationType::TAN;
}

float expression_tan::eval()
{
	return std::tan(mOperand->eval());
}
expression_ptr expression_tan::derivative()
{
	return pow(sec(mOperand), 2.0f) * mOperand->derivative();
}

std::string expression_tan::to_string()
{
	return  "tan(" + mOperand->to_string() + ")";
}
/* -------------- End tan -------------- */
/* -------------- Atan2 -------------- */
expression_atan2::	expression_atan2(expression_ptr left, expression_ptr right):
	binary_expression(left, right)
{
	mOp = operationType::ATAN2;
}

float expression_atan2::eval()
{
	return std::atan2(mLeft->eval(), mRight->eval());
}
expression_ptr expression_atan2::derivative()
{
	return mRight / (pow(mLeft, 2.0f) + pow(mRight, 2.0f)) * mLeft->derivative() * mRight->derivative();
}

std::string expression_atan2::to_string()
{
	return  "atan2(" + mLeft->to_string() + ", " + mRight->to_string() + ")";
}
/* -------------- End atan2 -------------- */
/* -------------- Cot -------------- */
expression_cot::expression_cot(expression_ptr operand):
	unary_expression(operand)
{
	mOp = operationType::COT;
}

float expression_cot::eval()
{
	return 1.0f / std::tan(mOperand->eval());
}
expression_ptr expression_cot::derivative()
{
	return -pow(csc(mOperand), 2.0f) * mOperand->derivative();
}

std::string expression_cot::to_string()
{
	return  "cot(" + mOperand->to_string() + ")";
}
/* -------------- End tan -------------- */


/* -------------- Add -------------- */
expression_add::expression_add(expression_ptr left, expression_ptr right):
	binary_expression(left, right)
{
	mOp = operationType::ADD;
}

float expression_add::eval()
{
	return mLeft->eval() + mRight->eval();
}
expression_ptr expression_add::derivative()
{
	return mLeft->derivative() + mRight->derivative();
}

std::string expression_add::to_string()
{
	return "(" + mLeft->to_string() + " + " + mRight->to_string() + ")";
}
/* -------------- End add -------------- */

/* -------------- Substr -------------- */
expression_substr::expression_substr(expression_ptr left, expression_ptr right):
	binary_expression(left, right)
{
	mOp = operationType::SUBSTR;
}

float expression_substr::eval()
{
	return mLeft->eval() - mRight->eval();
}
expression_ptr expression_substr::derivative()
{
	return mLeft->derivative() - mRight->derivative();
}

std::string expression_substr::to_string()
{
	return "(" + mLeft->to_string() + " - " + mRight->to_string() + ")";
}
/* -------------- End substr -------------- */


/* -------------- Mult -------------- */
expression_mult::expression_mult(expression_ptr left, expression_ptr right):
	binary_expression(left, right)
{
	mOp = operationType::MULT;
}

float expression_mult::eval()
{
	return mLeft->eval() * mRight->eval();
}
expression_ptr expression_mult::derivative()
{
	return mLeft->derivative() * mRight + mLeft * mRight->derivative();
}

std::string expression_mult::to_string()
{
	return "(" + mLeft->to_string() + " * " + mRight->to_string() + ")";
}
/* -------------- End mult -------------- */

/* -------------- Div -------------- */
expression_div::expression_div(expression_ptr left, expression_ptr right):
	binary_expression(left, right)
{
	mOp = operationType::DIV;
}

float expression_div::eval()
{
	return mLeft->eval() / mRight->eval();
}
expression_ptr expression_div::derivative()
{
	return (mLeft->derivative() * mRight - mLeft * mRight->derivative()) / pow(mRight, 2);
}

std::string expression_div::to_string()
{
	return "(" + mLeft->to_string() + " / " + mRight->to_string() + ")";
}
/* -------------- End div -------------- */

/* -------------- Pow -------------- */
expression_pow::expression_pow(expression_ptr base, expression_ptr power):
	binary_expression(base, power)
{
	mOp = operationType::POW;
}

float expression_pow::eval()
{
	return std::pow(mLeft->eval(), mRight->eval());
}
expression_ptr expression_pow::derivative()
{
	return mRight * pow(mLeft, mRight->eval() - 1.0f) * mLeft->derivative();
}

std::string expression_pow::to_string()
{
	return  "(" + mLeft->to_string() + "^" + mRight->to_string() + ")";
}
/* -------------- End pow -------------- */

expression_ptr operator+(expression_ptr left, expression_ptr right)
{
	return expression_ptr(new expression_add(left, right));
}
expression_ptr operator-(expression_ptr left, expression_ptr right)
{
	return expression_ptr(new expression_substr(left, right));
}
expression_ptr operator*(expression_ptr left, expression_ptr right)
{
	return expression_ptr(new expression_mult(left, right));
}
expression_ptr operator/(expression_ptr left, expression_ptr right)
{
	return expression_ptr(new expression_div(left, right));
}
expression_ptr operator+(expression_ptr exp)
{
	return expression_ptr(new expression_plus(exp));
}
expression_ptr operator-(expression_ptr exp)
{
	return expression_ptr(new expression_minus(exp));
}

expression_ptr pow(expression_ptr base, expression_ptr power)
{
	return expression_ptr(new expression_pow(base, power));
}
expression_ptr pow(expression_ptr base, float power)
{
	return pow(base, expression_ptr(new expression_const(power)));
}
expression_ptr sqrt(expression_ptr exp)
{
	return pow(exp, 0.5f);
}

expression_ptr sin(expression_ptr exp)
{
	return expression_ptr(new expression_sin(exp));
}
expression_ptr asin(expression_ptr exp)
{
	return expression_ptr(new expression_asin(exp));
}
expression_ptr csc(expression_ptr exp)
{
	return expression_ptr(new expression_csc(exp));
}
expression_ptr cos(expression_ptr exp)
{
	return expression_ptr(new expression_cos(exp));
}
expression_ptr acos(expression_ptr exp)
{
	return expression_ptr(new expression_acos(exp));
}
expression_ptr sec(expression_ptr exp)
{
	return expression_ptr(new expression_sec(exp));
}
expression_ptr tan(expression_ptr exp)
{
	return expression_ptr(new expression_tan(exp));
}
expression_ptr atan2(expression_ptr left, expression_ptr right)
{
	return expression_ptr(new expression_atan2(left, right));
}
expression_ptr cot(expression_ptr exp)
{
	return expression_ptr(new expression_cot(exp));
}

std::ostream& operator <<(std::ostream& os, expression_ptr exp)
{
	os<<exp->to_string();
	return os;
}
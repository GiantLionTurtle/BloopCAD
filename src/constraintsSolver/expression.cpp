
#include "expression.hpp"

#include <utils/errorLogger.hpp>
#include "equationsSystem.hpp"

#include <cmath>

expression_ptr expConst::zero(new expression_const(0.0f));
expression_ptr expConst::one(new expression_const(1.0f));
expression_ptr expConst::two(new expression_const(2.0f));
expression_ptr expConst::e(new expression_const(M_E));
expression_ptr expConst::pi(new expression_const(M_PI));
expression_ptr expConst::pi2(new expression_const(M_PI_2));

variable::variable():
	mName(""),
	mVal(0.0f),
	mFixed(false)
{

}
variable::variable(std::string name_):
	mName(name_),
	mVal(0.0f),
	mFixed(false)
{

}
variable::variable(std::string name_, float val_):
	mName(name_),
	mVal(val_),
	mFixed(true)
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

expression_ptr expression_const::make(float val)
{
	return expression_ptr(new expression_const(val));
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
	float right_val = mRight->eval();
	if(std::abs(right_val) < equationsSystem::kEpsilon)
		right_val = 1.0f;
	return mLeft->eval() / right_val;
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

/* -------------- Exp -------------- */
expression_exp::expression_exp(expression_ptr base, expression_ptr exponent):
	binary_expression(base, exponent)
{
	mOp = operationType::EXP;
}

float expression_exp::eval()
{
	return std::pow(mLeft->eval(), mRight->eval());
}
expression_ptr expression_exp::derivative()
{
	if(mLeft->fixed()) {
		return exp(mLeft, mRight) * ln(mLeft) * mRight->derivative();
	} else {
		return exp(mLeft, mRight) * (ln(mLeft) * mRight)->derivative();
	}
}

std::string expression_exp::to_string()
{
	return  "(" + mLeft->to_string() + "^" + mRight->to_string() + ")";
}
/* -------------- End exp -------------- */
/* -------------- Exp_e -------------- */
expression_exp_e::expression_exp_e(expression_ptr exponent):
	unary_expression(exponent)
{
	mOp = operationType::EXP;
}

float expression_exp_e::eval()
{
	return std::pow(M_E, mOperand->eval());
}
expression_ptr expression_exp_e::derivative()
{
	return exp(mOperand) * mOperand->derivative();
}

std::string expression_exp_e::to_string()
{
	return  "e^" + mOperand->to_string();
}
/* -------------- End exp_e -------------- */
/* -------------- Log -------------- */
expression_log::expression_log(expression_ptr base, expression_ptr arg):
	binary_expression(base, arg)
{
	mOp = operationType::LOG;
}

float expression_log::eval()
{
	return std::log(mRight->eval()) / std::log(mLeft->eval());
}
expression_ptr expression_log::derivative()
{
	return expConst::one / (mRight * ln(mLeft)) * mRight->derivative();
}

std::string expression_log::to_string()
{
	return  "log[" + mLeft->to_string() + "](" + mRight->to_string() + ")";
}
/* -------------- End log -------------- */
/* -------------- Ln -------------- */
expression_ln::expression_ln(expression_ptr arg):
	unary_expression(arg)
{
	mOp = operationType::LOG;
}

float expression_ln::eval()
{
	return std::log(mOperand->eval());
}
expression_ptr expression_ln::derivative()
{
	return exp(mOperand) * mOperand->derivative();
}

std::string expression_ln::to_string()
{
	return  "ln(" + mOperand->to_string() + ")";
}
/* -------------- End ln -------------- */


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

expression_ptr operator+(float left, expression_ptr right)
{
	return expression_const::make(left) + right;
}
expression_ptr operator-(float left, expression_ptr right)
{
	return expression_const::make(left) - right;
}
expression_ptr operator*(float left, expression_ptr right)
{
	return expression_const::make(left) * right;
}
expression_ptr operator/(float left, expression_ptr right)
{
	return expression_const::make(left) / right;
}
expression_ptr operator+(expression_ptr left, float right)
{
	return left + expression_const::make(right);
}
expression_ptr operator-(expression_ptr left, float right)
{
	return left - expression_const::make(right);
}
expression_ptr operator*(expression_ptr left, float right)
{
	return left * expression_const::make(right);
}
expression_ptr operator/(expression_ptr left, float right)
{
	return left / expression_const::make(right);
}

expression_ptr operator+(expression_ptr expr)
{
	return expression_ptr(new expression_plus(expr));
}
expression_ptr operator-(expression_ptr expr)
{
	return expression_ptr(new expression_minus(expr));
}

expression_ptr pow(expression_ptr base, expression_ptr power)
{
	return expression_ptr(new expression_pow(base, power));
}
expression_ptr pow(expression_ptr base, float power)
{
	return pow(base, expression_ptr(new expression_const(power)));
}
expression_ptr sqrt(expression_ptr expr)
{
	return pow(expr, 0.5f);
}

expression_ptr sin(expression_ptr expr)
{
	return expression_ptr(new expression_sin(expr));
}
expression_ptr asin(expression_ptr expr)
{
	return expression_ptr(new expression_asin(expr));
}
expression_ptr csc(expression_ptr expr)
{
	return expression_ptr(new expression_csc(expr));
}
expression_ptr cos(expression_ptr expr)
{
	return expression_ptr(new expression_cos(expr));
}
expression_ptr acos(expression_ptr expr)
{
	return expression_ptr(new expression_acos(expr));
}
expression_ptr sec(expression_ptr expr)
{
	return expression_ptr(new expression_sec(expr));
}
expression_ptr tan(expression_ptr expr)
{
	return expression_ptr(new expression_tan(expr));
}
expression_ptr atan2(expression_ptr left, expression_ptr right)
{
	return expression_ptr(new expression_atan2(left, right));
}
expression_ptr cot(expression_ptr expr)
{
	return expression_ptr(new expression_cot(expr));
}

expression_ptr exp(expression_ptr base, expression_ptr exponent)
{
	return expression_ptr(new expression_exp(base, exponent));
}
expression_ptr exp(expression_ptr exponent)
{
	return expression_ptr(new expression_exp_e(exponent));
}
expression_ptr log(expression_ptr base, expression_ptr arg)
{
	return expression_ptr(new expression_log(base, arg));
}
expression_ptr ln(expression_ptr arg)
{
	return expression_ptr(new expression_ln(arg));
}

std::ostream& operator <<(std::ostream& os, expression_ptr expr)
{
	os<<expr->to_string();
	return os;
}
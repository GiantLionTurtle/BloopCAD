
#include "expression.hpp"

#include <cmath>
#include <algorithm>
#include <iostream>

expression_ptr expConst::zero(new expression_const(0.0f));
expression_ptr expConst::one(new expression_const(1.0f));
expression_ptr expConst::two(new expression_const(2.0f));
expression_ptr expConst::e(new expression_const(M_E));
expression_ptr expConst::pi(new expression_const(M_PI));
expression_ptr expConst::pi2(new expression_const(M_PI_2));

int expression::n_exp = 0;

expression::expression():
	mID(n_exp++)
{

}
expression::~expression()
{
	
}

expression_ptr expression::d()
{
	return derivative();
}
var_ptr expression::get_single_var()
{
	int state = 0;
	var_ptr var = get_single_var_impl(state);
	if(state) {
		return nullptr;
	}
	return var;
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

void unary_expression::print_all_vars()
{
	if(mOperand->op() == VARIABLE) {
		std::cout<<"["<<mID<<"] "<<mOperand->id()<<"\n";
	} else {
		mOperand->print_all_vars();
	}
}
var_ptr unary_expression::get_single_var_impl(int& state)
{
	if(state)
		return nullptr;
	if(mOperand->op() == VARIABLE)
		return std::static_pointer_cast<expression_var>(mOperand);
	return mOperand->get_single_var_impl(state);
}
void unary_expression::add_changeCallBack(void* owner, std::function<void(void)> func)
{
	mOperand->add_changeCallBack(owner, func);
}
void unary_expression::delete_callBack(void* owner)
{
	mOperand->delete_callBack(owner);
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

void binary_expression::print_all_vars() 
{
	if(mLeft->op() == VARIABLE) {
		std::cout<<"["<<mID<<"] "<<mLeft->id()<<"\n"; 
	} else {
		mLeft->print_all_vars();
	}
	if(mRight->op() == VARIABLE) {
		std::cout<<"["<<mID<<"] "<<mRight->id()<<"\n"; 
	} else {
		mRight->print_all_vars();
	}
}
var_ptr binary_expression::get_single_var_impl(int& state)
{
	if(state)
		return nullptr;
	var_ptr l(nullptr), r(nullptr);
	if(mLeft->op() == VARIABLE) {
		l = std::static_pointer_cast<expression_var>(mLeft);
	} else {
		l = mLeft->get_single_var_impl(state);
	}
	if(mRight->op() == VARIABLE) {
		r = std::static_pointer_cast<expression_var>(mRight);
	} else {
		r = mRight->get_single_var_impl(state);
	}
	
	if(l != nullptr && r == nullptr) {
		return l;
	} else if(l == nullptr && r != nullptr) {
		return r;
	} else if(l != nullptr && r != nullptr) {
		if(l->id() == r->id())
			return l; // either
		state = 1;
	}
	return nullptr;
}
void binary_expression::add_changeCallBack(void* owner, std::function<void(void)> func)
{
	mLeft->add_changeCallBack(owner, func);
	mRight->add_changeCallBack(owner, func);
}
void binary_expression::delete_callBack(void* owner)
{
	mLeft->delete_callBack(owner);
	mRight->delete_callBack(owner);
}



/* -------------- Const -------------- */
expression_const::expression_const(double val)
{
	mVal = val;//= var_ptr(new variable("", val));
	mOp = operationType::CONST;
}

expression_ptr expression_const::make(double val)
{
	return expression_ptr(new expression_const(val));
}

double expression_const::eval()
{
	return mVal;//mParam->val();
}
expression_ptr expression_const::derivative()
{
	return expConst::zero;
}

std::string expression_const::to_string()
{
	return std::to_string(mVal);
}
/* -------------- End const -------------- */
/* -------------- Coefficient -------------- */
expression_coefficient::expression_coefficient()
{

}

double expression_coefficient::eval()
{
	return mVal;
}
expression_ptr expression_coefficient::derivative()
{
	return expConst::zero;
}
std::string expression_coefficient::to_string()
{
	return std::to_string(val()) + unit_symbol(mUnit);
}
void expression_coefficient::set_unit(int unit_, bool convert_currentValue_to_newUnit)
{
	if(convert_currentValue_to_newUnit) {
		double oldUnit_val = val();
		mUnit = unit_;
		set_val(oldUnit_val);
	} else {
		mUnit = unit_;
	}
	mUnit_to_internalFormat = unit_to_internalFormat(unit_);
	mInternalFormat_to_unit = internalFormat_to_unit(unit_);
}
int expression_coefficient::unit()
{
	return mUnit;
}
void expression_coefficient::set_val(double val)
{
	mVal = val * mUnit_to_internalFormat;
}
double expression_coefficient::val()
{
	return mVal * mInternalFormat_to_unit;
}

/* -------------- End coefficient -------------- */
/* -------------- Coefficient angle -------------- */
expression_coefficientAngle::expression_coefficientAngle(double angle, int unit_)
{
	set_unit(unit_, false);
	set_val(angle);
}
double expression_coefficientAngle::eval()
{
	return std::fmod(expression_coefficient::eval(), M_PI * 2.0);
}
std::shared_ptr<expression_coefficientAngle> expression_coefficientAngle::make(double angle, int unit_)
{
	return std::make_shared<expression_coefficientAngle>(angle, unit_);
}
std::string expression_coefficientAngle::unit_symbol(int unit_)
{
	switch(mUnit) {
	case units::DEG:
		return " deg";
	default:
		return " rad";
	};
}
double expression_coefficientAngle::unit_to_internalFormat(int unit_)
{
	switch(mUnit) {
	case units::DEG:
		return M_PI / 180.0;
	default:
		return 1.0;
	};
}
/* -------------- End coefficient angle -------------- */
/* -------------- Coefficient length -------------- */
expression_coefficientLength::expression_coefficientLength(double length, int unit_)
{
	set_unit(unit_, false);
	set_val(length);
}
std::shared_ptr<expression_coefficientLength> expression_coefficientLength::make(double length, int unit_)
{
	return std::make_shared<expression_coefficientLength>(length, unit_);
}
std::string expression_coefficientLength::unit_symbol(int unit_)
{
	switch(mUnit) {
	case units::MM:
		return " mm";
	case units::CM:
		return " cm";
	case units::DM:
		return " dm";
	case units::M:
		return " m";
	case units::IN:
		return " in";
	case units::FT:
		return " ft";
	default:
		return " cm";
	};
}
double expression_coefficientLength::unit_to_internalFormat(int unit_)
{
	switch(mUnit) {
	case units::MM:
		return 1.0e-1;
	case units::CM:
		return 1.0;
	case units::DM:
		return 1.0e1;
	case units::M:
		return 1.0e2;
	case units::IN:
		return 2.54;
	case units::FT:
		return 30.48;
	default:
		return 1.0;
	};
}
/* -------------- End coefficient length -------------- */

/* -------------- Variable -------------- */
expression_var::expression_var(double val, bool is_coeficient):
	mVal(val),
	mAs_coef(true),
	mIs_coef(is_coeficient),
	mExists(true),
	mIs_substituted(false),
	mIs_dragged(false),
	mSubstituant(nullptr)
{
	mOp = operationType::VARIABLE;
}

double expression_var::eval()
{
	return mIs_substituted ? mSubstituant->eval() : mVal;
}
expression_ptr expression_var::derivative()
{
	// TODO should it be in this order??
	if(mIs_substituted)
		return mSubstituant->derivative();
	if(is_deriv_zero())
		return expConst::zero;
	return expConst::one;
}

std::string expression_var::to_string()
{
	return "[" + std::to_string(mVal) + "]";
}
var_ptr expression_var::make(double val, bool is_coeficient)
{
	return var_ptr(new expression_var(val, is_coeficient));
}
int expression_var::id()
{
	return mIs_substituted ? mSubstituant->id() : mID;
}
bool expression_var::is_deriv_zero()
{
	return mIs_substituted ? mSubstituant->is_deriv_zero() : (mAs_coef || mIs_coef || mIs_dragged);
}
bool expression_var::as_coef()
{
	return mIs_substituted ? mSubstituant->as_coef() : (mAs_coef > 0);
}
int expression_var::as_coef_int()
{
	return mIs_substituted ? mSubstituant->as_coef_int() : (mAs_coef);
}
bool expression_var::is_coef()
{
	return mIs_substituted ? mSubstituant->is_coef() : (mIs_coef);
}
void expression_var::set_is_coef(bool coef)
{
	if(mIs_substituted) {
		mSubstituant->set_is_coef(coef);
	} else {
		mIs_coef = coef;
	}
}
void expression_var::set_as_coef()
{
	if(mIs_substituted) {
		mSubstituant->set_as_coef();
	} else {
		mAs_coef++;
	}
}
void expression_var::set_as_var()
{
	if(mIs_substituted) {
		mSubstituant->set_as_var();
	} else {
		mAs_coef--;
	}
}
void expression_var::reset_to_coef()
{
	if(mIs_substituted) {
		mSubstituant->reset_to_coef();
	} else {
		mAs_coef = 1;
	}
}
void expression_var::reset_to_var()
{
	if(mIs_substituted) {
		mSubstituant->reset_to_var();
	} else {
		mAs_coef = 0;
	}
}

void expression_var::set(double val)
{
	if(mIs_substituted) {
		mSubstituant->set(val);
	} else {
		if(!is_coef())
			mVal = val;
	}
	callback();
}

bool expression_var::exists() const
{
	return mIs_substituted ? mSubstituant->exists() : (mExists);
}
void expression_var::set_exists(bool ex)
{
	if(mIs_substituted) { // is it necessary, I don't remember
		mSubstituant->set_exists(ex);
	} else {
		mExists = ex;
	}
}

bool expression_var::is_substituted()
{
	return mIs_substituted;
}
void expression_var::clear_substitution()
{
	if(mSubstituant) {
		mVal = mSubstituant->mVal;
		mAs_coef = mSubstituant->mAs_coef;
	}
	mIs_substituted = false;
}
void expression_var::substitute(var_ptr sub)
{
	// sub->clear_substitution();
	mSubstituant = sub;
	mIs_substituted = true;
}

void expression_var::callback()
{
	for(auto call : mChangeCallbacks)
		std::get<1>(call)();
}
void expression_var::add_changeCallBack(void* owner, std::function<void(void)> func)
{
	mChangeCallbacks[owner] = func;
}
void expression_var::delete_callBack(void* owner)
{
	mChangeCallbacks.erase(owner);
}

/* -------------- End variable -------------- */


/* -------------- Plus -------------- */
expression_plus::expression_plus(expression_ptr operand):
	unary_expression(operand)
{
	mOp = operationType::PLUS;
}

double expression_plus::eval()
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

double expression_minus::eval()
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

double expression_sin::eval()
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

double expression_asin::eval()
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

double expression_csc::eval()
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

double expression_cos::eval()
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

double expression_acos::eval()
{
	return std::acos(mOperand->eval());
}
expression_ptr expression_acos::derivative()
{
	return -(expConst::one / sqrt(expConst::one - pow(mOperand, 2.0f))) * mOperand->derivative();
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

double expression_sec::eval()
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

double expression_tan::eval()
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

double expression_atan2::eval()
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

double expression_cot::eval()
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
/* -------------- End cot -------------- */
/* -------------- Abs -------------- */
expression_abs::expression_abs(expression_ptr operand):
	unary_expression(operand)
{
	mOp = operationType::ABS;
}

double expression_abs::eval()
{
	return std::abs(mOperand->eval());
}
expression_ptr expression_abs::derivative()
{
	if(mOperand->eval() > 0) 
		return mOperand->derivative();
	return -mOperand->derivative(); // not legit if the operands evaluates at 0 because abs'() is not defined at 0
}

std::string expression_abs::to_string()
{
	return  "|" + mOperand->to_string() + "|";
}
/* -------------- End abs -------------- */
/* -------------- Mod -------------- */
expression_mod::expression_mod(expression_ptr operand, double modulo):
	unary_expression(operand),
	mMod(modulo)
{
	mOp = operationType::MOD;
}

double expression_mod::eval()
{
	return std::fmod(mOperand->eval(), mMod);
}
expression_ptr expression_mod::derivative()
{
	return mOperand->derivative(); // reeeeeeeeeeeeee
}

std::string expression_mod::to_string()
{
	return  mOperand->to_string() + " % " + std::to_string(mMod);
}
/* -------------- End mod -------------- */

/* -------------- Add -------------- */
expression_add::expression_add(expression_ptr left, expression_ptr right):
	binary_expression(left, right)
{
	mOp = operationType::ADD;
}

double expression_add::eval()
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

double expression_substr::eval()
{
	return mLeft->eval() - mRight->eval();
}
expression_ptr expression_substr::derivative()
{
	return mLeft->derivative() - mRight->derivative();
}

void expression_substr::get_substitution_params(var_ptr& a, var_ptr& b)
{
	a = std::static_pointer_cast<expression_var>(mLeft);
	b = std::static_pointer_cast<expression_var>(mRight);
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

double expression_mult::eval()
{
	return mLeft->eval() * mRight->eval();
}
expression_ptr expression_mult::derivative()
{
	return mLeft->derivative() * mRight + mLeft * mRight->derivative();
}

std::string expression_mult::to_string()
{
	return mLeft->to_string() + " * " + mRight->to_string();
}
/* -------------- End mult -------------- */

/* -------------- Div -------------- */
expression_div::expression_div(expression_ptr left, expression_ptr right):
	binary_expression(left, right)
{
	mOp = operationType::DIV;
}

double expression_div::eval()
{
	double right_val = mRight->eval();
	if(std::abs(right_val) < 1e-5)
		right_val = 1.0f;
	return mLeft->eval() / right_val;
}
expression_ptr expression_div::derivative()
{
	return (mLeft->derivative() * mRight - mLeft * mRight->derivative()) / pow(mRight, 2.0);
}

std::string expression_div::to_string()
{
	return mLeft->to_string() + " / " + mRight->to_string();
}
/* -------------- End div -------------- */

/* -------------- Pow -------------- */
expression_pow::expression_pow(expression_ptr base, expression_ptr power):
	binary_expression(base, power)
{
	mOp = operationType::POW;
}

double expression_pow::eval()
{
	return std::pow(mLeft->eval(), mRight->eval());
}
expression_ptr expression_pow::derivative()
{
	return mRight * pow(mLeft, mRight->eval() - 1.0) * mLeft->derivative();
}

std::string expression_pow::to_string()
{
	return  "(" + mLeft->to_string() + "^" + mRight->to_string() + ")";
}
/* -------------- End pow -------------- */

/* -------------- Equation -------------- */
expression_equation::expression_equation(expression_ptr left, expression_ptr right):
	expression_substr(left, right),
	mTag(0)
{

}

expression_ptr expression_equation::derivative(var_ptr with_respect_to)
{
	with_respect_to->set_as_var();
	auto deriv = expression_substr::derivative();
	with_respect_to->set_as_coef();
	return deriv;
}

std::string expression_equation::to_string()
{
	return  mLeft->to_string() + " = " + mRight->to_string();
}
/* -------------- End equation -------------- */


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

expression_ptr operator+(double left, expression_ptr right)
{
	return expression_const::make(left) + right;
}
expression_ptr operator-(double left, expression_ptr right)
{
	return expression_const::make(left) - right;
}
expression_ptr operator*(double left, expression_ptr right)
{
	return expression_const::make(left) * right;
}
expression_ptr operator/(double left, expression_ptr right)
{
	return expression_const::make(left) / right;
}
expression_ptr operator+(expression_ptr left, double right)
{
	return left + expression_const::make(right);
}
expression_ptr operator-(expression_ptr left, double right)
{
	return left - expression_const::make(right);
}
expression_ptr operator*(expression_ptr left, double right)
{
	return left * expression_const::make(right);
}
expression_ptr operator/(expression_ptr left, double right)
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
equ_ptr equation(expression_ptr left, expression_ptr right)
{
	return std::make_shared<expression_equation>(left, right);
}
equ_ptr operator^=(expression_ptr left, expression_ptr right)
{
	return equation(left, right);
}

expression_ptr pow(expression_ptr base, expression_ptr power)
{
	return expression_ptr(new expression_pow(base, power));
}
expression_ptr pow(expression_ptr base, double power)
{
	return pow(base, expression_ptr(new expression_const(power)));
}
expression_ptr sqrt(expression_ptr expr)
{
	return pow(expr, 0.5);
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
expression_ptr abs(expression_ptr expr)
{
	return expression_ptr(new expression_abs(expr));
}
expression_ptr mod(expression_ptr expr, double modulo)
{
	return expression_ptr(new expression_mod(expr, modulo));
}

expression_ptr dot(expression_ptr x1, expression_ptr y1, expression_ptr x2, expression_ptr y2)
{
	return x1 * x2 + y1 * y2;
}

std::ostream& operator <<(std::ostream& os, expression_ptr expr)
{
	os<<expr->to_string();
	return os;
}
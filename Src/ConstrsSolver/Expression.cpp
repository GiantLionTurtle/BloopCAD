
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "Expression.hpp"

#include <Utils/Debug_util.hpp>

#include <cmath>
#include <algorithm>
#include <iostream>

/*
	@define SIMPLIFY_EXP_AT_OPERATOR enables simple simplifications
	at run time for expression creation (e.g x + 0 will return x),
	it will also catch invalid expressions such as division by zeroget_single_var
*/
// #define SIMPLIFY_EXP_AT_OPERATOR

/*
	Useful constants
*/
std::shared_ptr<ExpConst> ExpConst::zero(new ExpConst(0.0));
std::shared_ptr<ExpConst> ExpConst::one_half(new ExpConst(0.5));
std::shared_ptr<ExpConst> ExpConst::one(new ExpConst(1.0));
std::shared_ptr<ExpConst> ExpConst::two(new ExpConst(2.0));
std::shared_ptr<ExpConst> ExpConst::e(new ExpConst(M_E));
std::shared_ptr<ExpConst> ExpConst::pi(new ExpConst(M_PI));
std::shared_ptr<ExpConst> ExpConst::pi2(new ExpConst(M_PI_2));

int Expression_abstr::n_exp = 0;

Expression_abstr::Expression_abstr()
	: mID(n_exp++) // ID for debug purposes
{

}
Expression_abstr::~Expression_abstr()
{

}

exp_ptr Expression_abstr::d()
{
	return derivative();
}


Expression::Expression()
	: mDerivative(nullptr)
{

}
Expression::~Expression()
{
	
}
exp_ptr Expression::derivative()
{
	if(!mDerivative)
		mDerivative = generate_derivative();
	return mDerivative;
}

UnaryExp::UnaryExp()
	: mOperand(nullptr)
{

}
UnaryExp::UnaryExp(exp_ptr operand)
	: mOperand(operand)
{

}

BinaryExp::BinaryExp()
	: mLeft(nullptr)
	, mRight(nullptr)
{

}
BinaryExp::BinaryExp(exp_ptr left, exp_ptr right)
	: mLeft(left)
	, mRight(right)
{

}

/* -------------- Const -------------- */
ExpConst::ExpConst(double val)
{
	mVal = val;
}

exp_ptr ExpConst::make(double val)
{
	return exp_ptr(new ExpConst(val));
}

double ExpConst::eval()
{
	return mVal;
}
exp_ptr ExpConst::generate_derivative()
{
	return ExpConst::zero;
}

std::string ExpConst::to_string()
{
	return std::to_string(mVal);
}
/* -------------- End const -------------- */
/* -------------- Coefficient -------------- */
// ExpCoeff::ExpCoeff()
// {

// }

// double ExpCoeff::eval()
// {
// 	return mVal;
// }
// exp_ptr ExpCoeff::generate_derivative()
// {
// 	return ExpConst::zero;
// }
// std::string ExpCoeff::to_string()
// {
// 	return std::to_string(val()) + unit_symbol(mUnit);
// }
// void ExpCoeff::set_unit(int unit_, bool convert_currentValue_to_newUnit)
// {
// 	if(convert_currentValue_to_newUnit) {
// 		double oldUnit_val = val();
// 		mUnit = unit_;
// 		set_val(oldUnit_val);
// 	} else {
// 		mUnit = unit_;
// 	}
// 	mUnit_to_internalFormat = unit_to_internalFormat(unit_);
// 	mInternalFormat_to_unit = internalFormat_to_unit(unit_);
// }
// int ExpCoeff::unit()
// {
// 	return mUnit;
// }
// void ExpCoeff::set_val(double val)
// {
// 	mVal = val * mUnit_to_internalFormat;
// }
// double ExpCoeff::val()
// {
// 	return mVal * mInternalFormat_to_unit;
// }

// /* -------------- End coefficient -------------- */
// /* -------------- Coefficient angle -------------- */
// ExpCoeffAngle::ExpCoeffAngle(double angle, int unit_)
// {
// 	set_unit(unit_, false);
// 	set_val(angle);
// }
// double ExpCoeffAngle::eval()
// {
// 	return std::fmod(ExpCoeff::eval(), M_PI * 2.0);
// }
// std::shared_ptr<ExpCoeffAngle> ExpCoeffAngle::make(double angle, int unit_)
// {
// 	return std::make_shared<ExpCoeffAngle>(angle, unit_);
// }
// std::string ExpCoeffAngle::unit_symbol(int unit_)
// {
// 	switch(mUnit) {
// 	case units::DEG:
// 		return " deg";
// 	default:
// 		return " rad";
// 	};
// }
// double ExpCoeffAngle::unit_to_internalFormat(int unit_)
// {
// 	switch(mUnit) {
// 	case units::DEG:
// 		return M_PI / 180.0;
// 	default:
// 		return 1.0;
// 	};
// }
// /* -------------- End coefficient angle -------------- */
// /* -------------- Coefficient length -------------- */
// ExpCoeffLength::ExpCoeffLength(double length, int unit_)
// {
// 	set_unit(unit_, false);
// 	set_val(length);
// }
// std::shared_ptr<ExpCoeffLength> ExpCoeffLength::make(double length, int unit_)
// {
// 	return std::make_shared<ExpCoeffLength>(length, unit_);
// }
// std::string ExpCoeffLength::unit_symbol(int unit_)
// {
// 	switch(mUnit) {
// 	case units::MM:
// 		return " mm";
// 	case units::CM:
// 		return " cm";
// 	case units::DM:
// 		return " dm";
// 	case units::M:
// 		return " m";
// 	case units::IN:
// 		return " in";
// 	case units::FT:
// 		return " ft";
// 	default:
// 		return " cm";
// 	};
// }
// double ExpCoeffLength::unit_to_internalFormat(int unit_)
// {
// 	switch(mUnit) {
// 	case units::MM:
// 		return 1.0e-1;
// 	case units::CM:
// 		return 1.0;
// 	case units::DM:
// 		return 1.0e1;
// 	case units::M:
// 		return 1.0e2;
// 	case units::IN:
// 		return 2.54;
// 	case units::FT:
// 		return 30.48;
// 	default:
// 		return 1.0;
// 	};
// }
/* -------------- End coefficient length -------------- */

/* -------------- Variable -------------- */
ExpVarDerivative::ExpVarDerivative(ExpVar* var)
	: mVar(var)
{

}

double ExpVarDerivative::eval()
{
	if(mVar->is_deriv_zero())
		return 0.0f;
	return 1.0;
}
exp_ptr ExpVarDerivative::generate_derivative()
{
	return ExpConst::zero;
}


ExpVar::ExpVar(double val, bool fixed_)
	: mVal(val)
	, mExists(true)	
	, mAs_coeff(true)
	, mIs_substituted(false)
	, mIs_dragged(false)
	, mFrozen(fixed_ ? 2 : 0)
	, mSubstituant(nullptr)
	, mDerivative(std::make_shared<ExpVarDerivative>(this))
{
	
}

double ExpVar::eval()
{
	return is_substituted() ? mSubstituant->eval() : mVal;
}
exp_ptr ExpVar::derivative()
{
	if(mIs_substituted)
		return mSubstituant->derivative();
	return mDerivative;
}

std::string ExpVar::to_string()
{
	return "[" + std::to_string(mVal) + "]";
}
var_ptr ExpVar::make(double val, bool fixed_)
{
	return var_ptr(new ExpVar(val, fixed_));
}

bool ExpVar::is_deriv_zero()
{
	return (mAs_coeff || dragged() || frozen());
}
bool ExpVar::as_coeff()
{
	return is_substituted() ? mSubstituant->as_coeff() : (mAs_coeff);
}
void ExpVar::set_as_coeff()
{
	if(mIs_substituted) {
		mSubstituant->set_as_coeff();
	} else {
		mAs_coeff = true;
	}
}
void ExpVar::set_as_var()
{
	if(mIs_substituted) {
		mSubstituant->set_as_var();
	} else {
		mAs_coeff = false;
	}
}
void ExpVar::set(double val)
{
	if(!frozen())
		mVal = val;
}
void ExpVar::drag(double val)
{
	set(val);
	set_dragged(true);
}
bool ExpVar::dragged()
{
	return is_substituted() ? mSubstituant->dragged() : mIs_dragged;
}
void ExpVar::set_dragged(bool dr)
{
	mIs_dragged = dr;
}

int ExpVar::frozen()
{
	return mFrozen;
}
void ExpVar::set_frozen(bool fr)
{
	if(mFrozen != 2)
		mFrozen = fr;
	if(fr)
		clear_substitution();
}

int ExpVar::weight()
{
	return dragged() + frozen() * 2; // This way a temporary freeze looses to a permanent one??, is it really helpful
}

bool ExpVar::exists() const
{
	return mExists;
}
void ExpVar::set_exists(bool ex)
{
	mExists = ex;
}

bool ExpVar::is_substituted()
{
	return mIs_substituted;
}
void ExpVar::clear_substitution()
{
	mIs_substituted = false;
	mSubstituant = nullptr;
}
void ExpVar::apply_substitution()
{
	if(mIs_substituted) {
		mVal = mSubstituant->eval();
	}
}
void ExpVar::substitute(var_ptr sub)
{
	if(frozen())
		return;
	if(sub->is_substituted() && sub->mSubstituant.get() == this)
		sub->clear_substitution();
	mSubstituant = sub;
	mIs_substituted = true;
}

var_ptr ExpVar::driving(var_ptr a, var_ptr b)
{
	// if a is replaced by b, than b is driving, and conversly
	if(a->is_substituted() && a->mSubstituant == b) {
		return b;
	} else if(b->is_substituted() && b->mSubstituant == a) {
		return a;
	}
	return nullptr;
}

/* -------------- End variable -------------- */


/* -------------- Plus -------------- */
ExpPlus::ExpPlus(exp_ptr operand)
	: UnaryExp(operand)
{

}

double ExpPlus::eval()
{
	return mOperand->eval();
}
exp_ptr ExpPlus::generate_derivative()
{
	return mOperand->derivative();
}

std::string ExpPlus::to_string()
{
	return  "+" + mOperand->to_string();
}
/* -------------- End plus -------------- */

/* -------------- Minus -------------- */
ExpMinus::ExpMinus(exp_ptr operand)
	: UnaryExp(operand)
{

}

double ExpMinus::eval()
{
	return -mOperand->eval();
}
exp_ptr ExpMinus::generate_derivative()
{
	return -mOperand->derivative();
}

std::string ExpMinus::to_string()
{
	return  "-" + mOperand->to_string();
}
/* -------------- End minus -------------- */

/* -------------- Sin -------------- */
ExpSin::ExpSin(exp_ptr operand)
	: UnaryExp(operand)
{

}

double ExpSin::eval()
{
	return std::sin(mOperand->eval());
}
exp_ptr ExpSin::generate_derivative()
{
	return cos(mOperand) * mOperand->derivative();
}

std::string ExpSin::to_string()
{
	return  "sin(" + mOperand->to_string() + ")";
}
/* -------------- End sin -------------- */
/* -------------- Asin -------------- */
ExpAsin::ExpAsin(exp_ptr operand)
	: UnaryExp(operand)
{

}

double ExpAsin::eval()
{
	return std::asin(mOperand->eval());
}
exp_ptr ExpAsin::generate_derivative()
{
	return ExpConst::one / sqrt(ExpConst::one - pow(mOperand, 2)) * mOperand->derivative();
}

std::string ExpAsin::to_string()
{
	return  "asin(" + mOperand->to_string() + ")";
}
/* -------------- End asin -------------- */
/* -------------- Csc -------------- */
ExpCsc::ExpCsc(exp_ptr operand)
	: UnaryExp(operand)
{

}

double ExpCsc::eval()
{
	return 1.0f / std::sin(mOperand->eval());
}
exp_ptr ExpCsc::generate_derivative()
{
	return -cot(mOperand) * csc(mOperand) * mOperand->derivative();
}

std::string ExpCsc::to_string()
{
	return  "csc(" + mOperand->to_string() + ")";
}
/* -------------- End asin -------------- */

/* -------------- Cos -------------- */
ExpCos::ExpCos(exp_ptr operand)
	: UnaryExp(operand)
{

}

double ExpCos::eval()
{
	return std::cos(mOperand->eval());
}
exp_ptr ExpCos::generate_derivative()
{
	return -sin(mOperand) * mOperand->derivative();
}

std::string ExpCos::to_string()
{
	return  "cos(" + mOperand->to_string() + ")";
}
/* -------------- End cos -------------- */
/* -------------- Acos -------------- */
ExpAcos::ExpAcos(exp_ptr operand)
	: UnaryExp(operand)
{

}

double ExpAcos::eval()
{
	return std::acos(mOperand->eval());
}
exp_ptr ExpAcos::generate_derivative()
{
	return -(ExpConst::one / sqrt(ExpConst::one - pow(mOperand, 2.0f))) * mOperand->derivative();
}

std::string ExpAcos::to_string()
{
	return  "acos(" + mOperand->to_string() + ")";
}
/* -------------- End acos -------------- */
/* -------------- Sec -------------- */
ExpSec::ExpSec(exp_ptr operand)
	: UnaryExp(operand)
{

}

double ExpSec::eval()
{
	return 1.0f / std::cos(mOperand->eval());
}
exp_ptr ExpSec::generate_derivative()
{
	return -sec(mOperand) * tan(mOperand) * mOperand->derivative();
}

std::string ExpSec::to_string()
{
	return  "sec(" + mOperand->to_string() + ")";
}
/* -------------- End sec -------------- */

/* -------------- Tan -------------- */
ExpTan::ExpTan(exp_ptr operand)
	: UnaryExp(operand)
{

}

double ExpTan::eval()
{
	return std::tan(mOperand->eval());
}
exp_ptr ExpTan::generate_derivative()
{
	return pow(sec(mOperand), 2.0f) * mOperand->derivative();
}

std::string ExpTan::to_string()
{
	return  "tan(" + mOperand->to_string() + ")";
}
/* -------------- End tan -------------- */
/* -------------- Atan2 -------------- */
ExpAtan2::ExpAtan2(exp_ptr left, exp_ptr right)
	: BinaryExp(left, right)
{

}

double ExpAtan2::eval()
{
	return std::atan2(mLeft->eval(), mRight->eval());
}
exp_ptr ExpAtan2::generate_derivative()
{
	return mRight / (pow(mLeft, 2.0f) + pow(mRight, 2.0f)) * mLeft->derivative() * mRight->derivative();
}

std::string ExpAtan2::to_string()
{
	return  "atan2(" + mLeft->to_string() + ", " + mRight->to_string() + ")";
}
/* -------------- End atan2 -------------- */
/* -------------- Cot -------------- */
ExpCot::ExpCot(exp_ptr operand)
	: UnaryExp(operand)
{

}

double ExpCot::eval()
{
	return 1.0f / std::tan(mOperand->eval());
}
exp_ptr ExpCot::generate_derivative()
{
	return -pow(csc(mOperand), 2.0f) * mOperand->derivative();
}

std::string ExpCot::to_string()
{
	return  "cot(" + mOperand->to_string() + ")";
}
/* -------------- End cot -------------- */
/* -------------- Abs -------------- */
ExpAbsDerivative::ExpAbsDerivative(ExpAbs* var)
	: mAbs(var)
{

}

double ExpAbsDerivative::eval()
{
	if(mAbs->mOperand->eval() > 0) 
		return mAbs->mOperand->derivative()->eval();
	return -mAbs->mOperand->derivative()->eval(); // not legit if the operands evaluates at 0 because abs'() is not defined at 0
}
exp_ptr ExpAbsDerivative::generate_derivative()
{
	LOG_WARNING("This part should not be reached aaaaaaaaa >.<");
	return ExpConst::zero; // Not true but it should not be reached..
}

ExpAbs::ExpAbs(exp_ptr operand)
	: UnaryExp(operand)
{

}

double ExpAbs::eval()
{
	return std::abs(mOperand->eval());
}
exp_ptr ExpAbs::generate_derivative()
{
	return std::make_shared<ExpAbsDerivative>(this);	
}

std::string ExpAbs::to_string()
{
	return  "|" + mOperand->to_string() + "|";
}
/* -------------- End abs -------------- */
/* -------------- Mod -------------- */
ExpMod::ExpMod(exp_ptr operand, double modulo)
	: UnaryExp(operand)
	, mMod(modulo)
{

}

double ExpMod::eval()
{
	return std::fmod(mOperand->eval(), mMod);
}
exp_ptr ExpMod::generate_derivative()
{
	return mOperand->derivative(); // reeeeeeeeeeeeee
}

std::string ExpMod::to_string()
{
	return  mOperand->to_string() + " % " + std::to_string(mMod);
}
/* -------------- End mod -------------- */

/* -------------- Add -------------- */
ExpAdd::ExpAdd(exp_ptr left, exp_ptr right)
	: BinaryExp(left, right)
{

}

double ExpAdd::eval()
{
	return mLeft->eval() + mRight->eval();
}
exp_ptr ExpAdd::generate_derivative()
{
	return mLeft->derivative() + mRight->derivative();
}

std::string ExpAdd::to_string()
{
	return "(" + mLeft->to_string() + " + " + mRight->to_string() + ")";
}
/* -------------- End add -------------- */

/* -------------- Substr -------------- */
ExpSubstr::ExpSubstr(exp_ptr left, exp_ptr right)
	: BinaryExp(left, right)
{

}

double ExpSubstr::eval()
{
	return mLeft->eval() - mRight->eval();
}
exp_ptr ExpSubstr::generate_derivative()
{
	return mLeft->derivative() - mRight->derivative();
}

std::string ExpSubstr::to_string()
{
	return "(" + mLeft->to_string() + " - " + mRight->to_string() + ")";
}
/* -------------- End substr -------------- */


/* -------------- Mult -------------- */
ExpMult::ExpMult(exp_ptr left, exp_ptr right)
	: BinaryExp(left, right)
{

}

double ExpMult::eval()
{
	return mLeft->eval() * mRight->eval();
}
exp_ptr ExpMult::generate_derivative()
{
	return mLeft->derivative() * mRight + mLeft * mRight->derivative();
}

std::string ExpMult::to_string()
{
	return mLeft->to_string() + " * " + mRight->to_string();
}
/* -------------- End mult -------------- */

/* -------------- Div -------------- */
ExpDiv::ExpDiv(exp_ptr left, exp_ptr right)
	: BinaryExp(left, right)
{

}

double ExpDiv::eval()
{
	double right_val = mRight->eval();
	if(std::abs(right_val) < 1e-5)
		right_val = 1.0f;
	return mLeft->eval() / right_val;
}
exp_ptr ExpDiv::generate_derivative()
{
	return (mLeft->derivative() * mRight - mLeft * mRight->derivative()) / pow(mRight, 2.0);
}

std::string ExpDiv::to_string()
{
	return mLeft->to_string() + " / " + mRight->to_string();
}
/* -------------- End div -------------- */

/* -------------- Pow -------------- */
ExpPow::ExpPow(exp_ptr base, exp_ptr power)
	: BinaryExp(base, power)
{

}

double ExpPow::eval()
{
	return std::pow(mLeft->eval(), mRight->eval());
}
exp_ptr ExpPow::generate_derivative()
{
	return mRight->eval() * pow(mLeft, mRight->eval() - 1.0) * mLeft->derivative(); // This assumes that the right opperand will not change dynamically
}

std::string ExpPow::to_string()
{
	return  "(" + mLeft->to_string() + "^" + mRight->to_string() + ")";
}
/* -------------- End pow -------------- */

/* -------------- Equation -------------- */
ExpEqu::ExpEqu(exp_ptr left, exp_ptr right)
	: ExpSubstr(left, right)
	, mTag(0)
{

}

double ExpEqu::derivative_eval(var_ptr with_respect_to)
{
	with_respect_to->set_as_var();
	double deriv = derivative()->eval();
	with_respect_to->set_as_coeff();
	return deriv;
}

void ExpEqu::get_substitution_vars(var_ptr& a, var_ptr& b)
{
	a = std::static_pointer_cast<ExpVar>(mLeft);
	b = std::static_pointer_cast<ExpVar>(mRight);
}

var_ptr ExpEqu::get_single_var()
{
	var_ptr l(nullptr), r(nullptr);
	if(mLeft->is_var()) {
		l = std::static_pointer_cast<ExpVar>(mLeft);
	}
	if(mRight->is_var()) {
		r = std::static_pointer_cast<ExpVar>(mRight);
	}
	

	// if(l != nullptr && r == nullptr) {
	// 	return l;
	// } else if(l == nullptr && r != nullptr) {
	// 	return r;
	// } else 
	if(l != nullptr && r != nullptr) { // Only very simple substitutions are allowed
		return ExpVar::driving(l, r);
	}
	return nullptr;
}

std::string ExpEqu::to_string()
{
	return  mLeft->to_string() + " = " + mRight->to_string();
}
/* -------------- End equation -------------- */


exp_ptr operator+(exp_ptr left, exp_ptr right)
{
#ifdef SIMPLIFY_EXP_AT_OPERATOR
	if(left == ExpConst::zero) {
		return right;
	} else if(right == ExpConst::zero) {
		return left;
	}
#endif
	return exp_ptr(new ExpAdd(left, right));
}
exp_ptr operator-(exp_ptr left, exp_ptr right)
{
#ifdef SIMPLIFY_EXP_AT_OPERATOR
	if(left == ExpConst::zero) {
		return -right;
	} else if(right == ExpConst::zero) {
		return left;
	}
#endif
	return exp_ptr(new ExpSubstr(left, right));
}
exp_ptr operator*(exp_ptr left, exp_ptr right)
{
#ifdef SIMPLIFY_EXP_AT_OPERATOR
	if(left == ExpConst::zero || right == ExpConst::zero) {
		return ExpConst::zero;
	} else if(left == ExpConst::one) {
		return right;
	} else if(right == ExpConst::one) {
		return left;
	}
#endif
	return exp_ptr(new ExpMult(left, right));
}
exp_ptr operator/(exp_ptr left, exp_ptr right)
{
#ifdef SIMPLIFY_EXP_AT_OPERATOR
	if(left == ExpConst::zero) {
		return ExpConst::zero;
	} else if(right == ExpConst::zero) {
		LOG_ERROR("Division by zero.");
	} else if(right == ExpConst::one) {
		return left;
	}
#endif
	return exp_ptr(new ExpDiv(left, right));
}

exp_ptr operator+(double left, exp_ptr right)
{
#ifdef SIMPLIFY_EXP_AT_OPERATOR
	if(left == 0.0)
		return right;
#endif
	return ExpConst::make(left) + right;
}
exp_ptr operator-(double left, exp_ptr right)
{
#ifdef SIMPLIFY_EXP_AT_OPERATOR
	if(left == 0.0)
		return -right;
#endif
	return ExpConst::make(left) - right;
}
exp_ptr operator*(double left, exp_ptr right)
{
#ifdef SIMPLIFY_EXP_AT_OPERATOR
	if(left == 0.0) {
		return ExpConst::zero;
	} else if(left == 1.0) {
		return right;
	}
#endif
	return ExpConst::make(left) * right;
}
exp_ptr operator/(double left, exp_ptr right)
{
	if(left == 0.0) {
		return ExpConst::zero;
	}
	return ExpConst::make(left) / right;
}
exp_ptr operator+(exp_ptr left, double right)
{
#ifdef SIMPLIFY_EXP_AT_OPERATOR
	if(right == 0.0) {
		return left;
	}
#endif
	return left + ExpConst::make(right);
}
exp_ptr operator-(exp_ptr left, double right)
{
#ifdef SIMPLIFY_EXP_AT_OPERATOR
	if(right == 0.0) {
		return left;
	}
#endif
	return left - ExpConst::make(right);
}
exp_ptr operator*(exp_ptr left, double right)
{
#ifdef SIMPLIFY_EXP_AT_OPERATOR
	if(right == 0.0) {
		return ExpConst::zero;
	} else if(right == 1.0) {
		return left;
	}
#endif
	return left * ExpConst::make(right);
}
exp_ptr operator/(exp_ptr left, double right)
{
#ifdef SIMPLIFY_EXP_AT_OPERATOR
	if(right == 0.0) {
		LOG_ERROR("Division by 0.");
	} else if(right == 1.0) {
		return left;
	}
#endif
	return left / ExpConst::make(right);
}

exp_ptr operator+(exp_ptr expr)
{
	return exp_ptr(new ExpPlus(expr));
}
exp_ptr operator-(exp_ptr expr)
{
	return exp_ptr(new ExpMinus(expr));
}
equ_ptr equation(exp_ptr left, exp_ptr right)
{
	return std::make_shared<ExpEqu>(left, right);
}
equ_ptr operator^=(exp_ptr left, exp_ptr right)
{
	return equation(left, right);
}

exp_ptr pow(exp_ptr base, exp_ptr power)
{
	return exp_ptr(new ExpPow(base, power));
}
exp_ptr pow(exp_ptr base, double power)
{
	return pow(base, exp_ptr(new ExpConst(power)));
}
exp_ptr sqrt(exp_ptr expr)
{
	return pow(expr, ExpConst::one_half);
}

exp_ptr sin(exp_ptr expr)
{
	return exp_ptr(new ExpSin(expr));
}
exp_ptr asin(exp_ptr expr)
{
	return exp_ptr(new ExpAsin(expr));
}
exp_ptr csc(exp_ptr expr)
{
	return exp_ptr(new ExpCsc(expr));
}
exp_ptr cos(exp_ptr expr)
{
	return exp_ptr(new ExpCos(expr));
}
exp_ptr acos(exp_ptr expr)
{
	return exp_ptr(new ExpAcos(expr));
}
exp_ptr sec(exp_ptr expr)
{
	return exp_ptr(new ExpSec(expr));
}
exp_ptr tan(exp_ptr expr)
{
	return exp_ptr(new ExpTan(expr));
}
exp_ptr atan2(exp_ptr left, exp_ptr right)
{
	return exp_ptr(new ExpAtan2(left, right));
}
exp_ptr cot(exp_ptr expr)
{
	return exp_ptr(new ExpCot(expr));
}
exp_ptr abs(exp_ptr expr)
{
	return exp_ptr(new ExpAbs(expr));
}
exp_ptr mod(exp_ptr expr, double modulo)
{
	return exp_ptr(new ExpMod(expr, modulo));
}

exp_ptr dot(exp_ptr x1, exp_ptr y1, exp_ptr x2, exp_ptr y2)
{
	return x1 * x2 + y1 * y2;
}

std::ostream& operator <<(std::ostream& os, exp_ptr expr)
{
	os<<expr->to_string();
	return os;
}
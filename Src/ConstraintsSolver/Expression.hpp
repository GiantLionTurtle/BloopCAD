
#ifndef EXPRESSION_HPP_
#define EXPRESSION_HPP_

#include "Expression_forward.hpp"

#include <Bloop/BaseObject.hpp>

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <map>

/*
	@file This file describes expressions that interact with each other, currently they are all dynamic
	there might eventualy be a static (template based) version.
	Raw expressions are rarelly dealt with, the use of shared_ptr to wrap every expression simplifies allocation

	Expressions are created with functions such as cos, sin or pow with expressions as arguments as well as with
	overloaded operators such as +, -, *, etc..

	Eventually expression will have to be created by parsing a string (for useful dimensioning for instance), but it
	is not currently a thing
*/

/*
	@class Expression_abstr is a mathematical that does not implement most of it's functions,
	it describes the framework (value getter, derivative getter etc). It also has a debug id
*/
class Expression_abstr {
private:
	static int n_exp; // Number of abstract expression created
	int mID; // Debug ID of this expression
public:
	/*
		@constructor assigns the debug ID
	*/
	Expression_abstr();
	virtual ~Expression_abstr();

	/*
		@functiond eval computes the expression

		@return The value of the expression
	*/
	virtual double eval() = 0;
	/*
		@function derivative computes the derivative of the expression

		@return The derivative of the expression which is a smart pointer
	*/
	virtual exp_ptr derivative() = 0;
	/*
		@function d simple wrapped for @function derivative

		@return The derivative
	*/
	exp_ptr d();

	/*
		@function to_string prints the expression (currently it gives a very crude and ugly 
		string most of the time but it's purpose is to debug so no stress)
	*/
	virtual std::string to_string() = 0;
	/*
		@function id 

		@return The debug id of the expression
	*/
	int id() { return mID; }

	/*
		@function is_var 

		@return Wheter or not this expression is a variable

		@Note Not sure if it is a good practice, it is still manageable since it is
		a single function to check an instance type..
	*/
	virtual bool is_var() { return false; }
};

/*
	@class Expression defines a framework to reuse derivatives and not compute them
	every time @function derivative is called

	@Note This class is the ancestor of all but the ExpVar expressions
*/
class Expression : public Expression_abstr {
public:
protected:
	exp_ptr mDerivative; 	// Pointer to derivative of expression, 
							// only calculated on demand to avoid recursive derivative calculations
public:
	Expression();
	virtual ~Expression();

	virtual double eval() = 0;
	/*
		@function derivative generates a derivative if it is the first
		time it is called

		@return The derivative of the expression
	*/
	virtual exp_ptr derivative();
	/*
		@function generate_derivative creates the derivative of the expression

		@return The newly created derivative
	*/
	virtual exp_ptr generate_derivative() = 0;
};

/*
	@class UnaryExp describes an expression that has 1 parameter (e.g. sin, cos, tan)
*/
class UnaryExp : public Expression {
protected:
	exp_ptr mOperand; // Parameter of the expression onto which the expression acts
public:
	UnaryExp();
	/*
		@constructor

		@param operand The parameter of the expression
	*/
	UnaryExp(exp_ptr operand);
};

/*
	@class UnaryExp describes an expression that has 2 parameter (e.g. addition, substraction, multiplication)
*/
class BinaryExp : public Expression {
protected:
	exp_ptr mLeft, mRight; // Operands
public:
	BinaryExp();
	/*
		@constructor

		@param left		The left hand side of the expression
		@param right	The right hand side of the expression
	*/
	BinaryExp(exp_ptr left, exp_ptr right);
};

/*
	@class ExpConst is an expression that is a numeric constant or a coefficient
*/
class ExpConst : public Expression {
public:
	// Some useful constants, no need to create *zero* multiple time for instance
	static std::shared_ptr<ExpConst> zero;
	static std::shared_ptr<ExpConst> one_half;
	static std::shared_ptr<ExpConst> one;
	static std::shared_ptr<ExpConst> two;
	static std::shared_ptr<ExpConst> e;
	static std::shared_ptr<ExpConst> pi;
	static std::shared_ptr<ExpConst> pi2;
private:
	double mVal; // Value of the constant
public:
	/*
		@constructor 

		@param val The value of the constant, cannot be changed, ever
	*/
	ExpConst(double val);
	/*
		@function make is a light wrapper around std::make_shared<ExpConst>(val)
		
		@param val The value of the constant expression to create
		@return The created expression pointer

		@static
	*/
	static exp_ptr make(double val);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};


// class ExpCoeff : public Expression {
// protected:
// 	double mVal;
// 	int mUnit;
// 	double mUnit_to_internalFormat, mInternalFormat_to_unit;
// public:
// 	ExpCoeff();

// 	virtual double eval();
// 	virtual exp_ptr generate_derivative();

// 	virtual std::string to_string();

// 	void set_unit(int unit_, bool convert_currentValue_to_newUnit);
// 	int unit();
// 	void set_val(double val_);
// 	double val(); // not to confuse with eval. val() returns the value in the current unit
// protected:
// 	virtual std::string unit_symbol(int unit_) { return ""; }
// 	virtual double unit_to_internalFormat(int unit_) { return 1.0; }
// 	virtual double internalFormat_to_unit(int unit_) { return 1.0 / unit_to_internalFormat(unit_); }
// };

// class ExpCoeffAngle : public ExpCoeff {
// public:
// 	enum units { RAD, DEG };
// public:
// 	ExpCoeffAngle(double angle, int unit_ = units::RAD);

// 	virtual double eval();

// 	static std::shared_ptr<ExpCoeffAngle> make(double angle, int unit_ = units::RAD);
// protected:
// 	virtual std::string unit_symbol(int unit_);
// 	virtual double unit_to_internalFormat(int unit_);
// };
// class ExpCoeffLength : public ExpCoeff {
// public:
// 	enum units { MM, CM, DM, M, IN, FT };
// public:
// 	ExpCoeffLength(double length, int unit_ = units::CM);

// 	static std::shared_ptr<ExpCoeffLength> make(double length, int unit_ = units::CM);
// protected:
// 	virtual std::string unit_symbol(int unit_);
// 	virtual double unit_to_internalFormat(int unit_);
// };

/*
	@class ExpVarDerivative is the derivative of a variable
	it is special in that it checks if the variable is the target of
	derivation (if it is as a variable or as a coefficient) when returning
	a value
*/
class ExpVarDerivative : public Expression {
private:
	ExpVar* mVar; // Variable that this expression is the derivative of
public:
	/*
		@constructor

		@param var The variable that this expression is the derivative of
	*/
	ExpVarDerivative(ExpVar* var);

	/*
		@function eval checks if the variable is a coefficient

		@return 0 if the variable is in coefficient mode and 1 if not
	*/
	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string() { return "[var derivative]"; }
};

class ExpVar : public Expression_abstr, public BaseObject {
private:
	double mVal; // Current value of the var
	bool mExists; // Wheter the variable *exists*, as a base object, it is used when geometries are deleted but not dealocated
	bool mAs_coeff; // Variable act as coefficient by default to be able to derive complex expressions with respect to a single variable
	bool mIs_dragged; // Wheter this variable is being dragged (it has priority in the solver)
	int mFrozen; // Wheter this variable can change or not [0: not frozen, 1: frozen but can unfreeze, 2: frozen for ever (set at creation)]

	// Substitution in this case is a when a variable follows another variable so it 
	// returns the other variable's value and sets the other variable's value on set.
	// Currently it is limited to very simple equations (x1 = x2) but they are quite 
	// frequent as the point-point coincidence, horizontal and vertical constraint rely 
	// on those. This saves on solve time as those simple equations can then be solved 
	// individually and therefore reduce the size of the jacobians in the main solve
	// TODO: do this for Expressions in general??
	bool mIs_substituted; // Wheter the variable is substituted
	var_ptr mSubstituant; // Variable that drives this one if it is substituted

	std::shared_ptr<ExpVarDerivative> mDerivative; // Derivative of this variable (can be set to coeff/variable)
public:
	/*
		@constructor

		@param val The initial value of the variable
	*/
	ExpVar(double val, bool fixed_ = false);
	/*
		@function make is a light wrapper around std::make_shared<ExpVar>(val)

		@return a shared_ptr to a newly created ExpVar
	*/
	static var_ptr make(double val, bool fixed_ = false);

	virtual double eval();
	exp_ptr derivative();

	virtual std::string to_string();

	/*
		@function set sets the value of the variable

		@param val The new value of the variable
	*/
	void set(double val);
	/*
		@function drag sets the value of the variable and sets the mIs_dragged flag to true

		@param val The new value of the variable
	*/
	void drag(double val);
	/*
		@function dragged

		@return Wheter or not the mIs_dragged is true
	*/
	bool dragged();
	/*
		@function set_dragged sets the mIs_dragged flag

		@param dr The new value of the flag
	*/
	void set_dragged(bool dr);

	/*
		@function frozen

		@return Wheter or not this variable can change (>0 : cannot change, == 0: can change)
	*/
	int frozen();
	/*
		@function set_frozen

		@param fr If the variable should be frozen (immovable) or not
	*/
	void set_frozen(bool fr);

	int weight();

	bool exists() const;
	void set_exists(bool ex);

	/*
		@function is_substituted

		@return Wheter or not there is a valid substituant to the variable
	*/
	bool is_substituted();
	/*
		@function clear_substitution deletes substitution handles, the variable is 
		back to being equal to itself
	*/
	void clear_substitution();
	/*
		@function apply_substitution sets the internal value of the variable
		to the value of the substituant (which could be substituted as well)
		use this before @function clear_substitution to keep variable coherent
		in a context but dissociate it from it's substituant
	*/
	void apply_substitution();
	/*
		@function substitute sets the substituant of the variable, the 
		value of this variable will virtualy be the one of the substituant

		@param sub The new substituant 
	*/
	void substitute(var_ptr sub);

	/*
		@function is_deriv_zero

		@return Wheter the derivative of this variable is 0 
		(if it is dragged or simply not the target of the derivation)
	*/
	bool is_deriv_zero();

	/*
		@function as_coeff

		@return Wheter or not the variable is used as a coefficient
		(it is only *not* used as a coefficient when it is the 
		derivation target)
	*/
	bool as_coeff();
	/*
		@function set_as_coeff sets the variable to be used as a coefficient
		(derivative = 0)
	*/
	void set_as_coeff();
	/*
		@function set_as_var sets the variable to be used as a variable
		(derivative = 1)
	*/
	void set_as_var();

	/*
		@function is_var this variable is a variable 

		@override
		@note WOW
	*/
	bool is_var() { return true; }
	/*
		@function driving helper function to determine which of 
		two (or none) variable is driving and which is driven

		@return The substituant of the other variable
	*/
	static var_ptr driving(var_ptr a, var_ptr b);
};

/*
	@struct VarState describes a tuple between a variable and a value
*/
struct VarState {
	/*
		@constructor VarState

		@param v The variable to set
		@param s The value tupled with the variable
	*/
	VarState(var_ptr v, float s):
		var(v),
		st(s)
	{}
	VarState():
		var(nullptr),
		st(0.0f)
	{}
	var_ptr var; // Variable
	float st;	// State
};
/*
	@struct VarDualState describes a tuple between a variable and two values
	(a before and an after state)
*/
struct VarDualState {
	/*
		@constructor VarState

		@param v	The variable to set
		@param s1 	The "before" value
		@param s2 	The "after" value
	*/
	VarDualState(var_ptr v, float s1, float s2):
		var(v),
		st1(s1),
		st2(s2)
	{}
	VarDualState():
		var(nullptr),
		st1(0.0f),
		st2(0.0f)
	{}
	var_ptr var; // Variable
	float st1, st2; // Both states of the variable
};


class ExpPlus : public UnaryExp {
public:
	ExpPlus(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};

class ExpMinus : public UnaryExp {
public:
	ExpMinus(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};

class ExpSin : public UnaryExp {
public:
	ExpSin(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};
class ExpAsin : public UnaryExp {
public:
	ExpAsin(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};
class ExpCsc : public UnaryExp {
public:
	ExpCsc(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};


class ExpCos : public UnaryExp {
public:
	ExpCos(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};
class ExpAcos : public UnaryExp {
public:
	ExpAcos(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};
class ExpSec : public UnaryExp {
public:
	ExpSec(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};

class ExpTan : public UnaryExp {
public:
	ExpTan(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};
class ExpAtan2 : public BinaryExp {
public:
	ExpAtan2(exp_ptr left, exp_ptr right);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};
class ExpCot : public UnaryExp {
public:
	ExpCot(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};

/*
	@class ExpAbsDerivative is the derivative of an absolute value,
	it needs a separate class because the derivative of an absolute value depends on
	the value of it's opperand.

	@note it could be merged with ExpVarDerivative in the future
*/
class ExpAbs;
class ExpAbsDerivative : public Expression {
private:
	ExpAbs* mAbs; // Variable that this expression is the derivative of
public:
	/*
		@constructor

		@param var The ExpAbs that this expression is the derivative of
	*/
	ExpAbsDerivative(ExpAbs* abs);

	/*
		@function eval checks if the variable is a coefficient

		@return 0 if the variable is in coefficient mode and 1 if not
	*/
	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string() { return "[abs derivative]"; }
};

class ExpAbs : public UnaryExp { 
public:
	friend ExpAbsDerivative;
	ExpAbs(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};
// This one is tricky, it won't have a derivative so to speak, 
// it is a unary Expression, but has two component (one is fixed)
class ExpMod : public UnaryExp {
private:
	double mMod;
public:
	ExpMod(exp_ptr operand, double modulo);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};

class ExpAdd : public BinaryExp {
public:
	ExpAdd(exp_ptr left, exp_ptr right);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};

class ExpSubstr : public BinaryExp {
public:
	ExpSubstr(exp_ptr left, exp_ptr right);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};

class ExpMult : public BinaryExp {
public:
	ExpMult(exp_ptr left, exp_ptr right);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};

class ExpDiv : public BinaryExp {
public:
	ExpDiv(exp_ptr numerator, exp_ptr denominator);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};

class ExpPow : public BinaryExp {
public:
	/*
		@constructor ExpPow

		@param base 	The base of the exponent
		@param power 	The power of the exponent, it is assumed to not change after creation
	*/
	ExpPow(exp_ptr base, exp_ptr power);

	virtual double eval();
	virtual exp_ptr generate_derivative();

	virtual std::string to_string();
};

/*
	@class ExpEqu describes an equation, it is a fancy substraction
	with helpers for ExpressionCluster

	@parent ExpSubstr
	@parent DefaultBaseObject
*/
class ExpEqu : public ExpSubstr, public DefaultBaseObject {
private:
	int mTag; // Tag used in EquationCluster
public:
	/*
		@constructor ExpEqu

		@param left Left hand side of the equation
		@param right Right hand side of the equation
	*/
	ExpEqu(exp_ptr left, exp_ptr right);

	double derivative_eval(var_ptr with_respect_to);

	/*
		@function can substitute 

		@return If a simple substitution can be done (currently it can only be done if both sides of 
		the equation are variables)
	*/
	bool can_substitute() { return mLeft->is_var() && mRight->is_var(); }
	/*
		@function get_substitution_vars gives both sides of 
		the equation casted as ExpVar

		@param a [out] The left hand side
		@param b [out] The right hand side
	*/
	void get_substitution_vars(var_ptr& a, var_ptr& b);
	/*
		@function get_single_var gets a variable if there is only one in the equation

		@return A variable if there is only one or nullptr
		@note If two variables are present and one of them substitutes the other, it 
		still counts as one
	*/
	var_ptr get_single_var();
	/*
		@function set_tag sets the EquationCluster tag

		@param t New tag value
	*/
	void set_tag(int t) { mTag = t; }
	/*
		@function tag 

		@return The tag used by EquationCluster internaly
	*/
	int tag() { return mTag; }

	virtual std::string to_string();
};

/*
	Many operators and function are defined to create organic 
	expression writing
*/

/*
	Overloaded operators
*/
exp_ptr operator+(exp_ptr left, exp_ptr right);
exp_ptr operator-(exp_ptr left, exp_ptr right);
exp_ptr operator*(exp_ptr left, exp_ptr right);
exp_ptr operator/(exp_ptr left, exp_ptr right);
exp_ptr operator+(double left, exp_ptr right);
exp_ptr operator-(double left, exp_ptr right);
exp_ptr operator*(double left, exp_ptr right);
exp_ptr operator/(double left, exp_ptr right);
exp_ptr operator+(exp_ptr left, double right);
exp_ptr operator-(exp_ptr left, double right);
exp_ptr operator*(exp_ptr left, double right);
exp_ptr operator/(exp_ptr left, double right);
exp_ptr operator+(exp_ptr expr);
exp_ptr operator-(exp_ptr expr);
equ_ptr equation(exp_ptr left, exp_ptr right);
equ_ptr operator^=(exp_ptr left, exp_ptr right);

/*
	Functions using exp_ptr
*/
exp_ptr pow(exp_ptr base, exp_ptr power);
exp_ptr pow(exp_ptr base, double power);
exp_ptr sqrt(exp_ptr expr);
exp_ptr sin(exp_ptr expr);
exp_ptr asin(exp_ptr expr);
exp_ptr csc(exp_ptr expr);
exp_ptr cos(exp_ptr expr);
exp_ptr acos(exp_ptr expr);
exp_ptr sec(exp_ptr expr);
exp_ptr tan(exp_ptr expr);
exp_ptr atan2(exp_ptr left, exp_ptr right);
exp_ptr cot(exp_ptr expr);
exp_ptr abs(exp_ptr expr);
exp_ptr mod(exp_ptr expr, double modulo);

exp_ptr dot(exp_ptr x1, exp_ptr y1, exp_ptr x2, exp_ptr y2);

/*
	Print functions (never used as of now)
*/
std::ostream& operator <<(std::ostream& os, exp_ptr expr);
std::ostream& operator <<(std::ostream& os, var_ptr var);

#endif
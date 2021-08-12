
#ifndef EXPRESSION_HPP_
#define EXPRESSION_HPP_

#include <baseObject.hpp>

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <map>

class Expression;
class ExpVar;
class ExpEqu;
using exp_ptr = std::shared_ptr<Expression>; // TODO: change to exp_ptr
using var_ptr = std::shared_ptr<ExpVar>;
using equ_ptr = std::shared_ptr<ExpEqu>;

class Expression {
public:
protected:
	int mID;
	static int n_exp;
public:
	Expression();
	virtual ~Expression();

	virtual double eval() = 0;
	virtual exp_ptr derivative() = 0;
	exp_ptr d();

	virtual std::string to_string() = 0;
	virtual int id() { return mID; }

	virtual bool is_var() { return false; }
	virtual bool can_substitute() { return false; }
	virtual void get_substitution_vars(var_ptr& a, var_ptr& b) {}
	var_ptr get_single_var();
	virtual void print_all_vars() {}

	// Not a 100% sure the callback thingy is the right way to go, might change later
	virtual void add_changeCallBack(void* owner, std::function<void(void)> func) {}
	virtual void delete_callBack(void* owner) {}

	virtual var_ptr get_only_var_impl(int& state) { return nullptr; }
};

class unaryExpression : public Expression {
protected:
	exp_ptr mOperand;
public:
	unaryExpression();
	unaryExpression(exp_ptr operand);

	void print_all_vars();

	void add_changeCallBack(void* owner, std::function<void(void)> func);
	void delete_callBack(void* owner);

	var_ptr get_only_var_impl(int& state);
};

class binaryExpression : public Expression {
protected:
	exp_ptr mLeft, mRight;
public:
	binaryExpression();
	binaryExpression(exp_ptr left, exp_ptr right);
	
	void print_all_vars();

	void add_changeCallBack(void* owner, std::function<void(void)> func);
	void delete_callBack(void* owner);	

	var_ptr get_only_var_impl(int& state);
};

class ExpConst : public Expression {
public:
	static std::shared_ptr<ExpConst> zero;
	static std::shared_ptr<ExpConst> one;
	static std::shared_ptr<ExpConst> two;
	static std::shared_ptr<ExpConst> e;
	static std::shared_ptr<ExpConst> pi;
	static std::shared_ptr<ExpConst> pi2;
private:
	double mVal;
public:
	ExpConst(double val);

	static exp_ptr make(double val);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();
};
class ExpCoeff : public Expression {
protected:
	double mVal;
	int mUnit;
	double mUnit_to_internalFormat, mInternalFormat_to_unit;
public:
	ExpCoeff();

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();

	void set_unit(int unit_, bool convert_currentValue_to_newUnit);
	int unit();
	void set_val(double val_);
	double val(); // not to confuse with eval. val() returns the value in the current unit
protected:
	virtual std::string unit_symbol(int unit_) { return ""; }
	virtual double unit_to_internalFormat(int unit_) { return 1.0; }
	virtual double internalFormat_to_unit(int unit_) { return 1.0 / unit_to_internalFormat(unit_); }
};

class ExpCoeffAngle : public ExpCoeff {
public:
	enum units { RAD, DEG };
public:
	ExpCoeffAngle(double angle, int unit_ = units::RAD);

	virtual double eval();

	static std::shared_ptr<ExpCoeffAngle> make(double angle, int unit_ = units::RAD);
protected:
	virtual std::string unit_symbol(int unit_);
	virtual double unit_to_internalFormat(int unit_);
};
class ExpCoeffLength : public ExpCoeff {
public:
	enum units { MM, CM, DM, M, IN, FT };
public:
	ExpCoeffLength(double length, int unit_ = units::CM);

	static std::shared_ptr<ExpCoeffLength> make(double length, int unit_ = units::CM);
protected:
	virtual std::string unit_symbol(int unit_);
	virtual double unit_to_internalFormat(int unit_);
};

class ExpVar : public Expression, public baseObject {
private:
	double mVal;
	int mAs_coef;
	bool mIs_coef;
	bool mExists;
	// TODO: do this for Expressions in general??
	bool mIs_substituted, mIs_dragged;
	var_ptr mSubstituant;
	std::function<void(void)> mCallbackFunc;
	std::map<void*, std::function<void(void)>> mChangeCallbacks;
public:
	ExpVar(double val, bool is_Coeff = false);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();

	static var_ptr make(double val, bool is_coeficient = false);

	int id();

	bool is_deriv_zero();

	bool as_coef();
	int as_coef_int();
	bool is_coef();
    void set_is_coef(bool coef);

	void set_as_coef();
	void set_as_var();
	void reset_to_coef();
	void reset_to_var();

	void set(double val);

	bool exists() const;
	void set_exists(bool ex);

	bool is_substituted();
	void clear_substitution();
	void substitute(var_ptr sub);

	bool dragged() { return mIs_dragged; }
	void set_dragged(bool dr) { mIs_dragged = dr; }

	void callback();
	void add_changeCallBack(void* owner, std::function<void(void)> func);
	void delete_callBack(void* owner);

	bool is_var() { return true; }
};

struct VarState {
	VarState(var_ptr v, float s):
		var(v),
		st(s)
	{}
	VarState():
		var(nullptr),
		st(0.0f)
	{}
	var_ptr var;
	float st;
};

struct VarDualState {
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
	var_ptr var;
	float st1, st2;
};


class ExpPlus : public unaryExpression {
public:
	ExpPlus(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();
};

class ExpMinus : public unaryExpression {
public:
	ExpMinus(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();
};

class ExpSin : public unaryExpression {
public:
	ExpSin(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();
};
class ExpAsin : public unaryExpression {
public:
	ExpAsin(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();
};
class ExpCsc : public unaryExpression {
public:
	ExpCsc(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();
};


class ExpCos : public unaryExpression {
public:
	ExpCos(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();
};
class ExpAcos : public unaryExpression {
public:
	ExpAcos(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();
};
class ExpSec : public unaryExpression {
public:
	ExpSec(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();
};

class ExpTan : public unaryExpression {
public:
	ExpTan(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();
};
class ExpAtan2 : public binaryExpression {
public:
	ExpAtan2(exp_ptr left, exp_ptr right);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();
};
class ExpCot : public unaryExpression {
public:
	ExpCot(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();
};
class ExpAbs : public unaryExpression { 
public:
	ExpAbs(exp_ptr operand);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();
};
// This one is tricky, it won't have a derivative so to speak, 
// it is a unary Expression, but has two component (one is fixed)
class ExpMod : public unaryExpression {
private:
	double mMod;
public:
	ExpMod(exp_ptr operand, double modulo);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();
};

class ExpAdd : public binaryExpression {
public:
	ExpAdd(exp_ptr left, exp_ptr right);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();
};

class ExpSubstr : public binaryExpression {
public:
	ExpSubstr(exp_ptr left, exp_ptr right);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();

	virtual bool can_substitute() { return mLeft->is_var() && mRight->is_var(); }
	virtual void get_substitution_vars(var_ptr& a, var_ptr& b);
};

class ExpMult : public binaryExpression {
public:
	ExpMult(exp_ptr left, exp_ptr right);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();
};

class ExpDiv : public binaryExpression {
public:
	ExpDiv(exp_ptr numerator, exp_ptr denominator);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();
};

class ExpPow : public binaryExpression {
public:
	ExpPow(exp_ptr base, exp_ptr power);

	virtual double eval();
	virtual exp_ptr derivative();

	virtual std::string to_string();
};

class ExpEqu : public ExpSubstr, public defaultBaseObject {
private:
	int mTag;
public:
	ExpEqu(exp_ptr left, exp_ptr right);

	virtual exp_ptr derivative(var_ptr with_respect_to);

	void set_tag(int t) { mTag = t; }
	int tag() { return mTag; }

	virtual std::string to_string();
};

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

std::ostream& operator <<(std::ostream& os, exp_ptr expr);
std::ostream& operator <<(std::ostream& os, var_ptr var);

#endif
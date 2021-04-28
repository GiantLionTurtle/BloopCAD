
#ifndef EXPRESSION_HPP_
#define EXPRESSION_HPP_

#include <memory>
#include <string>
#include <vector>
#include <functional>

class expression;
class expression_var;
using expression_ptr = std::shared_ptr<expression>;
using var_ptr = std::shared_ptr<expression_var>;

class expression {
public:
	enum operationType { UNKNOWN, VARIABLE, CONST, ADD, SUBSTR, MULT, DIV, PLUS, 
	MINUS, POW, EXP, LOG, SIN, COS, TAN, ASIN, ACOS, ATAN2, CSC, SEC, COT, ABS, MOD };
protected:
	operationType mOp;
	int mID, mTag;
	static int n_exp;
public:
	expression();
	virtual ~expression();

	virtual double eval() = 0;
	virtual expression_ptr derivative() = 0;
	expression_ptr d();

	virtual std::string to_string() = 0;
	virtual int id() { return mID; }

	int op() { return mOp; }

	void set_tag(int t) { mTag = t; }
	int tag() { return mTag; }

	virtual bool can_substitute() { return false; }
	virtual void get_substitution_params(var_ptr& a, var_ptr& b) {}
	var_ptr get_single_var();
	virtual void print_all_vars() {}
	virtual var_ptr get_single_var_impl(int& state) { return nullptr; }
private:
	bool unary(operationType op);
};

class unary_expression : public expression {
protected:
	expression_ptr mOperand;
public:
	unary_expression();
	unary_expression(expression_ptr operand);

	virtual var_ptr get_single_var_impl(int& state);
	virtual void print_all_vars();
};

class binary_expression : public expression {
protected:
	expression_ptr mLeft, mRight;
public:
	binary_expression();
	binary_expression(expression_ptr left, expression_ptr right);

	virtual var_ptr get_single_var_impl(int& state);
	virtual void print_all_vars();
};

class expression_const : public expression {
private:
	double mVal;
public:
	expression_const(double val);

	static expression_ptr make(double val);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};
class expression_coefficient : public expression {
protected:
	double mVal;
	int mUnit;
	double mUnit_to_internalFormat, mInternalFormat_to_unit;
public:
	expression_coefficient();

	virtual double eval();
	virtual expression_ptr derivative();

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

class expression_coefficientAngle : public expression_coefficient {
public:
	enum units { RAD, DEG };
public:
	expression_coefficientAngle(double angle, int unit_ = units::RAD);

	virtual double eval();

	static std::shared_ptr<expression_coefficientAngle> make(double angle, int unit_ = units::RAD);
protected:
	virtual std::string unit_symbol(int unit_);
	virtual double unit_to_internalFormat(int unit_);
};
class expression_coefficientLength : public expression_coefficient {
public:
	enum units { MM, CM, DM, M, IN, FT };
public:
	expression_coefficientLength(double length, int unit_ = units::CM);

	static std::shared_ptr<expression_coefficientLength> make(double length, int unit_ = units::CM);
protected:
	virtual std::string unit_symbol(int unit_);
	virtual double unit_to_internalFormat(int unit_);
};

class expression_var : public expression {
private:
	double mVal;
	int mAs_coef;
	bool mIs_coef;
	bool mExists;

	// TODO: do this for expressions in general??
	bool mIs_substituted, mIs_dragged;
	var_ptr mSubstituant;
public:
	expression_var(double val, bool is_coefficient = false);

	virtual double eval();
	virtual expression_ptr derivative();

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

	bool exists();
	void set_exists(bool ex);

	bool is_substituted();
	void clear_substitution();
	void substitute(var_ptr sub);

	bool dragged() { return mIs_dragged; }
	void set_dragged(bool dr) { mIs_dragged = dr; }
};

class expression_plus : public unary_expression {
public:
	expression_plus(expression_ptr operand);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

class expression_minus : public unary_expression {
public:
	expression_minus(expression_ptr operand);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

class expression_sin : public unary_expression {
public:
	expression_sin(expression_ptr operand);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};
class expression_asin : public unary_expression {
public:
	expression_asin(expression_ptr operand);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};
class expression_csc : public unary_expression {
public:
	expression_csc(expression_ptr operand);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};


class expression_cos : public unary_expression {
public:
	expression_cos(expression_ptr operand);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};
class expression_acos : public unary_expression {
public:
	expression_acos(expression_ptr operand);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};
class expression_sec : public unary_expression {
public:
	expression_sec(expression_ptr operand);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

class expression_tan : public unary_expression {
public:
	expression_tan(expression_ptr operand);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};
class expression_atan2 : public binary_expression {
public:
	expression_atan2(expression_ptr left, expression_ptr right);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};
class expression_cot : public unary_expression {
public:
	expression_cot(expression_ptr operand);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};
class expression_abs : public unary_expression { 
public:
	expression_abs(expression_ptr operand);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};
// This one is tricky, it won't have a derivative so to speak, 
// it is a unary expression, but has two component (one is fixed)
class expression_mod : public unary_expression {
private:
	double mMod;
public:
	expression_mod(expression_ptr operand, double modulo);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

class expression_add : public binary_expression {
public:
	expression_add(expression_ptr left, expression_ptr right);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

class expression_substr : public binary_expression {
public:
	expression_substr(expression_ptr left, expression_ptr right);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();

	virtual bool can_substitute() { return mLeft->op() == VARIABLE && mRight->op() == VARIABLE; }
	virtual void get_substitution_params(var_ptr& a, var_ptr& b);
};

class expression_mult : public binary_expression {
public:
	expression_mult(expression_ptr left, expression_ptr right);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

class expression_div : public binary_expression {
public:
	expression_div(expression_ptr numerator, expression_ptr denominator);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

class expression_pow : public binary_expression {
public:
	expression_pow(expression_ptr base, expression_ptr power);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

expression_ptr operator+(expression_ptr left, expression_ptr right);
expression_ptr operator-(expression_ptr left, expression_ptr right);
expression_ptr operator*(expression_ptr left, expression_ptr right);
expression_ptr operator/(expression_ptr left, expression_ptr right);
expression_ptr operator+(double left, expression_ptr right);
expression_ptr operator-(double left, expression_ptr right);
expression_ptr operator*(double left, expression_ptr right);
expression_ptr operator/(double left, expression_ptr right);
expression_ptr operator+(expression_ptr left, double right);
expression_ptr operator-(expression_ptr left, double right);
expression_ptr operator*(expression_ptr left, double right);
expression_ptr operator/(expression_ptr left, double right);
expression_ptr operator+(expression_ptr expr);
expression_ptr operator-(expression_ptr expr);

expression_ptr pow(expression_ptr base, expression_ptr power);
expression_ptr pow(expression_ptr base, double power);
expression_ptr sqrt(expression_ptr expr);
expression_ptr sin(expression_ptr expr);
expression_ptr asin(expression_ptr expr);
expression_ptr csc(expression_ptr expr);
expression_ptr cos(expression_ptr expr);
expression_ptr acos(expression_ptr expr);
expression_ptr sec(expression_ptr expr);
expression_ptr tan(expression_ptr expr);
expression_ptr atan2(expression_ptr left, expression_ptr right);
expression_ptr cot(expression_ptr expr);
expression_ptr abs(expression_ptr expr);
expression_ptr mod(expression_ptr expr, double modulo);

expression_ptr dot(expression_ptr x1, expression_ptr y1, expression_ptr x2, expression_ptr y2);

std::ostream& operator <<(std::ostream& os, expression_ptr expr);
std::ostream& operator <<(std::ostream& os, var_ptr var);

class expConst {
public:
	static expression_ptr zero;
	static expression_ptr one;
	static expression_ptr two;
	static expression_ptr e;
	static expression_ptr pi;
	static expression_ptr pi2;
};

#endif
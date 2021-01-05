
#ifndef EXPRESSION_HPP_
#define EXPRESSION_HPP_

#include <memory>
#include <string>

class expression;
class variable;
using expression_ptr = std::shared_ptr<expression>;
using variable_ptr = std::shared_ptr<variable>;

class variable : public std::enable_shared_from_this<variable> {
private:
	std::string mName;
	float mVal;
	bool mFixed;
public:
	variable();
	variable(std::string name_);
	variable(std::string name_, float val_);

	expression_ptr expr();

	std::string name() const { return mName; }
	void set_name(std::string name_) { mName = name_; }

	float val() const { return mVal; }
	void set_val(float val_) { mVal = val_;}

	bool fixed() const { return mFixed; }
	void set_fixed(bool fixed_) { mFixed = fixed_; }
};

class expression {
public:
	enum operationType { UNKNOWN, VARIABLE, CONST, ADD, SUBSTR, MULT, DIV, PLUS, MINUS, POW, EXP, LOG, SIN, COS, TAN, ASIN, ACOS, ATAN2, CSC, SEC, COT };
protected:
	operationType mOp;
	variable_ptr mParam;
public:
	expression();

	virtual float eval() = 0;
	virtual expression_ptr derivative() = 0;
	expression_ptr d();

	virtual std::string to_string() = 0;

	virtual bool fixed() = 0;
private:
	bool unary(operationType op);
};

class unary_expression : public expression {
protected:
	expression_ptr mOperand;
public:
	unary_expression();
	unary_expression(expression_ptr operand);

	virtual bool fixed() { return mOperand->fixed(); }
};

class binary_expression : public expression {
protected:
	expression_ptr mLeft, mRight;
public:
	binary_expression();
	binary_expression(expression_ptr left, expression_ptr right);

	virtual bool fixed() { return mLeft->fixed() && mRight->fixed(); }
};

class expression_const : public expression {
public:
	expression_const(float val);

	static expression_ptr make(float val);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();

	virtual bool fixed() { return true; }
};

class expression_variable : public expression {
public:
	expression_variable(float val);
	expression_variable(variable_ptr var);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();

	virtual bool fixed() { return mParam->fixed(); }
};

class expression_plus : public unary_expression {
public:
	expression_plus(expression_ptr operand);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

class expression_minus : public unary_expression {
public:
	expression_minus(expression_ptr operand);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

class expression_sin : public unary_expression {
public:
	expression_sin(expression_ptr operand);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};
class expression_asin : public unary_expression {
public:
	expression_asin(expression_ptr operand);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};
class expression_csc : public unary_expression {
public:
	expression_csc(expression_ptr operand);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};


class expression_cos : public unary_expression {
public:
	expression_cos(expression_ptr operand);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};
class expression_acos : public unary_expression {
public:
	expression_acos(expression_ptr operand);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};
class expression_sec : public unary_expression {
public:
	expression_sec(expression_ptr operand);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

class expression_tan : public unary_expression {
public:
	expression_tan(expression_ptr operand);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};
class expression_atan2 : public binary_expression {
public:
	expression_atan2(expression_ptr left, expression_ptr right);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};
class expression_cot : public unary_expression {
public:
	expression_cot(expression_ptr operand);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};


class expression_add : public binary_expression {
public:
	expression_add(expression_ptr left, expression_ptr right);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

class expression_substr : public binary_expression {
public:
	expression_substr(expression_ptr left, expression_ptr right);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

class expression_mult : public binary_expression {
public:
	expression_mult(expression_ptr left, expression_ptr right);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

class expression_div : public binary_expression {
public:
	expression_div(expression_ptr numerator, expression_ptr denominator);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

class expression_pow : public binary_expression {
public:
	expression_pow(expression_ptr base, expression_ptr power);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

class expression_exp : public binary_expression {
public:
	expression_exp(expression_ptr base, expression_ptr exponent);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

class expression_exp_e : public unary_expression {
public:
	expression_exp_e(expression_ptr exponent);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

class expression_log : public binary_expression {
public:
	expression_log(expression_ptr base, expression_ptr arg);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

class expression_ln : public unary_expression {
public:
	expression_ln(expression_ptr arg);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
};

expression_ptr operator+(expression_ptr left, expression_ptr right);
expression_ptr operator-(expression_ptr left, expression_ptr right);
expression_ptr operator*(expression_ptr left, expression_ptr right);
expression_ptr operator/(expression_ptr left, expression_ptr right);
expression_ptr operator+(float left, expression_ptr right);
expression_ptr operator-(float left, expression_ptr right);
expression_ptr operator*(float left, expression_ptr right);
expression_ptr operator/(float left, expression_ptr right);
expression_ptr operator+(expression_ptr left, float right);
expression_ptr operator-(expression_ptr left, float right);
expression_ptr operator*(expression_ptr left, float right);
expression_ptr operator/(expression_ptr left, float right);
expression_ptr operator+(expression_ptr expr);
expression_ptr operator-(expression_ptr expr);

expression_ptr pow(expression_ptr base, expression_ptr power);
expression_ptr pow(expression_ptr base, float power);
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

expression_ptr exp(expression_ptr base, expression_ptr exponent);
expression_ptr exp(expression_ptr exponent);
expression_ptr log(expression_ptr base, expression_ptr arg);
expression_ptr ln(expression_ptr arg);

std::ostream& operator <<(std::ostream& os, expression_ptr expr);

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
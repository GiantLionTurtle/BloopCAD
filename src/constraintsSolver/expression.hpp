
#ifndef EXPRESSION_HPP_
#define EXPRESSION_HPP_

#include <memory>
#include <string>

class expression;
using expression_ptr = std::shared_ptr<expression>;

class expression {
public:
	enum operationType { UNKNOWN, CONST, ADD, SUBSTR, MULT, DIV, PLUS, MINUS, POW, SIN, COS, TAN, ASIN, ACOS, ATAN2, CSC, SEC, COT };
protected:
	operationType mOp;
	float mParam;
public:
	virtual float eval() = 0;
	virtual expression_ptr derivative() = 0;

	virtual std::string to_string() = 0;
private:
	bool unary(operationType op);
};

class unary_expression : public expression {
protected:
	expression_ptr mOperand;
public:
	unary_expression();
	unary_expression(expression_ptr operand);
};

class binary_expression : public expression {
protected:
	expression_ptr mLeft, mRight;
public:
	binary_expression();
	binary_expression(expression_ptr left, expression_ptr right);
};

class expression_const : public expression {
public:
	expression_const(float val);

	virtual float eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();
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


expression_ptr operator+(expression_ptr left, expression_ptr right);
expression_ptr operator-(expression_ptr left, expression_ptr right);
expression_ptr operator*(expression_ptr left, expression_ptr right);
expression_ptr operator/(expression_ptr left, expression_ptr right);
expression_ptr operator+(expression_ptr exp);
expression_ptr operator-(expression_ptr exp);

expression_ptr pow(expression_ptr base, expression_ptr power);
expression_ptr pow(expression_ptr base, float power);
expression_ptr sqrt(expression_ptr exp);
expression_ptr sin(expression_ptr exp);
expression_ptr asin(expression_ptr exp);
expression_ptr csc(expression_ptr exp);
expression_ptr cos(expression_ptr exp);
expression_ptr acos(expression_ptr exp);
expression_ptr sec(expression_ptr exp);
expression_ptr tan(expression_ptr exp);
expression_ptr atan2(expression_ptr left, expression_ptr right);
expression_ptr cot(expression_ptr exp);

class expConst {
public:
	static expression_ptr one;
	static expression_ptr two;
	static expression_ptr pi;
	static expression_ptr pi2;
};

#endif
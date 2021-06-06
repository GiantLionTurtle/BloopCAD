
#ifndef CONSTRAINT_HPP_
#define CONSTRAINT_HPP_

#include "expression.hpp"
#include <entities/tangibleEntities/sketchEntities/sketchPoint.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchLine.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchCircle.hpp>
#include <utils/errorLogger.hpp>
#include <baseObject.hpp>

#include <array>

class constraint_abstract : public defaultBaseObject {
public:
	enum constraint_types { NONE, COINCIDENCE, PERPENDICULARITY, PARALLELISM, HORIZONTALITY, VERTICALITY }; // high level representation (for little annotations and such)
protected:
	int mType;//, mTag;
public:
	constraint_abstract(int type):
		// mTag(0),
		mType(type)
	{}
	virtual double error(size_t eq) = 0;
	virtual bool satisfied() = 0;
	virtual double derivative(var_ptr withRespectTo_var, size_t from_eq) = 0;
	double d(var_ptr withRespectTo, size_t from_eq = 0) { return derivative(withRespectTo, from_eq); }

	virtual expression_ptr equ(size_t ind) = 0;
	virtual size_t n_equs() = 0;

	virtual var_ptr var(size_t ind) = 0;
	virtual size_t n_vars() = 0;

	virtual std::string name() = 0;

	void set_type(int tp) { mType = tp; }
	int type() { return mType; }

	// int tag() { return mTag; }
	// void set_tag(int t) { mTag = t; }
};

// TODO specialization for nE = 1 to avoid loop??
template<size_t nV, size_t nE = 1>
class constraint : public constraint_abstract {
protected:
	std::array<expression_ptr, nE> mEqus;
	std::array<var_ptr, nV> mVars;
public:
	constraint(std::array<var_ptr, nV> vars, expression_ptr equ, int type):
		constraint_abstract(type),
		mVars(vars),
		mEqus({equ})
	{

	}
	constraint(std::array<var_ptr, nV> vars, std::array<expression_ptr, nE> equs, int type):
		constraint_abstract(type),
		mVars(vars),
		mEqus(equs)
	{

	}

	double error(size_t eq) 
	{
		return mEqus[eq]->eval();
	}
	bool satisfied() 
	{
		for(size_t i = 0; i < mEqus.size(); ++i) {
			if(std::abs(mEqus[i]->eval()) > 1e-12)
				return false;
		}
		return true;
	}
	double derivative(var_ptr withRespectTo, size_t from_eq)
	{
		// TODO: would going through the constraint's variables to check if the target
		// is in the constraint save time? ; output zero if the variable is unknown to this constraint
		withRespectTo->set_as_var();
		double der = mEqus[from_eq]->d()->eval();
		withRespectTo->set_as_coef();
		return der;
	}

	expression_ptr equ(size_t ind) { return mEqus.at(ind); }
	constexpr size_t n_equs() { return mEqus.size(); }

	var_ptr var(size_t ind) { return mVars.at(ind); }
	constexpr size_t n_vars() { return mVars.size(); }
};

class pointPoint_horizontality : public constraint<2> {
public:
	static std::shared_ptr<pointPoint_horizontality> make(sketchPoint_ptr p1, sketchPoint_ptr p2);
	static std::shared_ptr<pointPoint_horizontality> make(sketchLine_ptr l);

	std::string name() { return "pointPoint_horizontality"; }
private:
	pointPoint_horizontality(sketchPoint_ptr p1, sketchPoint_ptr p2);
};
class pointPoint_verticality : public constraint<2> {
public:
	static std::shared_ptr<pointPoint_verticality> make(sketchPoint_ptr p1, sketchPoint_ptr p2);
	static std::shared_ptr<pointPoint_verticality> make(sketchLine_ptr l);
	std::string name() { return "pointPoint_verticality"; }
private:
	pointPoint_verticality(sketchPoint_ptr p1, sketchPoint_ptr p2);
};

class pointPoint_distance : public constraint<4> {
public:
	static std::shared_ptr<pointPoint_distance> make(sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr d);
	// static std::shared_ptr<pointPoint_distance> make_coincident(sketchPoint_ptr p1, sketchPoint_ptr p2);
	static std::shared_ptr<pointPoint_distance> make(sketchLine_ptr l, expression_ptr d);

	std::string name() { return "pointPoint_distance"; }
private:
	pointPoint_distance(sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr d);
};

class pointPoint_coincidence : public constraint<4, 2> {
public:
	static std::shared_ptr<pointPoint_coincidence> make(sketchPoint_ptr p1, sketchPoint_ptr p2);

	std::string name() { return "pointPoint_coincidence"; }
private:
	pointPoint_coincidence(sketchPoint_ptr p1, sketchPoint_ptr p2);
};


class pointPoint_horizontalDistance : public constraint<2> {
public:
	static std::shared_ptr<pointPoint_horizontalDistance> make(sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr d);

	std::string name() { return "pointPoint_horizontalDistance"; }
private:	
	pointPoint_horizontalDistance(sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr d);
};

class pointPoint_verticalDistance : public constraint<2> {
public:
	static std::shared_ptr<pointPoint_verticalDistance> make(sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr d);

	std::string name() { return "pointPoint_verticalDistance"; }
private:
	pointPoint_verticalDistance(sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr d);
};

class pointLine_distance : public constraint<6> {
public:
	static std::shared_ptr<pointLine_distance> make(sketchPoint_ptr p, sketchLine_ptr l, expression_ptr d);
	static std::shared_ptr<pointLine_distance> make(sketchCircle_ptr c, sketchLine_ptr l);
	static std::shared_ptr<pointLine_distance> make(sketchCircle_ptr c, sketchLine_ptr l, expression_ptr d);

	std::string name() { return "pointLine_distance"; }
private:
	pointLine_distance(sketchPoint_ptr p, sketchLine_ptr l, expression_ptr d);
};

class pointCircle_distance : public constraint<5> {
public:
	static std::shared_ptr<pointCircle_distance> make(sketchPoint_ptr p, sketchCircle_ptr c);
	static std::shared_ptr<pointCircle_distance> make(sketchPoint_ptr p, sketchCircle_ptr c, expression_ptr d);

	std::string name() { return "pointCircle_distance"; }
private:
	pointCircle_distance(sketchPoint_ptr p, sketchCircle_ptr c, expression_ptr d);
};

class lineCircle_distance : public constraint<7> {
public:
	static std::shared_ptr<lineCircle_distance> make(sketchLine_ptr l, sketchCircle_ptr c);
	static std::shared_ptr<lineCircle_distance> make(sketchLine_ptr l, sketchCircle_ptr c, expression_ptr d);

	std::string name() { return "lineCircle_distance"; }
private:
	lineCircle_distance(sketchLine_ptr l, sketchCircle_ptr c, expression_ptr d);
};

class lineLine_angle : public constraint<8> {
public:
	static std::shared_ptr<lineLine_angle> make(sketchLine_ptr l1, sketchLine_ptr l2, expression_ptr a);
	static std::shared_ptr<lineLine_angle> make_parallel(sketchLine_ptr l1, sketchLine_ptr l2);
	static std::shared_ptr<lineLine_angle> make_perpendicular(sketchLine_ptr l1, sketchLine_ptr l2);

	std::string name() { return "lineLine_angle"; }
private:
	lineLine_angle(sketchLine_ptr l1, sketchLine_ptr l2, expression_ptr a);
};

class lineLine_equal : public constraint<8> {
public:
	std::shared_ptr<lineLine_equal> make(sketchLine_ptr l1, sketchLine_ptr l2);

	std::string name() { return "lineLine_equal"; }
private:
	lineLine_equal(sketchLine_ptr l1, sketchLine_ptr l2);
};

class lineCircle_equal : public constraint<5> {
public:
	std::shared_ptr<lineCircle_equal> make(sketchLine_ptr l, sketchCircle_ptr c);

	std::string name() { return "lineCircle_equal"; }
private:
	lineCircle_equal(sketchLine_ptr l, sketchCircle_ptr c);
};

#endif
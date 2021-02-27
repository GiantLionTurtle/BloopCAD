
#ifndef CONSTRAINT_HPP_
#define CONSTRAINT_HPP_

#include "expression.hpp"
#include <entities/tangibleEntities/sketchEntities/sketchPoint.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchLine.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchCircle.hpp>

#include <array>

class constraint_abstract {
public:
	virtual double error() = 0;
	virtual bool satisfied() = 0;
	virtual double derivative(variable_ptr withRespectTo) = 0;
	double d(variable_ptr withRespectTo) { return derivative(withRespectTo); }

	virtual variable_ptr var(size_t ind) = 0;
	virtual size_t n_var() = 0;

	virtual std::string name() = 0;
};

template<size_t nV>
class constraint : public constraint_abstract {
protected:
	expression_ptr mEqu;
	std::array<variable_ptr, nV> mVars;
public:
	constraint(expression_ptr equ, std::array<variable_ptr, nV> vars):
		mEqu(equ),
		mVars(vars)
	{

	}

	double error() 
	{
		return mEqu->eval();
	}
	bool satisfied() 
	{
		return std::abs(mEqu->eval()) < 1e-12;
	}
	double derivative(variable_ptr withRespectTo)
	{
		// TODO: would going through the constraint's variables to check if the target
		// is in the constraint save time? ; output zero if the variable is unknown to this constraint
		withRespectTo->set_as_var();
		double der = mEqu->d()->eval();
		withRespectTo->set_as_coef();
		return der;
	}

	variable_ptr var(size_t ind) { return mVars.at(ind); }
	size_t n_var() { return mVars.size(); }

	virtual std::string name() = 0;
};

// class coincidence : public constraint<2, 4> {
// public:
// 	coincidence(sketchPoint_ptr p1, sketchPoint_ptr p2):
// 		constraint<2, 4>({ p1->x() - p2->x(), p1->y() - p2->y()}, { p1->x(), p1->y(), p2->x(), p2->y() })
// 	{
// 		// Equations: p1->x() - p2->x()  &  p1->y() - p2->y()

// 		// Coincident points have the same coordinate ; x1 = x2 & y1 = y2 ; ie. x1 - x2 = 0 & y1 - y2 = 0
// 		// This coinstraint could be defined as a pointpoint_distance with 0, but it seems more efficient to have
// 		// Two first degree equation rather than one second degree equation
// 	}
// };

class pointPoint_horizontality : public constraint<2> {
public:
	pointPoint_horizontality(sketchPoint_ptr p1, sketchPoint_ptr p2):
		constraint<2>({ p1->y() - p2->y() }, { p1->y(), p2->y() })
	{
		// Equation: p1->y() - p2->y()

		// Points are horizontaly aligned if their y coordinates are equal ; ie. y1 - y2 = 0
	}

	static std::shared_ptr<pointPoint_horizontality> make(sketchPoint_ptr p1, sketchPoint_ptr p2)
	{
		return std::shared_ptr<pointPoint_horizontality>(new pointPoint_horizontality(p1, p2));
	}
	static std::shared_ptr<pointPoint_horizontality> make(sketchLine_ptr l)
	{
		return std::shared_ptr<pointPoint_horizontality>(new pointPoint_horizontality(l->A(), l->B()));
	}

	virtual std::string name() { return "pointPoint_horizontality"; }
};
class pointPoint_verticality : public constraint<2> {
public:
	pointPoint_verticality(sketchPoint_ptr p1, sketchPoint_ptr p2):
		constraint<2>({ p1->x() - p2->x() }, { p1->x(), p2->x() })
	{
		// Equation: p1->x() - p2->x()

		// Points are verticaly aligned if their x coordinates are equal ; ie. x1 - x2 = 0
	}

	static std::shared_ptr<pointPoint_verticality> make(sketchPoint_ptr p1, sketchPoint_ptr p2)
	{
		return std::shared_ptr<pointPoint_verticality>(new pointPoint_verticality(p1, p2));
	}
	static std::shared_ptr<pointPoint_verticality> make(sketchLine_ptr l)
	{
		return std::shared_ptr<pointPoint_verticality>(new pointPoint_verticality(l->A(), l->B()));
	}

	virtual std::string name() { return "pointPoint_verticality"; }
};

class pointPoint_distance : public constraint<4> {
public:
	pointPoint_distance(sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr d):
		constraint<4>({ pow(p1->x() - p2->x(), 2.0) + pow(p1->y() - p2->y(), 2.0) - pow(d, 2.0) }, { p1->x(), p1->y(), p2->x(), p2->y() })
	{
		// Equation: pow(p1->x() - p2->x(), 2.0) + pow(p1->y() - p2->y(), 2.0) - pow(d, 2.0)

		// The square distance between two points is pow(p1->x() - p2->x(), 2.0) + pow(p1->y() - p2->y(), 2.0)
		// It must be equal to the square of d
		// TODO: check if squaring d truly is more efficient than taking the square root of the actual distance
	}

	static std::shared_ptr<pointPoint_distance> make(sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr d)
	{
		return std::shared_ptr<pointPoint_distance>(new pointPoint_distance(p1, p2, d));
	}
	static std::shared_ptr<pointPoint_distance> make(sketchLine_ptr l, expression_ptr d)
	{
		return std::shared_ptr<pointPoint_distance>(new pointPoint_distance(l->A(), l->B(), d));
	}

	virtual std::string name() { return "pointPoint_distance"; }
};

class pointPoint_horizontalDistance : public constraint<2> {
public:
	pointPoint_horizontalDistance(sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr d):
		constraint<2>({ abs(p1->x() - p2->x()) - d }, { p1->x(), p2->x() })
	{
		// Equation: abs(p1->x() - p2->x()) - d

		// The horizontal distance between two points is the absolute value of the difference of their x coordinate
		// This difference must be equal to d			
	}

	virtual std::string name() { return "pointPoint_horizontalDistance"; }
};

class pointPoint_verticalDistance : public constraint<2> {
public:
	pointPoint_verticalDistance(sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr d):
		constraint<2>({ abs(p1->y() - p2->y()) - d }, { p1->y(), p2->y() })
	{
		// Equation: abs(p1->y() - p2->y()) - d

		// The vertical distance between two points is the absolute value of the difference of their y coordinate
		// This difference must be equal to d		
	}

	virtual std::string name() { return "pointPoint_verticalDistance"; }
};

class pointLine_distance : public constraint<6> {
public:
	pointLine_distance(sketchPoint_ptr p, sketchLine_ptr l, expression_ptr d):
		constraint<6>({ abs((l->B()->x()-l->A()->x())*(p->y() - l->A()->y()) - (l->B()->y()-l->A()->y())*(p->x() - l->A()->x())) / sqrt(l->length2()) - d },
						 { p->x(), p->y(), l->A()->x(), l->A()->y(), l->B()->x(), l->B()->y()})
	{
		// Equation: abs((l->B()->x() - l->A()->x())*(l->B()->y() - l->A()->y()) - (p->x() - l->A()->x())*(p->y() - l->A()->y())) / sqrt(pow(l->B()->x()-l->A()->x(), 2.0) + pow(l->B()->y()-l->A()->y(), 2.0)) - d

		// Area of the parallelogram defined by the three points = abs((l->B()->x() - l->A()->x())*(l->B()->y() - l->A()->y()) - (p->x() - l->A()->x())*(p->y() - l->A()->y()))
		// Base of the parallelogram = sqrt(pow(l->B()->x()-l->A()->x(), 2.0) + pow(l->B()->y()-l->A()->y(), 2.0))
		// Area / Base = height
		// Height must equal d

		// From https://math.stackexchange->center()om/questions/2757318/distance-between-a-point-and-a-line-defined-by-2-points
	}
	
	static std::shared_ptr<pointLine_distance> make(sketchPoint_ptr p, sketchLine_ptr l, expression_ptr d)
	{
		return std::shared_ptr<pointLine_distance>(new pointLine_distance(p, l, d));
	}
	static std::shared_ptr<pointLine_distance> make(sketchCircle_ptr c, sketchLine_ptr l)
	{
		return std::shared_ptr<pointLine_distance>(new pointLine_distance(c->center(), l, c->radius()));
	}
	static std::shared_ptr<pointLine_distance> make(sketchCircle_ptr c, sketchLine_ptr l, expression_ptr d)
	{
		return std::shared_ptr<pointLine_distance>(new pointLine_distance(c->center(), l, c->radius() + d));
	}

	virtual std::string name() { return "pointLine_distance"; }
};

class pointCircle_distance : public constraint<5> {
public:
	pointCircle_distance(sketchPoint_ptr p, sketchCircle_ptr c, expression_ptr d):
		constraint<5>({ pow(p->x() - c->center()->x(), 2.0) + pow(p->y() - c->center()->y(), 2.0) - pow(c->radius() + d, 2.0) }, { p->x(), p->y(), c->center()->x(), c->center()->y(), c->radius() })
	{

	}
	static std::shared_ptr<pointCircle_distance> make(sketchPoint_ptr p, sketchCircle_ptr c)
	{
		return std::shared_ptr<pointCircle_distance>(new pointCircle_distance(p, c, expConst::zero));
	}
	static std::shared_ptr<pointCircle_distance> make(sketchPoint_ptr p, sketchCircle_ptr c, expression_ptr d)
	{
		return std::shared_ptr<pointCircle_distance>(new pointCircle_distance(p, c, d));
	}

	virtual std::string name() { return "pointCircle_distance"; }
};

class lineCircle_distance : public constraint<7> {
public:
	lineCircle_distance(sketchLine_ptr l, sketchCircle_ptr c, expression_ptr d):
		constraint<7>({ abs((l->B()->x()-l->A()->x())*(c->center()->y() - l->A()->y()) - (l->B()->y()-l->A()->y())*(c->center()->x() - l->A()->x())) / sqrt(l->length2()) - (c->radius() + d) },
						 { c->center()->x(), c->center()->y(), l->A()->x(), l->A()->y(), l->B()->x(), l->B()->y(), c->radius()})
	{

	}
	static std::shared_ptr<lineCircle_distance> make(sketchLine_ptr l, sketchCircle_ptr c)
	{
		return std::shared_ptr<lineCircle_distance>(new lineCircle_distance(l, c, expConst::zero));
	}
	static std::shared_ptr<lineCircle_distance> make(sketchLine_ptr l, sketchCircle_ptr c, expression_ptr d)
	{
		return std::shared_ptr<lineCircle_distance>(new lineCircle_distance(l, c, d));
	}

	virtual std::string name() { return "lineCircle_distance"; }
};

class lineLine_angle : public constraint<8> {
public:
	lineLine_angle(sketchLine_ptr l1, sketchLine_ptr l2, expression_ptr a):
		constraint<8>({ },
		 { l1->A()->x(), l1->A()->y(), l1->B()->x(), l1->B()->y(), l2->A()->x(), l2->A()->y(), l2->B()->x(), l2->B()->y() })

	{
		expression_ptr x1 = (l1->A()->x()-l1->B()->x());
		expression_ptr y1 = (l1->A()->y()-l1->B()->y());
		expression_ptr x2 = (l2->A()->x()-l2->B()->x());
		expression_ptr y2 = (l2->A()->y()-l2->B()->y());

		mEqu = mod(acos(dot(x1, y1, x2, y2) / sqrt(l1->length2() * l2->length2())), M_PI) - a;

		// From https://www.mathworks->center()om/matlabcentral/answers/180131-how-can-i-find-the-angle-between-two-vectors-including-directional-information

		// TODO: handle cases where a is not in range, and all
	}

	static std::shared_ptr<lineLine_angle> make(sketchLine_ptr l1, sketchLine_ptr l2, expression_ptr a)
	{
		return std::shared_ptr<lineLine_angle>(new lineLine_angle(l1, l2, a));
	}
	static std::shared_ptr<lineLine_angle> make_parallel(sketchLine_ptr l1, sketchLine_ptr l2)
	{
		return std::shared_ptr<lineLine_angle>(new lineLine_angle(l1, l2, expConst::zero));
	}
	static std::shared_ptr<lineLine_angle> make_perpendicular(sketchLine_ptr l1, sketchLine_ptr l2)
	{
		return std::shared_ptr<lineLine_angle>(new lineLine_angle(l1, l2, expConst::pi2));
	}

	virtual std::string name() { return "lineLine_angle"; }
};

class lineLine_equal : public constraint<8> {
public:
	lineLine_equal(sketchLine_ptr l1, sketchLine_ptr l2):
		constraint<8>(	{ l1->length2() - l2->length2() },
							{ l1->A()->x(), l1->A()->y(), l1->B()->x(), l1->B()->x(), l2->A()->x(), l2->A()->y(), l2->B()->x(), l2->B()->y() })
	{

	}
	virtual std::string name() { return "lineLine_equal"; }
};

class lineCircle_equal : public constraint<5> {
public:
	lineCircle_equal(sketchLine_ptr l, sketchCircle_ptr c):
		constraint<5>(	{ l->length2() - pow(c->radius()*2.0, 2.0) },
							{ l->A()->x(), l->A()->y(), l->B()->x(), l->B()->x(), c->radius() })
	{

	}
	virtual std::string name() { return "lineCircle_equal"; }
};

#endif
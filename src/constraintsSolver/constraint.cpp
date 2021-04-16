
#include "constraint.hpp"

std::shared_ptr<pointPoint_horizontality> pointPoint_horizontality::make(sketchPoint_ptr p1, sketchPoint_ptr p2)
{
	std::shared_ptr<pointPoint_horizontality> constr(new pointPoint_horizontality(p1, p2));
	auto annot1 = std::make_shared<constraintAnnotation>(p1->basePlane(), constr);
	auto annot2 = std::make_shared<constraintAnnotation>(p1->basePlane(), constr);
	annot1->set_twin(annot2);
	annot2->set_twin(annot1);
	p1->add_constraintAnnotation(annot1);
	p2->add_constraintAnnotation(annot2);
	return constr;
}
std::shared_ptr<pointPoint_horizontality> pointPoint_horizontality::make(sketchLine_ptr l)
{
	std::shared_ptr<pointPoint_horizontality> constr(new pointPoint_horizontality(l->A(), l->B()));
	auto annot = std::make_shared<constraintAnnotation>(l->basePlane(), constr);
	l->add_constraintAnnotation(annot);
	return constr;
}

pointPoint_horizontality::pointPoint_horizontality(sketchPoint_ptr p1, sketchPoint_ptr p2):
	constraint<2>({ p1->y() - p2->y() }, { p1->y(), p2->y() }, HORIZONTALITY)
{
	// Equation: p1->y() - p2->y()

	// Points are horizontaly aligned if their y coordinates are equal ; ie. y1 - y2 = 0
}


std::shared_ptr<pointPoint_verticality> pointPoint_verticality::make(sketchPoint_ptr p1, sketchPoint_ptr p2)
{
	std::shared_ptr<pointPoint_verticality> constr(new pointPoint_verticality(p1, p2));
	auto annot1 = std::make_shared<constraintAnnotation>(p1->basePlane(), constr);
	auto annot2 = std::make_shared<constraintAnnotation>(p1->basePlane(), constr);
	p1->add_constraintAnnotation(annot1);
	p2->add_constraintAnnotation(annot2);
	return constr;
}
std::shared_ptr<pointPoint_verticality> pointPoint_verticality::make(sketchLine_ptr l)
{
	std::shared_ptr<pointPoint_verticality> constr(new pointPoint_verticality(l->A(), l->B()));
	auto annot = std::make_shared<constraintAnnotation>(l->basePlane(), constr);
	l->add_constraintAnnotation(annot);
	return constr;	
}

pointPoint_verticality::pointPoint_verticality(sketchPoint_ptr p1, sketchPoint_ptr p2):
	constraint<2>({ p1->x() - p2->x() }, { p1->x(), p2->x() }, VERTICALITY)
{
	// Equation: p1->x() - p2->x()

	// Points are verticaly aligned if their x coordinates are equal ; ie. x1 - x2 = 0
}


std::shared_ptr<pointPoint_distance> pointPoint_distance::make(sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr d)
{
	std::shared_ptr<pointPoint_distance> constr(new pointPoint_distance(p1, p2, d));
	return constr;
}
std::shared_ptr<pointPoint_distance> pointPoint_distance::make_coincident(sketchPoint_ptr p1, sketchPoint_ptr p2)
{
	std::shared_ptr<pointPoint_distance> constr(new pointPoint_distance(p1, p2, expConst::zero));
	constr->set_type(COINCIDENCE);
	auto annot1 = std::make_shared<constraintAnnotation>(p1->basePlane(), constr);
	auto annot2 = std::make_shared<constraintAnnotation>(p1->basePlane(), constr);
	// annot1->set_twin(annot2);
	// annot2->set_twin(annot1);
	p1->add_constraintAnnotation(annot1);
	p2->add_constraintAnnotation(annot2);
	return constr;
}
std::shared_ptr<pointPoint_distance> pointPoint_distance::make(sketchLine_ptr l, expression_ptr d)
{
	return std::shared_ptr<pointPoint_distance>(new pointPoint_distance(l->A(), l->B(), d));
}

pointPoint_distance::pointPoint_distance(sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr d):
	constraint<4>({ pow(p1->x() - p2->x(), 2.0) + pow(p1->y() - p2->y(), 2.0) - pow(d, 2.0) }, { p1->x(), p1->y(), p2->x(), p2->y() }, NONE)
{
	// Equation: pow(p1->x() - p2->x(), 2.0) + pow(p1->y() - p2->y(), 2.0) - pow(d, 2.0)

	// The square distance between two points is pow(p1->x() - p2->x(), 2.0) + pow(p1->y() - p2->y(), 2.0)
	// It must be equal to the square of d
	// TODO: check if squaring d truly is more efficient than taking the square root of the actual distance
}


std::shared_ptr<pointPoint_horizontalDistance> pointPoint_horizontalDistance::make(sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr d)
{
	return std::shared_ptr<pointPoint_horizontalDistance>(new pointPoint_horizontalDistance(p1, p2, d));
}

pointPoint_horizontalDistance::pointPoint_horizontalDistance(sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr d):
	constraint<2>({ abs(p1->x() - p2->x()) - d }, { p1->x(), p2->x() }, NONE)
{
	// Equation: abs(p1->x() - p2->x()) - d

	// The horizontal distance between two points is the absolute value of the difference of their x coordinate
	// This difference must be equal to d
}

std::shared_ptr<pointPoint_verticalDistance> pointPoint_verticalDistance::make(sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr d)
{
	return std::shared_ptr<pointPoint_verticalDistance>(new pointPoint_verticalDistance(p1, p2, d));
}

pointPoint_verticalDistance::pointPoint_verticalDistance(sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr d):
	constraint<2>({ abs(p1->y() - p2->y()) - d }, { p1->y(), p2->y() }, NONE)
{
	// Equation: abs(p1->y() - p2->y()) - d

	// The vertical distance between two points is the absolute value of the difference of their y coordinate
	// This difference must be equal to d		
}

std::shared_ptr<pointLine_distance> pointLine_distance::make(sketchPoint_ptr p, sketchLine_ptr l, expression_ptr d)
{
	return std::shared_ptr<pointLine_distance>(new pointLine_distance(p, l, d));
}
std::shared_ptr<pointLine_distance> pointLine_distance::make(sketchCircle_ptr c, sketchLine_ptr l)
{
	return std::shared_ptr<pointLine_distance>(new pointLine_distance(c->center(), l, c->radius()));
}
std::shared_ptr<pointLine_distance> pointLine_distance::make(sketchCircle_ptr c, sketchLine_ptr l, expression_ptr d)
{
	return std::shared_ptr<pointLine_distance>(new pointLine_distance(c->center(), l, c->radius() + d));
}

pointLine_distance::pointLine_distance(sketchPoint_ptr p, sketchLine_ptr l, expression_ptr d):
	constraint<6>({ abs((l->B()->x()-l->A()->x())*(p->y() - l->A()->y()) - (l->B()->y()-l->A()->y())*(p->x() - l->A()->x())) / sqrt(l->length2()) - d },
						{ p->x(), p->y(), l->A()->x(), l->A()->y(), l->B()->x(), l->B()->y()}, NONE)
{
	// Equation: abs((l->B()->x() - l->A()->x())*(l->B()->y() - l->A()->y()) - (p->x() - l->A()->x())*(p->y() - l->A()->y())) / sqrt(pow(l->B()->x()-l->A()->x(), 2.0) + pow(l->B()->y()-l->A()->y(), 2.0)) - d

	// Area of the parallelogram defined by the three points = abs((l->B()->x() - l->A()->x())*(l->B()->y() - l->A()->y()) - (p->x() - l->A()->x())*(p->y() - l->A()->y()))
	// Base of the parallelogram = sqrt(pow(l->B()->x()-l->A()->x(), 2.0) + pow(l->B()->y()-l->A()->y(), 2.0))
	// Area / Base = height
	// Height must equal d

	// From https://math.stackexchange->center()om/questions/2757318/distance-between-a-point-and-a-line-defined-by-2-points
}

std::shared_ptr<pointCircle_distance> pointCircle_distance::make(sketchPoint_ptr p, sketchCircle_ptr c)
{
	return std::shared_ptr<pointCircle_distance>(new pointCircle_distance(p, c, expConst::zero));
}
std::shared_ptr<pointCircle_distance> pointCircle_distance::make(sketchPoint_ptr p, sketchCircle_ptr c, expression_ptr d)
{
	return std::shared_ptr<pointCircle_distance>(new pointCircle_distance(p, c, d));
}

pointCircle_distance::pointCircle_distance(sketchPoint_ptr p, sketchCircle_ptr c, expression_ptr d):
	constraint<5>({ pow(p->x() - c->center()->x(), 2.0) + pow(p->y() - c->center()->y(), 2.0) - pow(c->radius() + d, 2.0) }, 
	{ p->x(), p->y(), c->center()->x(), c->center()->y(), c->radius() }, NONE)
{

}

std::shared_ptr<lineCircle_distance> lineCircle_distance::make(sketchLine_ptr l, sketchCircle_ptr c)
{
	return std::shared_ptr<lineCircle_distance>(new lineCircle_distance(l, c, expConst::zero));
}
std::shared_ptr<lineCircle_distance> lineCircle_distance::make(sketchLine_ptr l, sketchCircle_ptr c, expression_ptr d)
{
	return std::shared_ptr<lineCircle_distance>(new lineCircle_distance(l, c, d));
}

lineCircle_distance::lineCircle_distance(sketchLine_ptr l, sketchCircle_ptr c, expression_ptr d):
	constraint<7>({ abs((l->B()->x()-l->A()->x())*(c->center()->y() - l->A()->y()) - (l->B()->y()-l->A()->y())*(c->center()->x() - l->A()->x())) / sqrt(l->length2()) - (c->radius() + d) },
						{ c->center()->x(), c->center()->y(), l->A()->x(), l->A()->y(), l->B()->x(), l->B()->y(), c->radius()}, NONE)
{

}

std::shared_ptr<lineLine_angle> lineLine_angle::make(sketchLine_ptr l1, sketchLine_ptr l2, expression_ptr a)
{
	return std::shared_ptr<lineLine_angle>(new lineLine_angle(l1, l2, a));
}
std::shared_ptr<lineLine_angle> lineLine_angle::make_parallel(sketchLine_ptr l1, sketchLine_ptr l2)
{
	std::shared_ptr<lineLine_angle> constr(new lineLine_angle(l1, l2, expConst::zero));
	constr->mType = PARALLELISM;
	auto annot1 = std::make_shared<constraintAnnotation>(l1->basePlane(), constr);
	auto annot2 = std::make_shared<constraintAnnotation>(l1->basePlane(), constr);
	annot1->set_twin(annot2);
	annot2->set_twin(annot1);
	l1->add_constraintAnnotation(annot1);
	l2->add_constraintAnnotation(annot2);
	return constr;
}
std::shared_ptr<lineLine_angle> lineLine_angle::make_perpendicular(sketchLine_ptr l1, sketchLine_ptr l2)
{
	std::shared_ptr<lineLine_angle> constr(new lineLine_angle(l1, l2, expConst::pi2));
	constr->mType = PERPENDICULARITY;
	auto annot1 = std::make_shared<constraintAnnotation>(l1->basePlane(), constr);
	auto annot2 = std::make_shared<constraintAnnotation>(l1->basePlane(), constr);
	annot1->set_twin(annot2);
	annot2->set_twin(annot1);
	l1->add_constraintAnnotation(annot1);
	l2->add_constraintAnnotation(annot2);
	return constr;
}

lineLine_angle::lineLine_angle(sketchLine_ptr l1, sketchLine_ptr l2, expression_ptr a):
	constraint<8>({ },
		{ l1->A()->x(), l1->A()->y(), l1->B()->x(), l1->B()->y(), l2->A()->x(), l2->A()->y(), l2->B()->x(), l2->B()->y() }, NONE)
{
	expression_ptr x1 = (l1->A()->x()-l1->B()->x());
	expression_ptr y1 = (l1->A()->y()-l1->B()->y());
	expression_ptr x2 = (l2->A()->x()-l2->B()->x());
	expression_ptr y2 = (l2->A()->y()-l2->B()->y());

	mEqu = mod(acos(dot(x1, y1, x2, y2) / (sqrt(x1*x1+y1*y1) * sqrt(x2*x2+y2*y2))), 2.0*M_PI) - mod(a, 2.0*M_PI);
	// https://www.omnicalculator.com/math/angle-between-two-vectors
}

std::shared_ptr<lineLine_equal> lineLine_equal::make(sketchLine_ptr l1, sketchLine_ptr l2)
{
	return std::shared_ptr<lineLine_equal>(new lineLine_equal(l1, l2));
}

lineLine_equal::lineLine_equal(sketchLine_ptr l1, sketchLine_ptr l2):
	constraint<8>(	{ l1->length2() - l2->length2() },
						{ l1->A()->x(), l1->A()->y(), l1->B()->x(), l1->B()->x(), l2->A()->x(), l2->A()->y(), l2->B()->x(), l2->B()->y() }, NONE)
{

}

std::shared_ptr<lineCircle_equal> lineCircle_equal::make(sketchLine_ptr l, sketchCircle_ptr c)
{
	return std::shared_ptr<lineCircle_equal>(new lineCircle_equal(l, c));
}

lineCircle_equal::lineCircle_equal(sketchLine_ptr l, sketchCircle_ptr c):
	constraint<5>(	{ l->length2() - pow(c->radius()*2.0, 2.0) },
						{ l->A()->x(), l->A()->y(), l->B()->x(), l->B()->x(), c->radius() }, NONE)
{

}

#ifndef CONSTRAINT_ABSTR_HPP_
#define CONSTRAINT_ABSTR_HPP_

#include "Expression_forward.hpp"
#include <BaseObject.hpp>

#include <Geometry/2d/Point_abstr.hpp>
#include <Geometry/2d/Line_abstr.hpp>
#include <Geometry/2d/Circle_abstr.hpp>

#include <array>

#include <Utils/Param.hpp>

#define CONSTR_SATISFACTION_TRESHOLD 1e-12

/*
	@class Constraint_abstr represents a constraint that is a collection of at least one
	equation

	@parent BaseObject
*/
class Constraint_abstr : virtual public BaseObject {
private:
#ifndef RELEASE_MODE // The name of the variable if not planned to be revealed to the user
	std::string mName;
#endif
	bool mExists;
	bool mCanSubstitute;
	double mLastError;
public:
#ifndef RELEASE_MODE
	Constraint_abstr(std::string name_, bool can_substitute = false);
#else
	Constraint_abstr(bool can_substitute = false);
#endif
	virtual ~Constraint_abstr();

	bool exists() const { return mExists; }
	void set_exists(bool ex) { mExists = ex; } // TODO: Check if it should iterate over all variable to set their existence status

	/*
		@function error gives an indexed error

		@oaram ind The index of the equation from which to pull the error
		@return The error at this index
	*/
	virtual double error() = 0;

	inline virtual double stepScale(double lastError) { return 1.0f; }

	/*
		@function satisfied

		@return If all the equations in the constraint have an error below a (currently arbitrary) treshold
	*/
	virtual bool satisfied();
	/*
		@function derivative gives a partial derivative for a variable for an equation

		@param withRespectTo 				The variable that the equation must be partially derived for
		@param from_eq  [defaults to 0]		The index of the equation that must be derived
		@note @param from_eq defaults to 0 because most constraint have 1 equation
	*/
	virtual double derivative(Param* withRespectTo) = 0;
	/*
		@function d Is a wrapper for @function derivative
	*/
	inline double d(Param* withRespectTo) { return derivative(withRespectTo); }

	virtual bool is_linePoints() { return false; } // If all constraints in a cluster act on lines and points only

	inline bool substitutionConstraint() { return mCanSubstitute; }
	virtual void substitute() { }

	/*
		@function var is a getter to access variables of the constraint

		@param ind Is the index of the wanted variable
	*/
	virtual Param* param(int ind) = 0;
	/*
		@function n_equs

		@return The number of equations contained (most times it will be 0)		
	*/
	virtual int n_params() = 0;

#ifndef RELEASE_MODE
	/*
		@function name

		@return The name of the constraint
		@note This is for debug
	*/
	std::string name() { return mName; }
#endif
};

/* ---------- Horizontality ---------- */
class PointPoint_horizontality : public Constraint_abstr {
private:
	Geom2d::Point* mP1, *mP2; // handles to the geometries involved, not used yet but might get handy
	int mSubstitute_state;
public:
	PointPoint_horizontality(Geom2d::Point* p1, Geom2d::Point* p2);

	double error();

	double derivative(Param* withRespectTo) { return 0.0; }

	void substitute();

	Param* param(int ind);
	inline int n_params() { return 2; }

};
class Line_horizontality : public Constraint_abstr {
private:
	Geom2d::Line* mLine; // Handles to the geometry involved, not used yet but might get handy
public:
	Line_horizontality(Geom2d::Line* l);

	double error();

	double derivative(Param* withRespectTo) { return 0.0; }

	void substitute();

	Param* param(int ind);
	inline int n_params() { return 2; }
};

// /* ---------- Verticality ---------- */
// class SkPointPoint_verticality : public SkSprite_constraint<2> {
// private:
// 	SkPoint* mP1, *mP2; // Handles to the geometries involved, not used yet but might get handy
// public:
// 	SkPointPoint_verticality(Geom3d::Plane_abstr* baseplane_, SkPoint* p1, SkPoint* p2);
// 	std::string name() { return "SkPointPoint_verticality"; }
// };
// class SkLine_verticality : public SkSprite_constraint<1> {
// private:
// 	SkLineCurve* mLine; // Handle to the geometry involved, not used yet but might get handy
// public:
// 	SkLine_verticality(Geom3d::Plane_abstr* baseplane_, SkLineCurve* l);
// 	std::string name() { return "SkLine_verticality"; }
// };

// /* ---------- Distance --------- */
// class SkLine_length : public SkConstraint {
// private:
// 	SkLineCurve* mLine; // Handle to the geometry involved, not used yet but might get handy
// 	exp_ptr mLength; // Handle to the length value, migue be useful
// public:
// 	SkLine_length(Geom3d::Plane_abstr* baseplane_, SkLineCurve* l, exp_ptr length);
// };
// class SkPointPoint_horizontalDistance : public SkConstraint {
// private:
// 	SkPoint* mP1, *mP2; // Handles to the geometries involved, not used yet but might get handy
// 	exp_ptr mDistance; // Handle to the length value, migue be useful
// public:
// 	SkPointPoint_horizontalDistance(Geom3d::Plane_abstr* baseplane_, 
// 	SkPoint* p1, SkPoint* p2, exp_ptr dist);
// 	std::string name() { return "SkPointPoint_horizontalDistance"; }
// };
// class SkPointPoint_verticalDistance : public SkConstraint {
// private:
// 	SkPoint* mP1, *mP2; // Handles to the geometries involved, not used yet but might get handy
// 	exp_ptr mDistance; // Handle to the length value, migue be useful
// public:
// 	SkPointPoint_verticalDistance(Geom3d::Plane_abstr* baseplane_, 
// 	SkPoint* p1, SkPoint* p2, exp_ptr dist);
// 	std::string name() { return "SkPointPoint_verticalDistance"; }
// };
// // class SkPointLine_distance_abstr : public SkConstraint {
// // public:
// // 	SkPointLine_distance_abstr(Geom3d::Plane_abstr* baseplane_, SkPoint* p, SkLineCurve* l, exp_ptr dist);
// // };
// // class SkPointLine_distance : public SkPointLine_distance_abstr {
// // private:
// // 	SkPoint* mPoint;
// // 	SkLineCurve* mLine;
// // 	exp_ptr mDistance;
// // public:
// // 	SkPointLine_distance(Geom3d::Plane_abstr* baseplane_, SkPoint* p, SkLineCurve* l, exp_ptr dist);
// // 	std::string name() { return "SkPointLine_distance"; }
// // };

// // class SkCircleLine_distance : public SkPointLine_distance_abstr {
// // private:
// // 	sketchCircle_ptr mCircle;
// // 	SkLineCurve* mLine;
// // 	exp_ptr mDistance;
// // public:
// // 	SkCircleLine_distance(Geom3d::Plane_abstr* baseplane_, sketchCircle_ptr c, SkLineCurve* l, exp_ptr dist);
// // 	std::string name() { return "SkCircleLine_distance"; }
// // };

// // /* ---------- Angle ---------- */
// class SkLineLinePerpendicularity : public SkSprite_constraint<2> {
// private:
// 	SkLineCurve* mLine1, *mLine2;
// public:
// 	SkLineLinePerpendicularity(Geom3d::Plane_abstr* baseplane_, SkLineCurve* l1, SkLineCurve* l2);
// 	std::string name() { return "SkLineLinePerpendicularity"; }
// };

// // /* ---------- Coincidence ---------- */
// class SkPointPoint_coincidence : public SkSprite_constraint<1> {
// private:
// 	SkPoint* mP1, *mP2;
// public:
// 	SkPointPoint_coincidence(Geom3d::Plane_abstr* baseplane_, SkPoint* p1, SkPoint* p2);
// 	std::string name() { return "SkPointPoint_coincidence"; }
// };
// class SkPointLine_coincidence : public SkSprite_constraint<1> {
// private:
// 	SkPoint* mPoint;
// 	SkLineCurve* mLine;
// public:
// 	SkPointLine_coincidence(Geom3d::Plane_abstr* baseplane_, SkPoint* p, SkLineCurve* l);
// 	std::string name() { return "SkPointLine_coincidence"; }
// };
// class SkPointCircle_coincidence : public SkSprite_constraint<1> {
// private:
// 	SkPoint* mPoint;
// 	SkCircleCurve* mCircle;
// public:
// 	SkPointCircle_coincidence(Geom3d::Plane_abstr* baseplane_, SkPoint* p, SkCircleCurve* c);
// 	std::string name() { return "SkPointCircle_coincidence"; }
// };

// class circleLine_coincidence : public SkSprite_constraint<2> {
// private:
// 	sketchCircle_ptr mCircle;
// 	SkLineCurve* mLine;
// 	exp_ptr mDistance;
// public:
// 	circleLine_coincidence(Geom3d::Plane_abstr* baseplane_, sketchCircle_ptr c, SkLineCurve* l, exp_ptr dist):
// 		SkSprite_constraint<2>({ c->center()->x(), c->center()->y(), l->A()->x(), l->A()->y(), l->B()->x(), l->B()->y()},
// 		{ abs((l->B()->x()-l->A()->x())*(c->center()->y() - l->A()->y()) - (l->B()->y()-l->A()->y())*(c->center()->x() - 
// 		l->A()->x())) / sqrt(l->length2()) - c->radius() }, 
// 		baseplane_)
// 	{
// 		c->add_constraint()
// 	}
// };

// std::shared_ptr<lineLine_equal> lineLine_equal::make(SkLineCurve* l1, SkLineCurve* l2)
// {
// 	return std::shared_ptr<lineLine_equal>(new lineLine_equal(l1, l2));
// }

// lineLine_equal::lineLine_equal(SkLineCurve* l1, SkLineCurve* l2):
// 	constraint<8>({ l1->A()->x(), l1->A()->y(), l1->B()->x(), l1->B()->x(), l2->A()->x(), l2->A()->y(), l2->B()->x(), l2->B()->y() },
// 	{ l1->length2() - l2->length2() }, NONE)
// {

// }

// std::shared_ptr<lineCircle_equal> lineCircle_equal::make(SkLineCurve* l, sketchCircle_ptr c)
// {
// 	return std::shared_ptr<lineCircle_equal>(new lineCircle_equal(l, c));
// }

// lineCircle_equal::lineCircle_equal(SkLineCurve* l, sketchCircle_ptr c):
// 	constraint<5>({ l->A()->x(), l->A()->y(), l->B()->x(), l->B()->x(), c->radius() },
// 	{ l->length2() - pow(c->radius()*2.0, 2.0) }, NONE)
// {

// }


#endif
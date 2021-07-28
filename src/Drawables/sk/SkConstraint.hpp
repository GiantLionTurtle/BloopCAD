
#ifndef SKCONSTRAINT_HPP_
#define SKCONSTRAINT_HPP_

#include "SkDrawable.hpp"
#include "SkSprite.hpp"

#include <constraintsSolver/constraint.hpp>
#include <constraintsSolver/expression.hpp>
#include <utils/DebugUtils.hpp>

#include <array>
#include <vector>

class SkPoint;
class SkLineCurve;
// class SkCircle;

class SkConstraint : virtual public SkDrawable, public constraint_abstr {
protected:
	std::vector<equ_ptr> mEqus;
	std::vector<var_ptr> mVars;
public:
	SkConstraint(std::vector<var_ptr> vars, std::vector<equ_ptr> equs);

	double error(size_t eq);
	bool satisfied();
	double derivative(var_ptr withRespectTo, size_t from_eq);

	equ_ptr equ(size_t ind) { return mEqus.at(ind); }
	/*constexpr*/ size_t n_equs() { return mEqus.size(); }

	var_ptr var(size_t ind) { return mVars.at(ind); }
	/*constexpr*/ size_t n_vars() { return mVars.size(); }

	void exists_impl(bool ex);
};

template<size_t nA>
class sprite_constraint : public SkConstraint, public Linked_Collection<std::array<SkSprite*, nA>> {
public:
	using pT = Linked_Collection<std::array<SkSprite*, nA>>;

	sprite_constraint(std::vector<var_ptr> vars, std::vector<equ_ptr> equs,
		geom_3d::plane_abstr_ptr baseplane_, std::string const& spritePath):
		SkConstraint(vars, equs)
	{
		for(int i = 0; i < pT::mDrawList.size(); ++i) {
			pT::mDrawList[i] = new SkSprite(baseplane_, glm::vec2(40, 40), spritePath);
			pT::mDrawList[i]->set_parent(this);
		}
	}
	void notify(Drawable* who, int msg, bool child)
	{
		if(!child)
			return;

		switch(msg) {
		case RESURRECTED:
			set_exists(true);
			break;
		case DELETED:
			set_exists(false);
			break;
		case SELECTED:
			set_selected(true);
			break;
		case UNSELECTED:
			set_selected(false);
			break;
		}
	}

	SelectionPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
	{
		for(int i = 0; i < nA; ++i) {
			SelectionPoint selPt = pT::mDrawList.at(i)->closest_2d(planePos, cam, cursorPos, filter);
			if(selPt.ent)
				return selPt;
		}
	}
	DraggableSelectionPoint closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
	{
		for(int i = 0; i < nA; ++i) {
			DraggableSelectionPoint selPt = pT::mDrawList.at(i)->closest_2d_draggable(planePos, cam, cursorPos, filter);
			if(selPt.ent)
				return selPt;
		}
	}
protected:
	virtual void select_impl(bool sel)
	{
		// True as second argument to avoid getting notified and causing recursion
		for(auto annot : pT::mDrawList)
			annot->set_selected(sel, true);
	}
	virtual void hover_impl(bool hov)
	{
		// True as second argument to avoid getting notified and causing recursion
		for(auto annot : pT::mDrawList)	
			annot->set_hover(hov, true);
	}
	virtual void hidden_impl(bool hid) 
	{
		// True as second argument to avoid getting notified and causing recursion
		for(auto annot : pT::mDrawList)
			annot->set_hidden(hid, true);
	}
};

/* ---------- Horizontality ---------- */
class pointPoint_horizontality : public sprite_constraint<2> {
private:
	SkPoint* mP1, *mP2; // handles to the geometries involved, not used yet but might get handy
public:
	pointPoint_horizontality(geom_3d::plane_abstr_ptr baseplane_, SkPoint* p1, SkPoint* p2);
	std::string name() { return "pointPoint_horizontality"; }
};
class line_horizontality : public sprite_constraint<1> {
private:
	SkLineCurve* mLine; // Handles to the geometry involved, not used yet but might get handy
public:
	line_horizontality(geom_3d::plane_abstr_ptr baseplane_, SkLineCurve* l);
	std::string name() { return "line_horizontality"; }
};

/* ---------- Verticality ---------- */
class pointPoint_verticality : public sprite_constraint<2> {
private:
	SkPoint* mP1, *mP2; // Handles to the geometries involved, not used yet but might get handy
public:
	pointPoint_verticality(geom_3d::plane_abstr_ptr baseplane_, SkPoint* p1, SkPoint* p2);
	std::string name() { return "pointPoint_verticality"; }
};
class line_verticality : public sprite_constraint<1> {
private:
	SkLineCurve* mLine; // Handle to the geometry involved, not used yet but might get handy
public:
	line_verticality(geom_3d::plane_abstr_ptr baseplane_, SkLineCurve* l);
	std::string name() { return "line_verticality"; }
};

/* ---------- Distance --------- */
class line_length : public SkConstraint {
private:
	SkLineCurve* mLine; // Handle to the geometry involved, not used yet but might get handy
	expression_ptr mLength; // Handle to the length value, migue be useful
public:
	line_length(geom_3d::plane_abstr_ptr baseplane_, SkLineCurve* l, expression_ptr length);
};
class pointPoint_horizontalDistance : public SkConstraint {
private:
	SkPoint* mP1, *mP2; // Handles to the geometries involved, not used yet but might get handy
	expression_ptr mDistance; // Handle to the length value, migue be useful
public:
	pointPoint_horizontalDistance(geom_3d::plane_abstr_ptr baseplane_, 
	SkPoint* p1, SkPoint* p2, expression_ptr dist);
	std::string name() { return "pointPoint_horizontalDistance"; }
};
class pointPoint_verticalDistance : public SkConstraint {
private:
	SkPoint* mP1, *mP2; // Handles to the geometries involved, not used yet but might get handy
	expression_ptr mDistance; // Handle to the length value, migue be useful
public:
	pointPoint_verticalDistance(geom_3d::plane_abstr_ptr baseplane_, 
	SkPoint* p1, SkPoint* p2, expression_ptr dist);
	std::string name() { return "pointPoint_verticalDistance"; }
};
class pointLine_distance_abstr : public SkConstraint {
public:
	pointLine_distance_abstr(geom_3d::plane_abstr_ptr baseplane_, SkPoint* p, SkLineCurve* l, expression_ptr dist);
};
class pointLine_distance : public pointLine_distance_abstr {
private:
	SkPoint* mPoint;
	SkLineCurve* mLine;
	expression_ptr mDistance;
public:
	pointLine_distance(geom_3d::plane_abstr_ptr baseplane_, SkPoint* p, SkLineCurve* l, expression_ptr dist);
	std::string name() { return "pointLine_distance"; }
};
// class circleLine_distance : public pointLine_distance_abstr {
// private:
// 	sketchCircle_ptr mCircle;
// 	SkLineCurve* mLine;
// 	expression_ptr mDistance;
// public:
// 	circleLine_distance(geom_3d::plane_abstr_ptr baseplane_, sketchCircle_ptr c, SkLineCurve* l, expression_ptr dist);
// 	std::string name() { return "circleLine_distance"; }
// };

// /* ---------- Angle ---------- */
class lineLinePerpendicularity : public sprite_constraint<2> {
private:
	SkLineCurve* mLine1, *mLine2;
public:
	lineLinePerpendicularity(geom_3d::plane_abstr_ptr baseplane_, SkLineCurve* l1, SkLineCurve* l2);
	std::string name() { return "lineLinePerpendicularity"; }
};

// /* ---------- Coincidence ---------- */
class pointPoint_coincidence : public sprite_constraint<1> {
private:
	SkPoint* mP1, *mP2;
public:
	pointPoint_coincidence(geom_3d::plane_abstr_ptr baseplane_, SkPoint* p1, SkPoint* p2);
	std::string name() { return "pointPoint_coincidence"; }
};
// class circleLine_coincidence : public sprite_constraint<2> {
// private:
// 	sketchCircle_ptr mCircle;
// 	SkLineCurve* mLine;
// 	expression_ptr mDistance;
// public:
// 	circleLine_coincidence(geom_3d::plane_abstr_ptr baseplane_, sketchCircle_ptr c, SkLineCurve* l, expression_ptr dist):
// 		sprite_constraint<2>({ c->center()->x(), c->center()->y(), l->A()->x(), l->A()->y(), l->B()->x(), l->B()->y()},
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


#ifndef SKCONSTRAINT_HPP_
#define SKCONSTRAINT_HPP_

#include "SkDrawable.hpp"
#include "SkSprite.hpp"
#include <Drawables/Collection.hpp>
#include <ConstraintsSolver/Constraint_abstr.hpp>
#include <ConstraintsSolver/Expression.hpp>
#include <utils/DebugUtils.hpp>

#include <glm/glm.hpp>

#include <array>
#include <vector>

class SkPoint;
class SkLineCurve;
class Sketch;
// class SkCircle;

class SkConstraint : public SkDrawable, public Constraint_abstr {
protected:
	std::vector<equ_ptr> mEqus;
	std::vector<var_ptr> mVars;
public:
	SkConstraint(Geom3d::Plane_abstr* baseplane_, std::vector<var_ptr> vars, std::vector<equ_ptr> equs);

	equ_ptr equ(size_t ind) { return mEqus.at(ind); }
	/*constexpr*/ size_t n_equs() { return mEqus.size(); }

	var_ptr var(size_t ind) { return mVars.at(ind); }
	/*constexpr*/ size_t n_vars() { return mVars.size(); }

	void exists_impl(bool ex);
};

template<size_t nA>
class SkSprite_constraint : public SkConstraint, public Collection_abstr<LinearFixed_indexer<SkSprite*, nA>> {
public:
	using pT = Collection_abstr<LinearFixed_indexer<SkSprite*, nA>>;

	SkSprite_constraint(Geom3d::Plane_abstr* baseplane_, std::vector<var_ptr> vars, std::vector<equ_ptr> equs,
		std::string const& spritePath):
		SkConstraint(baseplane_, vars, equs)
	{
		for(int i = 0; i < nA; ++i) {
			SkSprite* sprite = new SkSprite(baseplane_, glm::vec2(40, 40), spritePath);
			sprite->set_notif_on_exists(true);
			pT::mDrawList.set(i, sprite);
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
		return SelectionPoint();
	}
	std::unique_ptr<DraggableSelectionPoint> closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
	{
		for(int i = 0; i < nA; ++i) {
			std::unique_ptr<DraggableSelectionPoint> selPt = 
				pT::mDrawList.at(i)->closest_2d_draggable(planePos, cam, cursorPos, filter);
			if(selPt)
				return selPt;
		}
		return nullptr;
	}

	SkSprite* annot(size_t ind)
	{
		return pT::mDrawList.at(ind);
	}
protected:
	virtual void select_impl(bool sel)
	{
		// True as second argument to avoid getting notified and causing recursion
		for(int i = 0; i < nA; ++i) {
			pT::mDrawList.at(i)->set_selected(sel, true);
		}
	}
	virtual void hover_impl(bool hov)
	{
		// True as second argument to avoid getting notified and causing recursion
		for(int i = 0; i < nA; ++i) {
			pT::mDrawList.at(i)->set_hover(hov, true);
		}
	}
	virtual void hidden_impl(bool hid) 
	{
		// True as second argument to avoid getting notified and causing recursion
		for(int i = 0; i < nA; ++i) {
			pT::mDrawList.at(i)->set_hidden(hid, true);
		}
	}
};

/* ---------- Horizontality ---------- */
class SkPointPoint_horizontality : public SkSprite_constraint<2> {
private:
	SkPoint* mP1, *mP2; // handles to the geometries involved, not used yet but might get handy
public:
	SkPointPoint_horizontality(Geom3d::Plane_abstr* baseplane_, SkPoint* p1, SkPoint* p2);
	std::string name() { return "SkPointPoint_horizontality"; }
};
class SkLine_horizontality : public SkSprite_constraint<1> {
private:
	SkLineCurve* mLine; // Handles to the geometry involved, not used yet but might get handy
public:
	SkLine_horizontality(Geom3d::Plane_abstr* baseplane_, SkLineCurve* l);
	std::string name() { return "SkLine_horizontality"; }
};

/* ---------- Verticality ---------- */
class SkPointPoint_verticality : public SkSprite_constraint<2> {
private:
	SkPoint* mP1, *mP2; // Handles to the geometries involved, not used yet but might get handy
public:
	SkPointPoint_verticality(Geom3d::Plane_abstr* baseplane_, SkPoint* p1, SkPoint* p2);
	std::string name() { return "SkPointPoint_verticality"; }
};
class SkLine_verticality : public SkSprite_constraint<1> {
private:
	SkLineCurve* mLine; // Handle to the geometry involved, not used yet but might get handy
public:
	SkLine_verticality(Geom3d::Plane_abstr* baseplane_, SkLineCurve* l);
	std::string name() { return "SkLine_verticality"; }
};

/* ---------- Distance --------- */
class SkLine_length : public SkConstraint {
private:
	SkLineCurve* mLine; // Handle to the geometry involved, not used yet but might get handy
	exp_ptr mLength; // Handle to the length value, migue be useful
public:
	SkLine_length(Geom3d::Plane_abstr* baseplane_, SkLineCurve* l, exp_ptr length);
};
class SkPointPoint_horizontalDistance : public SkConstraint {
private:
	SkPoint* mP1, *mP2; // Handles to the geometries involved, not used yet but might get handy
	exp_ptr mDistance; // Handle to the length value, migue be useful
public:
	SkPointPoint_horizontalDistance(Geom3d::Plane_abstr* baseplane_, 
	SkPoint* p1, SkPoint* p2, exp_ptr dist);
	std::string name() { return "SkPointPoint_horizontalDistance"; }
};
class SkPointPoint_verticalDistance : public SkConstraint {
private:
	SkPoint* mP1, *mP2; // Handles to the geometries involved, not used yet but might get handy
	exp_ptr mDistance; // Handle to the length value, migue be useful
public:
	SkPointPoint_verticalDistance(Geom3d::Plane_abstr* baseplane_, 
	SkPoint* p1, SkPoint* p2, exp_ptr dist);
	std::string name() { return "SkPointPoint_verticalDistance"; }
};
class SkPointLine_distance_abstr : public SkConstraint {
public:
	SkPointLine_distance_abstr(Geom3d::Plane_abstr* baseplane_, SkPoint* p, SkLineCurve* l, exp_ptr dist);
};
class SkPointLine_distance : public SkPointLine_distance_abstr {
private:
	SkPoint* mPoint;
	SkLineCurve* mLine;
	exp_ptr mDistance;
public:
	SkPointLine_distance(Geom3d::Plane_abstr* baseplane_, SkPoint* p, SkLineCurve* l, exp_ptr dist);
	std::string name() { return "SkPointLine_distance"; }
};
// class SkCircleLine_distance : public SkPointLine_distance_abstr {
// private:
// 	sketchCircle_ptr mCircle;
// 	SkLineCurve* mLine;
// 	exp_ptr mDistance;
// public:
// 	SkCircleLine_distance(Geom3d::Plane_abstr* baseplane_, sketchCircle_ptr c, SkLineCurve* l, exp_ptr dist);
// 	std::string name() { return "SkCircleLine_distance"; }
// };

// /* ---------- Angle ---------- */
class SkLineLinePerpendicularity : public SkSprite_constraint<2> {
private:
	SkLineCurve* mLine1, *mLine2;
public:
	SkLineLinePerpendicularity(Geom3d::Plane_abstr* baseplane_, SkLineCurve* l1, SkLineCurve* l2);
	std::string name() { return "SkLineLinePerpendicularity"; }
};

// /* ---------- Coincidence ---------- */
class SkPointPoint_coincidence : public SkSprite_constraint<1> {
private:
	SkPoint* mP1, *mP2;
public:
	SkPointPoint_coincidence(Geom3d::Plane_abstr* baseplane_, SkPoint* p1, SkPoint* p2);
	std::string name() { return "SkPointPoint_coincidence"; }
};

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

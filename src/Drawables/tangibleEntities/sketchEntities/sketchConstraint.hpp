
#ifndef SKETCHCONSTRAINT_HPP_
#define SKETCHCONSTRAINT_HPP_

#include <constraintsSolver/expression.hpp>
#include <utils/DebugUtils.hpp>
#include "sketchEntity.hpp"
#include "spriteAnnotation.hpp"
#include <constraintsSolver/constraint.hpp>

#include <array>
#include <vector>
#include <string>

class sketchPoint;
using sketchPoint_ptr = std::shared_ptr<sketchPoint>;
class sketchLine;
using sketchLine_ptr = std::shared_ptr<sketchLine>;
class sketchCircle;
using sketchCircle_ptr = std::shared_ptr<sketchCircle>;

class constraint_entity : public Drawable, public constraint_abstr {
protected:
	std::vector<equ_ptr> mEqus;
	std::vector<var_ptr> mVars;
public:
	constraint_entity(std::vector<var_ptr> vars, std::vector<equ_ptr> equs);

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
class sprite_constraint : public constraint_entity {
protected:
	std::array<std::shared_ptr<spriteAnnotation>, nA> mAnnots;
public:
	sprite_constraint(std::vector<var_ptr> vars, std::vector<equ_ptr> equs,
		geom_3d::plane_abstr_ptr baseplane_, std::string const& spritePath):
		constraint_entity(vars, equs)
	{
		for(int i = 0; i < mAnnots.size(); ++i) {
			mAnnots[i] = std::make_shared<spriteAnnotation>(baseplane_, glm::vec2(40, 40), spritePath);
			mAnnots[i]->set_parent(this);
		}
	}
	virtual void for_each(std::function<void (Drawable_ptr)> func)
	{
		for(auto annot : mAnnots)
			func(annot);
	}

	void notify(int msg)
	{
		switch(msg) {
		case SELECTED:
			select();
			break;
		case UNSELECTED:
			unselect();
			break;
		case HOVERED:
			set_hover(true);
			break;
		case UNHOVERED:
			set_hover(false);
			break;
		case DELETED:
			set_exists(false);
			break;
		case RESURRECTED: 
			set_exists(true);
			break;
		}
	}
protected:
	virtual void select_impl(bool sel)
	{
		// True as second argument to avoid getting notified and causing recursion
		for(auto annot : mAnnots)
			annot->set_selected(sel, true);
	}
	virtual void hover_impl(bool hov)
	{
		// True as second argument to avoid getting notified and causing recursion
		for(auto annot : mAnnots)	
			annot->set_hover(hov, true);
	}
	virtual void hidden_impl(bool hid) 
	{
		// True as second argument to avoid getting notified and causing recursion
		for(auto annot : mAnnots)
			annot->set_hidden(hid, true);
	}
};

/* ---------- Horizontality ---------- */
class pointPoint_horizontality : public sprite_constraint<2> {
private:
	sketchPoint_ptr mP1, mP2; // handles to the geometries involved, not used yet but might get handy
public:
	pointPoint_horizontality(geom_3d::plane_abstr_ptr baseplane_, sketchPoint_ptr p1, sketchPoint_ptr p2);
	std::string name() { return "pointPoint_horizontality"; }
};
class line_horizontality : public sprite_constraint<1> {
private:
	sketchLine_ptr mLine; // Handles to the geometry involved, not used yet but might get handy
public:
	line_horizontality(geom_3d::plane_abstr_ptr baseplane_, sketchLine_ptr l);
	std::string name() { return "line_horizontality"; }
};

/* ---------- Verticality ---------- */
class pointPoint_verticality : public sprite_constraint<2> {
private:
	sketchPoint_ptr mP1, mP2; // Handles to the geometries involved, not used yet but might get handy
public:
	pointPoint_verticality(geom_3d::plane_abstr_ptr baseplane_, sketchPoint_ptr p1, sketchPoint_ptr p2);
	std::string name() { return "pointPoint_verticality"; }
};
class line_verticality : public sprite_constraint<1> {
private:
	sketchLine_ptr mLine; // Handle to the geometry involved, not used yet but might get handy
public:
	line_verticality(geom_3d::plane_abstr_ptr baseplane_, sketchLine_ptr l);
	std::string name() { return "line_verticality"; }
};

/* ---------- Distance --------- */
class line_length : public constraint_entity {
private:
	sketchLine_ptr mLine; // Handle to the geometry involved, not used yet but might get handy
	expression_ptr mLength; // Handle to the length value, migue be useful
public:
	line_length(geom_3d::plane_abstr_ptr baseplane_, sketchLine_ptr l, expression_ptr length);
};
class pointPoint_horizontalDistance : public constraint_entity {
private:
	sketchPoint_ptr mP1, mP2; // Handles to the geometries involved, not used yet but might get handy
	expression_ptr mDistance; // Handle to the length value, migue be useful
public:
	pointPoint_horizontalDistance(geom_3d::plane_abstr_ptr baseplane_, 
	sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr dist);
	std::string name() { return "pointPoint_horizontalDistance"; }
};
class pointPoint_verticalDistance : public constraint_entity {
private:
	sketchPoint_ptr mP1, mP2; // Handles to the geometries involved, not used yet but might get handy
	expression_ptr mDistance; // Handle to the length value, migue be useful
public:
	pointPoint_verticalDistance(geom_3d::plane_abstr_ptr baseplane_, 
	sketchPoint_ptr p1, sketchPoint_ptr p2, expression_ptr dist);
	std::string name() { return "pointPoint_verticalDistance"; }
};
class pointLine_distance_abstr : public constraint_entity {
public:
	pointLine_distance_abstr(geom_3d::plane_abstr_ptr baseplane_, sketchPoint_ptr p, sketchLine_ptr l, expression_ptr dist);
};
class pointLine_distance : public pointLine_distance_abstr {
private:
	sketchPoint_ptr mPoint;
	sketchLine_ptr mLine;
	expression_ptr mDistance;
public:
	pointLine_distance(geom_3d::plane_abstr_ptr baseplane_, sketchPoint_ptr p, sketchLine_ptr l, expression_ptr dist);
	std::string name() { return "pointLine_distance"; }
};
class circleLine_distance : public pointLine_distance_abstr {
private:
	sketchCircle_ptr mCircle;
	sketchLine_ptr mLine;
	expression_ptr mDistance;
public:
	circleLine_distance(geom_3d::plane_abstr_ptr baseplane_, sketchCircle_ptr c, sketchLine_ptr l, expression_ptr dist);
	std::string name() { return "circleLine_distance"; }
};

// /* ---------- Angle ---------- */
class lineLinePerpendicularity : public sprite_constraint<2> {
private:
	sketchLine_ptr mLine1, mLine2;
public:
	lineLinePerpendicularity(geom_3d::plane_abstr_ptr baseplane_, sketchLine_ptr l1, sketchLine_ptr l2);
	std::string name() { return "lineLinePerpendicularity"; }
};

// /* ---------- Coincidence ---------- */
class pointPoint_coincidence : public sprite_constraint<1> {
private:
	sketchPoint_ptr mP1, mP2;
public:
	pointPoint_coincidence(geom_3d::plane_abstr_ptr baseplane_, sketchPoint_ptr p1, sketchPoint_ptr p2);
	std::string name() { return "pointPoint_coincidence"; }
};
// class circleLine_coincidence : public sprite_constraint<2> {
// private:
// 	sketchCircle_ptr mCircle;
// 	sketchLine_ptr mLine;
// 	expression_ptr mDistance;
// public:
// 	circleLine_coincidence(geom_3d::plane_abstr_ptr baseplane_, sketchCircle_ptr c, sketchLine_ptr l, expression_ptr dist):
// 		sprite_constraint<2>({ c->center()->x(), c->center()->y(), l->A()->x(), l->A()->y(), l->B()->x(), l->B()->y()},
// 		{ abs((l->B()->x()-l->A()->x())*(c->center()->y() - l->A()->y()) - (l->B()->y()-l->A()->y())*(c->center()->x() - 
// 		l->A()->x())) / sqrt(l->length2()) - c->radius() }, 
// 		baseplane_)
// 	{
// 		c->add_constraint()
// 	}
// };

// std::shared_ptr<lineLine_equal> lineLine_equal::make(sketchLine_ptr l1, sketchLine_ptr l2)
// {
// 	return std::shared_ptr<lineLine_equal>(new lineLine_equal(l1, l2));
// }

// lineLine_equal::lineLine_equal(sketchLine_ptr l1, sketchLine_ptr l2):
// 	constraint<8>({ l1->A()->x(), l1->A()->y(), l1->B()->x(), l1->B()->x(), l2->A()->x(), l2->A()->y(), l2->B()->x(), l2->B()->y() },
// 	{ l1->length2() - l2->length2() }, NONE)
// {

// }

// std::shared_ptr<lineCircle_equal> lineCircle_equal::make(sketchLine_ptr l, sketchCircle_ptr c)
// {
// 	return std::shared_ptr<lineCircle_equal>(new lineCircle_equal(l, c));
// }

// lineCircle_equal::lineCircle_equal(sketchLine_ptr l, sketchCircle_ptr c):
// 	constraint<5>({ l->A()->x(), l->A()->y(), l->B()->x(), l->B()->x(), c->radius() },
// 	{ l->length2() - pow(c->radius()*2.0, 2.0) }, NONE)
// {

// }

#endif
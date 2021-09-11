
#ifndef CONSTRAINT_TOOL_HPP_
#define CONSTRAINT_TOOL_HPP_

#include <Workspaces/Sketch_ws.hpp>
#include <Tools/Tool.hpp>

class Constraint_tool : public tool<Sketch_ws> {
public:
	enum add_states { COULDNT_ADD, COULD_ADD, WOULD_BE_COMPLETE };
protected:
	SkDrawable* mEntA, *mEntB, *mCurrentHover;
	int mFilter;
public:
	Constraint_tool(Sketch_ws* env, int filter = Drawable_types::DRAWABLE);

	virtual void init();
	virtual void finish();

	virtual bool manage_button_press(GdkEventButton* event);
	virtual bool manage_mouse_move(GdkEventMotion* event);
// protected:
	virtual int could_add_geom(SkDrawable* geom) = 0;

	virtual void add_geom(SkDrawable* geom);
	void add_constraint();
	void clear_geometries();
	virtual void create_constraint(SkConstraint*& constr, SkDrawable*& priority_ent) = 0;

	// TODO: Inline these thingies?
	static bool is_point(SkDrawable* ent);
	static bool is_line(SkDrawable* ent);
	static bool is_curve(SkDrawable* ent);
	static bool is_point_or_curve(SkDrawable* ent);
};
#endif
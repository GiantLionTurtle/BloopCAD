
#ifndef CONSTRAINT_TOOL_HPP_
#define CONSTRAINT_TOOL_HPP_

#include <workspaces/sketchDesign.hpp>
#include <tools/tool.hpp>

class constraint_tool : public tool<sketchDesign> {
public:
	enum add_states { COULDNT_ADD, COULD_ADD, WOULD_BE_COMPLETE };
protected:
	SkDrawable* mEntA, *mEntB, *mCurrentHover;
	int mFilter;
public:
	constraint_tool(sketchDesign* env, int filter = Drawable_types::DRAWABLE);

	virtual void init();
	virtual void finish();

	virtual bool manage_button_press(GdkEventButton* event);
	virtual bool manage_mouse_move(GdkEventMotion* event);
// protected:
	virtual int could_add_entity(SkDrawable* ent) = 0;

	virtual void add_entity(SkDrawable* ent);
	void add_constraint();
	virtual void create_constraint(SkConstraint*& constr, SkDrawable*& priority_ent) = 0;

	// TODO: Inline these thingies?
	static bool is_point(SkDrawable* ent);
	static bool is_line(SkDrawable* ent);
	static bool is_curve(SkDrawable* ent);
	static bool is_point_or_curve(SkDrawable* ent);
};
#endif
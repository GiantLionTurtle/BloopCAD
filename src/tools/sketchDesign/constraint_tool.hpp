
#ifndef CONSTRAINT_TOOL_HPP_
#define CONSTRAINT_TOOL_HPP_

#include <workspaces/sketchDesign.hpp>
#include <tools/tool.hpp>

class constraint_tool : public tool<sketchDesign> {
public:
	enum add_states { COULDNT_ADD, COULD_ADD, WOULD_BE_COMPLETE };
protected:
	sketchEntity_ptr mEntA, mEntB, mCurrentHover;
public:
	constraint_tool(sketchDesign* env);

	virtual void init();
	virtual void finish();

	virtual bool manage_button_press(GdkEventButton* event);
	virtual bool manage_mouse_move(GdkEventMotion* event);
// protected:
	virtual int could_add_entity(sketchEntity_ptr ent) = 0;

	virtual void add_entity(sketchEntity_ptr ent);
	virtual void add_constraint() = 0;

	// TODO: Inline these thingies?
	static bool is_point(sketchEntity_ptr ent);
	static bool is_line(sketchEntity_ptr ent);
	static bool is_curve(sketchEntity_ptr ent);
	static bool is_point_or_curve(sketchEntity_ptr ent);
};
#endif

#ifndef HORIZONTALITY_TOOL_HPP_
#define HORIZONTALITY_TOOL_HPP_

#include <tools/common/simpleSelector_tool.hpp>
#include <constraintsSolver/equationsSystem.hpp>
#include <workspaces/sketchDesign.hpp>

class horizontality_tool : public simpleSelector_tool<sketchDesign> {
private:
	subEquationsSystem mSysA, mSysB;
	bool mStarted;
public:
	horizontality_tool(sketchDesign* env);

	virtual void init();

	virtual bool manage_button_press(GdkEventButton* event);

	virtual std::string name() { return "horizontality"; }
private:
	bool set_systems(std::vector<subEquationsSystem> sys);
	void add_constraint();
};

#endif
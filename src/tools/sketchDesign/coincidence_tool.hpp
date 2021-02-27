
#ifndef COINCIDENCE_TOOL_HPP_
#define COINCIDENCE_TOOL_HPP_

#include <tools/common/simpleSelector_tool.hpp>
// #include <constraintsSolver/equationsSystem.hpp>
#include <workspaces/sketchDesign.hpp>

class coincidence_tool : public simpleSelector_tool<sketchDesign> {
private:
	// subEquationsSystem mSysA, mSysB;
	bool mStarted;
public:
	coincidence_tool(sketchDesign* env);

	virtual void init();

	virtual bool manage_button_press(GdkEventButton* event);

	virtual std::string name() { return "coincidence"; }
private:
	void add_constraint();
};

#endif
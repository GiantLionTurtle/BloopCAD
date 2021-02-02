
#ifndef PERPENDICULARITY_TOOL_HPP_
#define PERPENDICULARITY_TOOL_HPP_

#include <tools/common/simpleSelector_tool.hpp>
#include <constraintsSolver/expressionVector3.hpp>

class perpendicularity_tool : public simpleSelector_tool {
private:
	subEquationsSystem_vec mSysA, mSysB;
	bool mStarted;
public:
	perpendicularity_tool(workspace* env);

	virtual void init();

	virtual bool manage_button_press(GdkEventButton* event);

	virtual std::string name() { return "perpendicularity"; }
private:
	void add_constraint();
};

#endif
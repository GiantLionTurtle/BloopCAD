
#ifndef COINCIDENCE_TOOL_HPP_
#define COINCIDENCE_TOOL_HPP_

#include <tools/common/simpleSelector_tool.hpp>
#include <entities/geometry/point_abstract.hpp>

class coincidence_tool : public simpleSelector_tool {
private:
	point_abstract_ptr mPointA, mPointB;
	bool mStarted;
public:
	coincidence_tool(workspace* env);

	virtual void init();

	virtual bool manage_button_press(GdkEventButton* event);

	virtual std::string name() { return "coincidence"; }
private:
	void point_point_coincidence();
};

#endif

#ifndef VERTICALITY_TOOL_HPP_
#define VERTICALITY_TOOL_HPP_

#include <tools/common/simpleSelector_tool.hpp>
// #include <constraintsSolver/equationsSystem.hpp>
#include <workspaces/sketchDesign.hpp>

class verticality_tool : public simpleSelector_tool<sketchDesign> {
private:
	// subEquationsSystem mSysA, mSysB;
	bool mStarted;
public:
	verticality_tool(sketchDesign* env);

	virtual void init();

	virtual bool manage_button_press(GdkEventButton* event);

	virtual std::string name() { return "verticality"; }
private:
	// bool set_systems(std::vector<subEquationsSystem> sys);
	void add_constraint();
};

#endif
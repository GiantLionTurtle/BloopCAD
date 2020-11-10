
#ifndef SIMPLESELECTOR_TOOL_HPP_
#define SIMPLESELECTOR_TOOL_HPP_

#include <tools/tool.hpp>

class simpleSelector_tool : public tool_abstract {
private:
	std::shared_ptr<entity> mCurrentHover;
public: 
	simpleSelector_tool(std::shared_ptr<std::shared_ptr<workspaceState>> workspaceState_);

	virtual bool manage_mouse_move(GdkEventMotion* event);

	virtual std::shared_ptr<entity> entity_at_point(glm::vec2 point);
protected:
	std::shared_ptr<entity> primitive_entity_at_point(glm::vec2 point);
};

#endif

#ifndef SIMPLESELECTOR_TOOL_HPP_
#define SIMPLESELECTOR_TOOL_HPP_

#include <tools/tool.hpp>

class simpleSelector_tool : public tool_abstract {
private:
	std::shared_ptr<entity> mCurrentHover;
public: 
	simpleSelector_tool(workspace* env);

	virtual bool manage_button_press(GdkEventButton* event);
	virtual bool manage_mouse_move(GdkEventMotion* event);

	virtual std::shared_ptr<entity> entity_at_point(glm::vec2 point);
protected:
	glm::ivec3 id_at_point(glm::vec2 point);
	std::shared_ptr<entity> primitive_entity_at_point(glm::vec2 point);
};

#endif
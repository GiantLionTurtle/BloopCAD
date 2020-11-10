
#ifndef PLANESELECTOR_TOOL_HPP_
#define PLANESELECTOR_TOOL_HPP_

#include "simpleSelector_tool.hpp"

class planeSelector_tool : public simpleSelector_tool {
private:

public:
    planeSelector_tool(std::shared_ptr<std::shared_ptr<workspaceState>> workspaceState_);

   	virtual std::shared_ptr<entity> entity_at_point(glm::vec2 point);
};

#endif
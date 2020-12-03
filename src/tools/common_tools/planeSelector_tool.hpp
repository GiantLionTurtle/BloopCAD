
#ifndef PLANESELECTOR_TOOL_HPP_
#define PLANESELECTOR_TOOL_HPP_

#include "simpleSelector_tool.hpp"
#include <entities/forward_entities.hpp>

class planeSelector_tool : public simpleSelector_tool {
private:

public:
    planeSelector_tool(workspace* env);

   	virtual std::shared_ptr<entity> entity_at_point(glm::vec2 point);
};

#endif
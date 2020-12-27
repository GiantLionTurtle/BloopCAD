
#ifndef PLANESELECTOR_TOOL_HPP_
#define PLANESELECTOR_TOOL_HPP_

#include "simpleSelector_tool.hpp"
#include <entities/forward_entities.hpp>

/*
    @class planeSelector_tool describes a selector tool that only select entities that can be reduced to plane_abstract
    @parent : simpleSelector_tool
*/
class planeSelector_tool : public simpleSelector_tool {
public:
    /*
		@function planeSelector_tool creates a simpleSelector_tool object 

		@param env : The workspace owning the tool
	*/
    planeSelector_tool(workspace* env);
    virtual ~planeSelector_tool() {};

    virtual bool should_hover(std::shared_ptr<entity> ent);

    /*
        @function entity_at_point is an override of the parent's entity_at_point to filter out non planes

        @param point : The point on screen to evaluate
    */
   	virtual std::shared_ptr<entity> entity_at_point(glm::vec2 point);
protected:
    bool entity_valid(std::shared_ptr<entity> ent);
};

#endif
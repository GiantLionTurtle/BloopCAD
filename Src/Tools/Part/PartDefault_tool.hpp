
#ifndef PARTDEFAULT_TOOL_HPP_
#define PARTDEFAULT_TOOL_HPP_

#include <Tools/Common/SimpleSelector_tool.hpp>
#include <Workspaces/Part_ws.hpp>

class PartDefault_tool : public SimpleSelector_tool<Part_ws> {
public:
    PartDefault_tool(Part_ws* env);
    virtual ~PartDefault_tool() {};

    virtual void act_on_entity(Drawable* drw);

	virtual std::string name() { return "part design default"; }
};

#endif
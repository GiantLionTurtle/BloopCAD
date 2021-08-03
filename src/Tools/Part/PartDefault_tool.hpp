
#ifndef PARTDEFAULT_TOOL_HPP_
#define PARTDEFAULT_TOOL_HPP_

#include <Tools/Common/SimpleSelector_tool.hpp>
#include <workspaces/partDesign.hpp>

class PartDefault_tool : public SimpleSelector_tool<partDesign> {
public:
    PartDefault_tool(partDesign* env);
    virtual ~PartDefault_tool() {};

    virtual void act_on_entity(Drawable* drw);

	virtual std::string name() { return "part design default"; }
};

#endif
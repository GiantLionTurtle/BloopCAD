
#include <tools/common/simpleSelector_tool.hpp>
#include <workspaces/partDesign.hpp>

class partDesignDefault_tool : public simpleSelector_tool<partDesign> {
public:
    partDesignDefault_tool(partDesign* env);
    virtual ~partDesignDefault_tool() {};

    virtual void act_on_entity(entity_ptr ent);

	virtual std::string name() { return "part design default"; }
};
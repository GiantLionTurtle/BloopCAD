
#include <tools/common/simpleSelector_tool.hpp>

class partDesignDefault_tool : public simpleSelector_tool {
public:
    partDesignDefault_tool(workspace* env);
    virtual ~partDesignDefault_tool() {};

    virtual void act_on_entity(entity_ptr ent);
};
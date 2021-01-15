
#include <tools/common/simpleSelector_tool.hpp>

class sketchDesignDefault_tool : public simpleSelector_tool {
public:
    sketchDesignDefault_tool(workspace* env);
    virtual ~sketchDesignDefault_tool() {};

    virtual bool manage_mouse_move(GdkEventMotion* event);

	virtual std::string name() { return "sketch design default"; }
};
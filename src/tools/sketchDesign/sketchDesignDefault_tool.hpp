
#include <tools/common/simpleSelector_tool.hpp>

class sketchDesignDefault_tool : public simpleSelector_tool {
private:   
    entity_ptr mDraggedEnt;
    bool mMoving;
    glm::vec2 mPrevPos;
public:
    sketchDesignDefault_tool(workspace* env);
    virtual ~sketchDesignDefault_tool() {};
    
    virtual void init();

	virtual bool manage_button_press(GdkEventButton* event);
	virtual bool manage_button_release(GdkEventButton* event);
    virtual bool manage_mouse_move(GdkEventMotion* event);

	virtual std::string name() { return "sketch design default"; }
};
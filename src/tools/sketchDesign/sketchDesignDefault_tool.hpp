
#include <entities/tangibleEntities/sketchEntities/sketchEntity.hpp>
#include <workspaces/sketchDesign.hpp>

class sketchDesignDefault_tool : public tool<sketchDesign> {
private:   
    sketchEntity_ptr mDraggedEnt, mHoveredEnt;
    bool mMoving;
    glm::vec2 mPrevPos;
public:
    sketchDesignDefault_tool(sketchDesign* env);
    virtual ~sketchDesignDefault_tool() {};
    
    virtual void init();

	virtual bool manage_button_press(GdkEventButton* event);
	virtual bool manage_button_release(GdkEventButton* event);
    virtual bool manage_mouse_move(GdkEventMotion* event);

	virtual std::string name() { return "sketch design default"; }
};
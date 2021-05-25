
#include <entities/tangibleEntities/sketchEntities/sketchEntity.hpp>
#include <entities/tangibleEntities/sketchEntities/selectionRectangle.hpp>
#include <workspaces/sketchDesign.hpp>

class sketchDesignDefault_tool : public tool<sketchDesign> {
private:   
    std::shared_ptr<selectionRectangle> mSelectionRect;
    sketchEntity_ptr /*mDraggedEnt, */mHoveredEnt;
    bool mAllowedToMove, mMoving;
    glm::vec2 mPrevPos;
    glm::vec2 mStartPos;

    std::vector<entityPosSnapshot_ptr> mStartMoveSnapshot;
public:
    sketchDesignDefault_tool(sketchDesign* env);
    virtual ~sketchDesignDefault_tool() {};
    
    void init();

	bool manage_key_press(GdkEventKey* event);

	bool manage_button_press(GdkEventButton* event);
	bool manage_button_release(GdkEventButton* event);
    bool manage_mouse_move(GdkEventMotion* event);

	std::string name() { return "sketch design default"; }
private:
    void unselect_all();
};

#include <Drawables/sk/SkDrawable.hpp>
#include <Drawables/sk/SkSelRect.hpp>
#include <workspaces/sketchDesign.hpp>

class SketchDefault_tool : public tool<sketchDesign> {
private:   
	SkSelRect* mSelectionRect;
	Drawable* /*mDraggedEnt, */mHoveredEnt;
	bool mAllowedToMove, mMoving;
	glm::vec2 mPrevPos, mPrevMousePos;
	glm::vec2 mStartPos;

	std::map<var_ptr, float> mStartSnapshot;
public:
	SketchDefault_tool(sketchDesign* env);
	virtual ~SketchDefault_tool();
	
	void init();

	bool manage_key_press(GdkEventKey* event);

	bool manage_button_press(GdkEventButton* event);
	bool manage_button_release(GdkEventButton* event);
	bool manage_mouse_move(GdkEventMotion* event);

	std::string name() { return "sketch design default"; }
};
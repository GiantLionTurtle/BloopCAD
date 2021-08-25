
#include <Drawables/Sk/SkDrawable.hpp>
#include <Drawables/Sk/SkSelRect.hpp>
#include <workspaces/sketchDesign.hpp>

class SketchDefault_tool : public tool<sketchDesign> {
private:   
	SkSelRect* mSelArea;
	SkExpSelPoint mDragCandidate;
	Drawable* mCurrentHover;

	enum modes { NORMAL, DRAGGING, AREASELECT };
	int mMode;

	std::map<var_ptr, float> mStartSnapshot; // Used to creat move actions

	glm::vec2 mLastCursorPos; // Remember last time a cursor pos was used internally
	glm::vec2 mLastPlPos;
	float mAreaSelStartX;
public:
	SketchDefault_tool(sketchDesign* env);
	virtual ~SketchDefault_tool();
	
	void init();

	bool manage_key_press(GdkEventKey* event);

	bool manage_button_press(GdkEventButton* event);
	bool manage_button_release(GdkEventButton* event);
	bool manage_mouse_move(GdkEventMotion* event);

	void update_dragCandidate(glm::vec2 cursorPos);
	void update_hover(glm::vec2 cursorPos);
	void areaSelect(glm::vec2 plPos, float cursorX);

	std::string name() { return "sketch design default"; }
};
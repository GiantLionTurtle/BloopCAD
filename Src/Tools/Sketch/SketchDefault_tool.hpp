
// BloopCAD
// Copyright (C) 2020  BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <Drawables/Sk/SkDrawable.hpp>
#include <Drawables/Sk/SkSelRect.hpp>
#include <Workspaces/Sketch_ws.hpp>

class SketchDefault_tool : public tool<Sketch_ws> {
private:   
	SkSelRect* mSelArea { nullptr };
	SkGeomDragPoint mDragCandidate;
	SkConstrAnnot* mAnnot { nullptr };
	Drawable* mCurrentHover { nullptr };

	enum modes { NORMAL, GEOMDRAG, ANNOTDRAG, AREASELECT };
	int mMode;

	std::map<var_ptr, float> mStartSnapshot; // Used to creat move actions

	glm::vec2 mLastCursorPos; // Remember last time a cursor pos was used internally
	glm::vec2 mLastPlPos;
	float mAreaSelStartX;
public:
	SketchDefault_tool(Sketch_ws* env);
	virtual ~SketchDefault_tool();
	
	void init();

	bool manage_key_press(GdkEventKey* event);

	bool manage_button_press(GdkEventButton* event);
	bool manage_button_release(GdkEventButton* event);
	bool manage_mouse_move(GdkEventMotion* event);

	void update_annot(glm::vec2 cursorPos);
	void update_dragCandidate(glm::vec2 cursorPos);
	void update_hover(glm::vec2 cursorPos);
	void areaSelect(glm::vec2 plPos, float cursorX);

	std::string name() { return "sketch design default"; }
};
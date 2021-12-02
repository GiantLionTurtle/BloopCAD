
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

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

#ifndef PAN2D_TOOL_HPP_
#define PAN2D_TOOL_HPP_

#include <Workspaces/Sketch_ws.hpp>
#include <Tools/Tool.hpp>

class Pan2d_tool : public tool<Sketch_ws> {
private:
	glm::vec3 mTranStart, mDragStart; // Helper record of the position of the mouse on the previous update
	glm::vec3 mPl_right, mPl_up;
	bool is_moving; // If the mouse is currently moving
	Camera* mProxyCam; // Virtual Camera used to calculate the pan offset
public:
	/*
		@function Pan3d_tool creates an Pan3d_tool object

		@param env : The workspace owning the tool
	*/
	Pan2d_tool(Sketch_ws* env);
	~Pan2d_tool();

	virtual void init();

	virtual bool manage_button_press(GdkEventButton* event);
	virtual bool manage_button_release(GdkEventButton* event);
	/*
		@function manage_mouse_move moves the model according to the mouse movement if the right condition are met

		@param event : The motion event handed by gtk
	*/
	virtual bool manage_mouse_move(GdkEventMotion* event);

	virtual std::string name() { return "pan2d"; }
};

#endif
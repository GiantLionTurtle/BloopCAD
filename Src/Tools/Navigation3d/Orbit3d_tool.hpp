
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

#ifndef ORBIT_TOOL_HPP_
#define ORBIT_TOOL_HPP_

#include <Tools/Tool.hpp>

/*
	@class Orbit3d_tool describes the orbit function to rotate the Camera around the center of the scene
	@parent : Tool_abstract
*/
class Orbit3d_tool : public tool<Workspace_abstr> {
private:
	glm::vec2 prevPos; // Helper record of the position of the mouse on the previous update
	bool is_moving; // If the mouse is currently moving
public:
	/*
		@function Orbit3d_tool creates an Orbit3d_tool object

		@param env : The workspace owning the tool
	*/
	Orbit3d_tool(Workspace_abstr* env);

	/*
		@function finish is an override to clean up after the tool stops being used (but not destroyed)
	*/
	virtual void finish();
	/*
		@function init is an override to prepare the tool to be used
	*/
	virtual void init();

	/*
		@function manage_mouse_move moves the Camera around if the right conditions are met

		@param event : The event handed out by gtk
	*/
	virtual bool manage_mouse_move(GdkEventMotion* event);
	/*
		@function manage_button_release calls finish if the middle mouse is released

		@param event : The event handed out by gtk
	*/
	virtual bool manage_button_release(GdkEventButton* event);

	virtual std::string name() { return "orbit"; }
};

#endif
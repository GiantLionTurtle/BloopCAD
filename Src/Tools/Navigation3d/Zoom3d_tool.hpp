
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

#ifndef ZOOM_TOOL_HPP_
#define ZOOM_TOOL_HPP_

#include <Tools/Tool.hpp>

/*
	@class Zoom3d_tool describe the tool used to zoom the model
	@parent : Tool_abstract
*/
class Zoom3d_tool : public tool<Workspace_abstr> {
private:
	glm::vec2 prevPos; // Helper previous position of mouse, on last update
	glm::vec2 zoomStart; // Position of the mouse when the zoom start (with mouse move zoom)
	bool is_zooming; // Is the model being zoomed
public:
	/*
		@function Zoom3d_tool creates an Zoom3d_tool object

		@param env : The workspace owning the tool
	*/
	Zoom3d_tool(Workspace_abstr* env);
	
	/*
		@function finish is an override to clean up after the tool stops being used (but not destroyed)
	*/
	virtual void finish();
	/*
		@function init is an override to prepare the tool to be used
	*/
	virtual void init();

	/*
		@function manage_mouse_move zooms the model relative to the y distance of the mouse
		to the zoom starting point if the right conditions are met

		@param event : The motion event handed by gtk
	*/
	virtual bool manage_mouse_move(GdkEventMotion* event);
	/*
		@function manage_button_release calls finish when zoom condition stop being met

		@param event : The button event handed by gtk
	*/
	virtual bool manage_button_release(GdkEventButton* event);
	/*
		@function manage_scroll zooms the model around the scroll point

		@param event : The scroll event handed by gtk
	*/
	virtual bool manage_scroll(GdkEventScroll* event);

	virtual std::string name() { return "zoom"; }

	/*
		@function zoom scales the model around a point

		@param origin : The scale origin
		@param amount : By how much to scale
	*/
	void zoom(glm::vec2 origin, float amount);
};

#endif
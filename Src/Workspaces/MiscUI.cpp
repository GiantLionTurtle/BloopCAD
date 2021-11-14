
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

#include "MiscUI.hpp"

#include <Utils/Debug_util.hpp>

TabButton::TabButton(std::string const& label_) 
{
	mLabel = Gtk::Label(label_);
	mCloseButton = Gtk::Button("x");
	mCloseButton.set_relief(Gtk::ReliefStyle::RELIEF_NONE); // Make the button blend in

	pack_start(mLabel);
	pack_start(mCloseButton);

	show_all();
}


StatusBar::StatusBar(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& builder)
	: Gtk::Box(cobject)
{
	builder->get_widget("status_workspace", workspaceName);
	builder->get_widget("status_tool", toolName);
}

void StatusBar::update(status which, std::string const& content)
{
	if(which == status::WORKSPACE) {
		workspaceName->set_label(content);
	} else if(which == status::TOOL) {
		toolName->set_label(content);
	} else {
		LOG_WARNING("Attempted to set unknown status: " + std::to_string(which) + ".");
	}
}

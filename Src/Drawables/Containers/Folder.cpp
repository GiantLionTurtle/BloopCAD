
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

#include "Folder.hpp"

Folder::Folder(std::string const& name_)
{
	set_name(name_);
}

void Folder::set_handle(UILink* handle_)
{
	mUILink = handle_;
	// for(int i = 0; i < mDrawList.size(); ++i) {
	// 	auto ent = mDrawList.at(i);
	// 	if(ent->handle()) {
	// 		delete ent->handle();
	// 		ent->set_handle(nullptr);
	// 	}
	// 	ent->set_handle(new entityHandle(ent, mHandle->view(), mHandle));
	// }
}
void Folder::add(Drawable* ent)
{
	if(ent) {
		need_redraw();
		ent->set_parent(this);
		mDrawList.add(ent);
		// if(mHandle)
			// ent->set_handle(new entityHandle(ent, mHandle->view(), mHandle));
	}
}

Drawable* Folder::get(size_t ind)
{
	if(ind <= mDrawList.size())
		return mDrawList.at(ind);
	return nullptr;
}
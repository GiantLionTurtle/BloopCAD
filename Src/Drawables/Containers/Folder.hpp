
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

#ifndef FOLDER_HPP_
#define FOLDER_HPP_

#include <Drawables/Base/Collection.hpp>

class Folder;
using Folder_ptr = std::shared_ptr<Folder>;

class Folder : public Collection_abstr<LinearResizable_indexer<Drawable*>> {
public:
	Folder(std::string const& name_ = "folder");

	virtual void set_handle(UILink* handle_);
	
	void add(Drawable* elem);
	Drawable* get(size_t ind);
};

#endif
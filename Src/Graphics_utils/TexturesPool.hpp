
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

#ifndef TEXTURESPOOL_HPP_
#define TEXTURESPOOL_HPP_

#include "Texture.hpp"

#include <string>
#include <memory>
#include <map>

class TexturesPool {
private:
	std::map<std::string, Texture*> mTextures;
public:
	TexturesPool(TexturesPool const&) = delete; // As it is a singleton, the copy constructor is deleted

	static TexturesPool& get_instance();

	void add(std::string const& name, Texture* texture);
	Texture* get(std::string const& name);
private:
	TexturesPool();
	~TexturesPool();
};

#endif
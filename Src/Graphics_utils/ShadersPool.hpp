
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

#ifndef SHADERSPOOL_HPP_
#define SHADERSPOOL_HPP_

#include "Shader.hpp"

#include <string>
#include <memory>
#include <map>

class ShadersPool {
private:
	std::map<std::string, Shader*> mShaders;
public:
	ShadersPool(ShadersPool const&) = delete; // As it is a singleton, the copy constructor is deleted

	static ShadersPool& get_instance();

	void add(std::string const& name, Shader* Shader_);
	Shader* get(std::string const& name);
private:
	ShadersPool();
	~ShadersPool();
};

#endif

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

#include "ShadersPool.hpp"

#include <Utils/Expunge.hpp>

ShadersPool& ShadersPool::get_instance()
{
	static ShadersPool pool;
	return pool;
}

void ShadersPool::add(std::string const& name, Shader* Shader_)
{
	mShaders[name] = Shader_;
}
Shader* ShadersPool::get(std::string const& name) 
{
	if(mShaders.find(name) != mShaders.end()) {
		return mShaders.at(name);
	}
	return nullptr;
}

ShadersPool::ShadersPool()
{

}
ShadersPool::~ShadersPool()
{
	for(auto it = mShaders.begin(); it != mShaders.end(); ++it) {
		expunge(it->second);
	}
}
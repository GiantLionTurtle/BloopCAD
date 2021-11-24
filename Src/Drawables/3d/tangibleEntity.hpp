
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

#ifndef GEOMETRICENTITY_HPP_
#define GEOMETRICENTITY_HPP_

#include <Drawables/Base/Drawable.hpp>

class tangibleEntity : public Drawable {
protected:
	std::shared_ptr<VertexBuffer> 	mVB; // The vertex buffer to contain the vertices on the gpu
	std::shared_ptr<VertexArray> 	mVA; // The vertex array to contain the attributes on the gpu
	std::shared_ptr<IndexBuffer>    mIB; // The index buffer to contain vertex handles on the gpu
	Shader* 		mShader; // Shader to draw the entity
	bool mNeed_VBUpdate;
public:
	tangibleEntity(): 
		mVB(nullptr), 
		mVA(nullptr), 
		mIB(nullptr),
		mShader(nullptr), 
		mNeed_VBUpdate(false) {}
	virtual ~tangibleEntity() {}

	virtual void init() = 0;

	void draw(Camera* cam, int frame, draw_type type = draw_type::ALL, bool on_required = false)
	{
		// This is not exactly duplicate logic from what is in entity.cpp
		if(!mInited) {
			init();
			mInited = true;
		}
		if(mNeed_VBUpdate)
			update_VB();
		Drawable::draw(cam, frame, type, on_required);
	}

	virtual void notify(int msg)
	{
		if(msg == UPDATED && mParent)
			mParent->notify(msg);
		set_require_VBUpdate();
	}

	virtual void update_VB() = 0;

	void set_require_VBUpdate() { mNeed_VBUpdate = true; set_need_redraw(); notify_parent(UPDATED); }
	bool require_VBUpdate() const { return mNeed_VBUpdate; }
protected:
	virtual void graphicUpdate_impl() { set_require_VBUpdate(); }
};

#endif

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

#include "SkSelRect.hpp"

#include <Utils/Expunge.hpp>
#include <Graphics_utils/GLCall.hpp>
#include <Graphics_utils/Camera.hpp>
#include <Graphics_utils/VertexBuffer.hpp>
#include <Graphics_utils/VertexBufferLayout.hpp>
#include <Graphics_utils/IndexBuffer.hpp>
#include <Graphics_utils/VertexArray.hpp>
#include <Graphics_utils/ShadersPool.hpp>

SkSelRect::SkSelRect(glm::vec2 start, glm::vec2 end, Geom3d::Plane_abstr* basePlane_)
	: SkDrawable(basePlane_)
	, mStartPt(start)
	, mEndPt(end)
	, mMode(TOUCH)
	, mVA(nullptr)
	, mVB(nullptr)
	, mShader(nullptr)
{
	set_name("selection");
}
SkSelRect::~SkSelRect()
{
	if(mInited) {
		expunge(mVA);
		expunge(mVB);
		expunge(mIB);
	}
}

void SkSelRect::set_endPoint(glm::vec2 pt)
{
	mEndPt = pt;
    glm::vec2 diff = mEndPt - mStartPt;
	mVertices[1] = mBasePlane->to_worldPos(mStartPt + glm::vec2(0.0f, diff.y));
	mVertices[2] = mBasePlane->to_worldPos(mEndPt);
	mVertices[3] = mBasePlane->to_worldPos(mStartPt + glm::vec2(diff.x, 0.0f));
	mNeed_graphicUpdate = true;
	set_need_redraw();
}
void SkSelRect::set_points(glm::vec2 start_, glm::vec2 end_)
{
	mStartPt = start_;
	mEndPt = end_;
	glm::vec2 diff = end_ - start_;
	mVertices[0] = mBasePlane->to_worldPos(mStartPt);
	mVertices[1] = mBasePlane->to_worldPos(mStartPt + glm::vec2(0.0f, diff.y));
	mVertices[2] = mBasePlane->to_worldPos(mEndPt);
	mVertices[3] = mBasePlane->to_worldPos(mStartPt + glm::vec2(diff.x, 0.0f));
	mNeed_graphicUpdate = true;
	set_need_redraw();
}

void SkSelRect::init_impl()
{
	set_points(mStartPt, mEndPt);
	init_buffers();
	
	mVB = new VertexBuffer(mVertices, sizeof(glm::vec3) * 4); // Upload the vertices
	mVA = new VertexArray();

	VertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();
	mVA->add_buffer(*mVB, layout);
	mIB = new IndexBuffer(mIndices, 6);

	// Create the Shaders
	mShader = ShadersPool::get_instance().get("plane");
	if(!mShader) {
		mShader = Shader::fromFiles_ptr("Resources/Shaders/Point.vert", "Resources/Shaders/PlainColor.frag");
		ShadersPool::get_instance().add("plane", mShader);
	}
}
void SkSelRect::draw_impl(Camera* cam, int frame, draw_type type)
{
	mShader->bind();
	glm::vec3 color;
	if(mMode == COVER) {
		color = mColorA;
	} else {
		color = mColorB;
	}
	mShader->setUniform4f("u_Color", color.r, color.g, color.b, 0.3f);

	if(mShader->lastUsed() != frame) {
		mShader->setUniformMat4f("u_MVP", cam->mvp());
		mShader->set_used(frame);
	}
	mVA->bind();
	mIB->bind();

	GLCall(glDrawElements(GL_TRIANGLES, mIB->count(), GL_UNSIGNED_INT, nullptr)); // Actual draw call

	// Clean up
	mIB->unbind();
	mVA->unbind();
	mShader->unbind();
}
void SkSelRect::graphicUpdate_impl()
{
	mVB->bind();
	mVB->set(mVertices, sizeof(glm::vec3) * 4);
	mVB->unbind();
}

void SkSelRect::set_startPoint(glm::vec2 pt)
{
	mStartPt = pt;
    glm::vec2 diff = mEndPt - mStartPt;
	mVertices[0] = mBasePlane->to_worldPos(mStartPt);
	mVertices[1] = mBasePlane->to_worldPos(mStartPt + glm::vec2(0.0f, diff.y));
	mVertices[3] = mBasePlane->to_worldPos(mStartPt + glm::vec2(diff.x, 0.0f));
	mNeed_graphicUpdate = true;
	set_need_redraw();
}

void SkSelRect::init_buffers()
{
	mIndices[0] = 0;
	mIndices[1] = 1;
	mIndices[2] = 2; // First triangle
	mIndices[3] = 2;
	mIndices[4] = 3;
	mIndices[5] = 0; // Second triangle
}
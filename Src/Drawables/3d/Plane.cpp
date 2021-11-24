
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

#include "Plane.hpp"

#include <Utils/Expunge.hpp>
#include <Graphics_utils/GLCall.hpp>
#include <Graphics_utils/ShadersPool.hpp>
#include <Graphics_utils/VertexArray.hpp>
#include <Graphics_utils/VertexBuffer.hpp>
#include <Graphics_utils/Shader.hpp>
#include <Graphics_utils/IndexBuffer.hpp>
#include <Graphics_utils/Camera.hpp>

Plane::Plane(Plane_abstr const& plane_):
	Plane_abstr(plane_),
	mVA(nullptr),
	mVB(nullptr),
	mShader(nullptr)
{
	mType |= Drawable_types::PLANE;
	set_name("plane");
}
Plane::~Plane()
{
	if(mInited) {
		expunge(mVA);
		expunge(mVB);
		expunge(mIB);
	}
}

SelPoint Plane::closest(glm::vec2 cursor, Camera* cam, int filter)
{
	if(!(mType & filter))
		return SelPoint();
	glm::vec3 inter = line_intersection(cam->pos(), cam->cast_ray(cursor));
	glm::vec2 plane_pos = to_planePos(inter);
	if(std::abs(plane_pos.x) <= glm::length(mV) && std::abs(plane_pos.y) <= glm::length(mW)) {
		return SelPoint(this, glm::length(cam->pos()-inter));
	}
	return SelPoint();
}

void Plane::init_impl()
{
	set_need_redraw();
	init_buffers(); // Set all the vertices to the right values
	
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

void Plane::draw_impl(Camera* cam, int frame, draw_type type)
{
	// TODO: make this less sketch?
	// GLCall(glDisable(GL_DEPTH_TEST)); // Disable the depth buffer to draw the whole quad, even if it is hidden by another semi-transparent quad
	mShader->bind();

	glm::vec3 color;
	if(dist_to_point_signed(cam->pos()) >= 0) {
		color = mColorA;
	} else {
		color = mColorB;
	}
	if(selected()) 
		color += glm::vec3(0.0f, 0.0f, 0.4f); // Make it blue-er if selected
	mShader->setUniform4f("u_Color", color.r, color.g, color.b, hovered() || selected() ? 0.5 : 0.3);

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
	// GLCall(glEnable(GL_DEPTH_TEST));
}

void Plane::graphicUpdate_impl()
{
	init_buffers();
	mVB->bind();
	mVB->set(mVertices, sizeof(glm::vec3) * 4);
	mVB->unbind();
}


void Plane::init_buffers()
{
	// Set all vertices in anti-clockwise fashion
	mVertices[0] = mOrigin + mV + mW;
	mVertices[1] = mOrigin - mV + mW;
	mVertices[2] = mOrigin - mV - mW;
	mVertices[3] = mOrigin + mV - mW;

	mIndices[0] = 0;
	mIndices[1] = 1;
	mIndices[2] = 2; // First triangle
	mIndices[3] = 2;
	mIndices[4] = 3;
	mIndices[5] = 0; // Second triangle
}

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

#include "TextQuad.hpp"

#include <Graphics_utils/GLCall.hpp>
#include <Graphics_utils/ShadersPool.hpp>
#include <Graphics_utils/VertexArray.hpp>
#include <Graphics_utils/VertexBuffer.hpp>
#include <Graphics_utils/Shader.hpp>
#include <Graphics_utils/IndexBuffer.hpp>
#include <Graphics_utils/Camera.hpp>

TextQuad::TextQuad(glm::vec3 anchorPos, AnchorPoints anchorPoint, glm::vec3 color)
	: mAnchorPos(anchorPos)
	, mAnchorPoint(anchorPoint)
	, mColor(color)
{
	
}

void TextQuad::init_impl()
{

}
void TextQuad::draw_impl(Camera* cam, int frame, draw_type type)
{
	mShader->setUniform4f("u_Color", glm::vec4(mColor, 1.0f));

	if(mShader->lastUsed() != frame) {
		mShader->setUniformMat4f("u_MVP", cam->mvp());
		mShader->set_used(frame);
	}
	
	mVA->bind();
	mIB->bind();

	GLCall(glDrawArrays(GL_POINTS, 0, 1));
	
	// Clean up
	mVA->unbind();
	mShader->unbind();
}
void TextQuad::graphicUpdate_impl()
{
	init_buffers();
	mVB->bind();
	mVB->set(&mAnchorPos, sizeof(glm::vec3));
	mVB->unbind();
}
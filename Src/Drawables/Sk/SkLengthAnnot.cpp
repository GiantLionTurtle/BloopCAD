

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

#include "SkLengthAnnot.hpp"

#include <Graphics_utils/ShadersPool.hpp>
#include <Graphics_utils/TexturesPool.hpp>
#include <Graphics_utils/Camera.hpp>
#include <Graphics_utils/Image.hpp>
#include <Graphics_utils/GLCall.hpp>
#include <Utils/Maths_util.hpp>

SkLineLengthAnnot::SkLineLengthAnnot(Geom3d::Plane_abstr* pl, Geom2d::Line* l)
	: SkNumericAnnot(pl)
	, mLine(l)
{

}

int SkLineLengthAnnot::expected_unit()
{
	return Units::milimeter;
}

void SkLineLengthAnnot::init_impl()
{
	mNeed_graphicUpdate = false;
	
	mVA = new VertexArray();
	VertexBufferLayout layout;
	layout.add_proprety_float(3); // Position
	layout.add_proprety_float(3); // Direction
	mVA->bind();

	set_vertices();
	mVB = new VertexBuffer(&mVertices[0], sizeof(ArrowVertex) * 2);
	mVA->add_buffer(*mVB, layout);
	mVA->unbind();

	BLOOP_MARKER;
	mLineShader = ShadersPool::get_instance().get("linelength");
	if(!mLineShader) {
		mLineShader = Shader::fromFiles_ptr({
		{"Resources/Shaders/Point.vert", GL_VERTEX_SHADER},
		{"Resources/Shaders/LineLength.geom", GL_GEOMETRY_SHADER}, 
		{"Resources/Shaders/Line.frag", GL_FRAGMENT_SHADER}});
		ShadersPool::get_instance().add("linelength", mLineShader);
	}

	BLOOP_MARKER;
	mArrowShader = ShadersPool::get_instance().get("arrow");
	if(!mArrowShader) {
		mArrowShader = Shader::fromFiles_ptr({
		{"Resources/Shaders/Arrow.vert", GL_VERTEX_SHADER},
		{"Resources/Shaders/Arrow.geom", GL_GEOMETRY_SHADER}, 
		{"Resources/Shaders/TextureBacklit.frag", GL_FRAGMENT_SHADER}}); // Geometry Shader is needed because point is expanded on the gpu
		ShadersPool::get_instance().add("arrow", mArrowShader);
	}
	mTexture = TexturesPool::get_instance().get("Resources/Textures/Images/Icons/Sketch/Constraints/Arrow.png");
	if(!mTexture) {
		mTexture = new Image("Resources/Textures/Images/Icons/Sketch/Constraints/Arrow.png");
		TexturesPool::get_instance().add("Resources/Textures/Images/Icons/Sketch/Constraints/Arrow.png", mTexture);
	}
	
}
void SkLineLengthAnnot::draw_impl(Camera* cam, int frame, draw_type type)
{
	mArrowShader->bind();

	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

	mArrowShader->setUniform3f("u_Color", color);
	mArrowShader->setUniform1f("u_Width", 10.0);
	mArrowShader->setUniform1f("u_Height", 10.0);

	if(mArrowShader->lastUsed() != frame) {
		mArrowShader->setUniformMat4f("u_MVP", cam->mvp());
		mArrowShader->setUniform2f("u_Viewport", cam->viewport());
		mArrowShader->set_used(frame);
	}

	mTexture->bind();
	mVA->bind();

	GLCall(glDrawArrays(GL_POINTS, 0, 2));

	mVA->unbind();
	mTexture->unbind();
	mArrowShader->unbind();
}
void SkLineLengthAnnot::graphicUpdate_impl()
{
	
}

void SkLineLengthAnnot::set_vertices()
{
	glm::dvec2 cross = glm::normalize(perpendicular_ccw(mLine->as_vec()));
	glm::dvec3 dir = glm::normalize(mBasePlane->to_worldPos(mLine->as_vec()));

	mVertices[0] = { mBasePlane->to_worldPos(mLine->posA() + cross * mDistToLine),  dir };
	mVertices[1] = { mBasePlane->to_worldPos(mLine->posB() + cross * mDistToLine), -dir };
}

Constraint* SkLineLengthAnnot::build_constr_impl()
{
	mConstr = new Line_length(mLine, mVal);
	return mConstr;
}
void SkLineLengthAnnot::update_constr()
{
	mConstr->set_length(mVal);
}
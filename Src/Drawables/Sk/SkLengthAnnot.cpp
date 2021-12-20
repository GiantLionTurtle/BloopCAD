

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

struct ArrowVertex {
	glm::vec3 pos;
	glm::vec3 dir;
};

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
	
	{
		mArrowVA = new VertexArray();
		VertexBufferLayout layout;
		layout.add_proprety_float(3); // Position
		layout.add_proprety_float(3); // Direction
		mArrowVA->bind();

		mArrowVB = new VertexBuffer(sizeof(ArrowVertex) * 2);
		mArrowVA->add_buffer(*mArrowVB, layout);
		mArrowVA->unbind();
	}
	{
		mLegsVA = new VertexArray();
		VertexBufferLayout layout;
		layout.add_proprety_float(3); // Position
		mLegsVA->bind();

		mLegsVB = new VertexBuffer(sizeof(glm::vec3) * 4);
		mLegsVA->add_buffer(*mLegsVB, layout);
		mLegsVA->unbind();
	}

	fill_vbs();

	mLineShader = ShadersPool::get_instance().get("linelength");
	if(!mLineShader) {
		mLineShader = Shader::fromFiles_ptr({
		{"Resources/Shaders/Point.vert", GL_VERTEX_SHADER},
		{"Resources/Shaders/LineLengthLegs.geom", GL_GEOMETRY_SHADER}, 
		{"Resources/Shaders/Line.frag", GL_FRAGMENT_SHADER}});
		ShadersPool::get_instance().add("linelength", mLineShader);
	}

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
	mArrowVA->bind();

	GLCall(glDrawArrays(GL_POINTS, 0, 2));

	mArrowVA->unbind();
	mTexture->unbind();
	mArrowShader->unbind();

	/* dimensioning legs */

	mLineShader->bind();

	mLineShader->setUniform4f("u_Color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	mLineShader->setUniform1f("u_LineWidth", 2); 	// Line width is in pixel
	mLineShader->setUniform1f("u_Feather", 0.6);	// Amount of feather, the formula might change so for the time being, higher is more feather

	if(mLineShader->lastUsed() != frame) {
		mLineShader->setUniformMat4f("u_MVP", cam->mvp());
		mLineShader->setUniform2f("u_Viewport", cam->viewport());
		mLineShader->set_used(frame);
	}

	mLegsVA->bind();

	GLCall(glDrawArrays(GL_LINES, 0, 4));

	mLegsVA->unbind();
	mLineShader->unbind();
}
void SkLineLengthAnnot::graphicUpdate_impl()
{
	fill_vbs();
}

void SkLineLengthAnnot::moveTo(glm::vec2 pos)
{
	mDistToLine = mLine->signed_dist_to_point(pos);
	// std::cout<<"Closest: "<<mLine->closest_to_point_interp(pos)<<"\n";
	float closest_lerp = mLine->closest_to_point_interp(pos);
	if(closest_lerp == 0.0) {
		mDirection = -1;
	} else if(closest_lerp == 1.0) {
		mDirection = 1;
	} else {
		mDirection = 0;
	}

	// mDistToLine = Geom2d::Line::ccw(mLine->posA(), glm::vec2(0.0, 0.0), mLine->posB()) ? mDistToLine : -mDistToLine;
	set_need_graphicUpdate();
}

glm::dvec2 flip_to_second_third(glm::dvec2 vec)
{
	if(vec.x >= 0.0)
		return -vec;
	return vec;
}

void SkLineLengthAnnot::fill_vbs()
{
	mArrowVB->bind();
	
	glm::dvec2 linevec = mLine->as_vec();
	glm::dvec2 cross = glm::normalize(perpendicular_ccw(flip_to_second_third(linevec)));
	// flip_to_first_fourth(cross);
	glm::dvec3 dir = glm::normalize(mBasePlane->to_worldPos(linevec));
	double dir_mod = mDirection == 0 ? 1.0 : -1.0;

	ArrowVertex arrowVert[2] = 
		{ 	{mBasePlane->to_worldPos(mLine->posA() + cross * mDistToLine),  dir * dir_mod},
			{mBasePlane->to_worldPos(mLine->posB() + cross * mDistToLine), -dir * dir_mod}};

	mArrowVB->set(&arrowVert[0], sizeof(ArrowVertex) * 2);
	mArrowVB->unbind();


	mLegsVB->bind();

	glm::vec3 legsVert[4] = 
		{	arrowVert[0].pos, mBasePlane->to_worldPos(mLine->posA()),
			arrowVert[1].pos, mBasePlane->to_worldPos(mLine->posB()) };
	
	mLegsVB->set(&legsVert[0], sizeof(glm::vec3) * 4);
	mLegsVB->unbind();
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

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

#include "SkLineCurve.hpp"

#include <Graphics_utils/ShadersPool.hpp>
#include <Utils/Preferences.hpp>
#include <Graphics_utils/GLCall.hpp>
#include <Utils/Debug_util.hpp>

bool SkLine::kFisrstInst = true;
glm::vec3 SkLine::kColor = glm::vec3(0.0); 
glm::vec3 SkLine::kColorHovered = glm::vec3(0.0);
glm::vec3 SkLine::kColorSelected = glm::vec3(0.0);

SkLine::SkLine(glm::vec2 ptA_, glm::vec2 ptB_, Geom3d::Plane_abstr* pl, bool fixed_)
	: SkCurve<LinearFixed_indexer<SkPoint*, 2>, Geom2d::Line>(pl, fixed_)
{
	mGeom = new Geom2d::Line(ptA_, ptB_);
	handle(0) = new SkPoint(mGeom->ptA(), pl, fixed_);
	handle(1) = new SkPoint(mGeom->ptB(), pl, fixed_);
	set_name("SkLine");
}

void SkLine::set_annotOffset(SkSprite* sp, int ind)
{
	glm::vec2 dir = glm::normalize(posA() - posB());
	glm::vec2 normal = glm::cross(glm::vec3(dir, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	int line_side = ind % 2 == 0 ? 1 : -1;
	bool icon_dir = line_side == -1 ? ind % 4 ? -1 : 1 : ind % 3 ? -1 : 1;
	float dir_offset = line_side == 1 ? ind : ind - 1;
	sp->set_pixelOffset(dir * 25.0f * (float)icon_dir * dir_offset + normal * 25.0f * (float)line_side);
}

void SkLine::init_impl()
{
	// asvecExp = *ptA() - *ptB();
	mNeed_graphicUpdate = false;
	
	// mLength2 = (pow(mA->x()-mB->x(), 2.0) + pow(mA->y()-mB->y(), 2.0));

	mVA = new VertexArray();
	VertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();

	mVertices[0] = mBasePlane->to_worldPos(posA());
	mVertices[1] = mBasePlane->to_worldPos(posB());
	mVB = new VertexBuffer(&mVertices[0], sizeof(glm::vec3) * 2);
	mVA->add_buffer(*mVB, layout);
	mVA->unbind();

	mShader = ShadersPool::get_instance().get("line");
	if(!mShader) {
		mShader = Shader::fromFiles_ptr({
		{"Resources/Shaders/Point.vert", GL_VERTEX_SHADER},
		{"Resources/Shaders/Line.geom", GL_GEOMETRY_SHADER}, 
		{"Resources/Shaders/Line.frag", GL_FRAGMENT_SHADER}}); // Geometry Shader is needed because line is expanded on the gpu
		ShadersPool::get_instance().add("line", mShader);
	}
	if(kFisrstInst) {
		kSelDist2 = Preferences::get_instance().get_float("seldistcurve2");
		Preferences::get_instance().add_callback("seldistcurve2", std::function<void(float)>([this](float val) { kSelDist2 = val; }));
		kColor = Preferences::get_instance().get_vec3("sketchEntityColor");
		Preferences::get_instance().add_callback("sketchEntityColor", std::function<void(glm::vec3)>([this](glm::vec3 val) { kColor = val; }));
		kColorHovered = Preferences::get_instance().get_vec3("sketchEntityColorHovered");
		Preferences::get_instance().add_callback("sketchEntityColorHovered", std::function<void(glm::vec3)>([this](glm::vec3 val) { kColorHovered = val; }));
		kColorSelected = Preferences::get_instance().get_vec3("sketchEntityColorSelected");
		Preferences::get_instance().add_callback("sketchEntityColorSelected", std::function<void(glm::vec3)>([this](glm::vec3 val) { kColorSelected = val; }));

		kFisrstInst = false;
	}
}

void SkLine::draw_impl(Camera* cam, int frame, draw_type type)
{
	mShader->bind();
	glm::vec4 color = glm::vec4(kColor, 1.0f);
	if(selected()) {
		color = glm::vec4(kColorSelected, 1.0f);
	} else if(hovered()) {
		color = glm::vec4(kColorHovered, 1.0f);
	}

	mShader->setUniform4f("u_Color", color);
	mShader->setUniform1f("u_LineWidth", 5); 	// Line width is in pixel
	mShader->setUniform1f("u_Feather", 0.6);	// Amount of feather, the formula might change so for the time being, higher is more feather

	if(mShader->lastUsed() != frame) {
		mShader->setUniformMat4f("u_MVP", cam->mvp());
		mShader->setUniform2f("u_Viewport", cam->viewport());
		mShader->set_used(frame);
	}

	mVA->bind();

	GLCall(glDrawArrays(GL_LINES, 0, 2)); // No indexing needed, a line only has two vertices

	mVA->unbind();
	mShader->unbind();
}
void SkLine::graphicUpdate_impl()
{
	update_annots();
	mNeed_graphicUpdate = false;
	mVertices[0] = mBasePlane->to_worldPos(posA());
	mVertices[1] = mBasePlane->to_worldPos(posB());
	mVB->bind();
	mVB->set(&mVertices[0], sizeof(glm::vec3) * 2);
	mVB->unbind();
}

void SkLine::internalUpdate_impl()
{
	// Param* Ax = ptA()->x();
	// Param* Ay = ptA()->y();
	// Param* Bx = ptB()->x();
	// Param* By = ptB()->y();
	
	// verbose(VERBOSE_INNERSTEPS, "Lineupdate drag: "<<Ax->dragged()<<",  "<<
	// Ay->dragged()<<" : "<<Bx->dragged()<<",  "<<By->dragged())

	// if(Ax->dragged() && !Bx->dragged()) {
	// 	Bx->set_dragged(true);
	// 	verbose(VERBOSE_INNERSTEPS, "Set ptbx");
	// } else if(!Ax->dragged() && Bx->dragged()) {
	// 	Ax->set_dragged(true);
	// 	verbose(VERBOSE_INNERSTEPS, "Set ptax");
	// }

	// if(Ay->dragged() && !By->dragged()) {
	// 	By->set_dragged(true);
	// 	verbose(VERBOSE_INNERSTEPS, "Set ptby");
	// } else if(!Ay->dragged() && By->dragged()) {
	// 	Ay->set_dragged(true);
	// 	verbose(VERBOSE_INNERSTEPS, "Set ptay");
	// }
}
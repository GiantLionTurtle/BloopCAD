

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

#ifndef SKLENGTHANNOT_HPP_
#define SKLENGTHANNOT_HPP_

#include "SkNumericAnnot.hpp"
#include <Graphics_utils/VertexArray.hpp>
#include <Graphics_utils/VertexBuffer.hpp>
#include <Graphics_utils/Shader.hpp>
#include <Graphics_utils/Texture.hpp>

struct ArrowVertex {
	glm::vec3 pos;
	glm::vec3 dir;
};

class SkLineLengthAnnot : public SkNumericAnnot {
private:	
	Geom2d::Line* mLine;
	double mDistToLine { 0.0 };

	Line_length* mConstr;

	ArrowVertex mVertices[2];
	VertexArray* mVA;
	VertexBuffer* mVB;
	Shader* mLineShader, *mArrowShader;
	Texture* mTexture;
public:
	SkLineLengthAnnot(Geom3d::Plane_abstr* pl, Geom2d::Line* l);

	int expected_unit();
protected:
	void init_impl();
	void draw_impl(Camera* cam, int frame, draw_type type);
	void graphicUpdate_impl();

	void moveTo(glm::vec2 pos);

	void set_vertices();

	bool closest_2d(SelPoint& selP, glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) { return false; }

	Constraint* build_constr_impl();
	void update_constr();
};

#endif
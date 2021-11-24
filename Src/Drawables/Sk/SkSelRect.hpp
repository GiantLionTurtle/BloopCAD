
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

#ifndef SELECTIONRECTANGLE_HPP_
#define SELECTIONRECTANGLE_HPP_

#include "SkDrawable.hpp"
#include <Geometry/3d/Plane_abstr.hpp>

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Shader;

class SkSelRect : public SkDrawable {
public:
	enum mode_types { TOUCH, COVER };
private:
	const glm::vec3 mColorA = glm::vec3(1.0f, 0.0f, 0.0f), 
					mColorB = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 mVertices[4];	// The four vertices of the rectangle that represents the plane
	unsigned int mIndices[6];
	glm::vec2 mStartPt, mEndPt;

	int mMode;

	VertexArray* mVA;
	VertexBuffer* mVB;
	IndexBuffer* mIB;
	Shader* mShader;
public:
	SkSelRect(glm::vec2 start_, glm::vec2 end_, Geom3d::Plane_abstr* basePlane_);
	virtual ~SkSelRect();

	glm::vec2 start() const { return mStartPt; }
	void set_startPoint(glm::vec2 pt);
	glm::vec2 end() const { return mEndPt; }
	void set_endPoint(glm::vec2 pt);
	void set_points(glm::vec2 start_, glm::vec2 end_);

	void set_mode(int md) { mMode = md; }
	int mode() const { return mMode; }

	int selection_rank() { return -1; }

	// These should not be called
	virtual SelPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) { return SelPoint(); }
	virtual SkExpSelPoint closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) { return SkExpSelPoint(); }
protected:	
	void init_impl();
	void draw_impl(Camera* cam, int frame, draw_type type = draw_type::ALL);
	void graphicUpdate_impl();

	void init_buffers();
};

#endif
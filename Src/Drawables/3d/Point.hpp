
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

#ifndef POINT_HPP_
#define POINT_HPP_

#include <Drawables/Base/Drawable.hpp>
#include <Geometry/3d/Point_abstr.hpp>
#include <ConstraintsSolver/Expression.hpp>

#include <glm/glm.hpp>

class VertexArray;
class VertexBuffer;
class Shader;

class Point : public Drawable, public Geom3d::Point_abstr {
private:
	const glm::vec3 mColor = glm::vec3(0.0f, 0.89f, 0.725f); // Point color

	var_ptr mX, mY, mZ;

	VertexArray* mVA;
	VertexBuffer* mVB;
	Shader* mShader;
public:
	Point(glm::vec3 const& basePoint);
	virtual ~Point();

	virtual int selection_rank() { return 5; }
	SelPoint closest(glm::vec2 cursor, Camera* cam, int filter);

	virtual void move(glm::vec3 from, glm::vec3 to);

	glm::vec3 pos() const;
	void set(glm::vec3 const& p);
protected:
	void init_impl();
	void draw_impl(Camera* cam, int frame, draw_type type = draw_type::ALL);
	void graphicUpdate_impl();
};

#endif

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

#ifndef SKSPRITE_HPP_
#define SKSPRITE_HPP_

#include "SkDrawable.hpp"
#include <Geometry/2d/Point_abstr.hpp>
#include <ConstraintsSolver/Expression_forward.hpp>
#include <Graphics_utils/Texture.hpp>

#include <map>
#include <string>

class VertexArray;
class VertexBuffer;
class Shader;

struct pos_offseted {
	glm::vec3 pos;
	glm::vec2 pixelOffset;
};
class SkSprite : public SkIntDrawable {
private:
	static bool kFisrstInst;
	static glm::vec3 kColorHovered;

	std::string mTexturePath;
	Texture* mTexture;
	glm::vec2 mDimensions;
	glm::vec2 mPixelOffset; // A sprite has a position in the world as well
			 				// as a screen offset that stays constant regardless of zoom

	std::array<var_ptr, 2> mPos;
	VertexArray* mVA;
	VertexBuffer* mVB;
	Shader* mShader;
public:
	SkSprite(Geom3d::Plane_abstr* basePlane_, glm::vec2 dims, std::string const& texturePath);
	~SkSprite();
	
	bool closest_2d(SelPoint& selP, glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter);
	// will have to find another way to drag then the geometry drag, I think it would be ridiculous to solve a system for this (the solve would be fast but still)
	bool closest_2d_draggable(SkExpSelPoint& selP, glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter);
	int selection_rank() { return 1; }
	void move(glm::vec2 from, glm::vec2 to, glm::vec2 pixel_move);

	glm::vec2 pixelOffset() { return mPixelOffset; }
	void set_pixelOffset(glm::vec2 offset);

	glm::vec2 pos() { return { mPos[0]->eval(), mPos[1]->eval() }; }
	void set(glm::vec2 pos_);
protected:
	void init_impl();
	void draw_impl(Camera* cam, int frame, draw_type type = draw_type::ALL);
	void graphicUpdate_impl();
};

#endif
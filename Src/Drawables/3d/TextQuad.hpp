
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

#ifndef TEXTQUAD_HPP_
#define TEXYQUAD_HPP_

#include <Drawables/Base/Drawable.hpp>
#include <memory>

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Shader;

class TextQuad : public Drawable {
public:
	enum AnchorPoints { TOPLEFT = 0, TOPRIGHT = 1, BOTTOMLEFT = 2, BOTTOMRIGHT = 3, CENTER = 4 };

private:
	glm::vec3 mColor { 0.0f, 0.0f, 0.0f };

	glm::vec3 mAnchorPos { glm::vec3(0.0f) };
	int mAnchorPoint { TOPLEFT };

	std::unique_ptr<VertexArray> mVA { nullptr };
	std::unique_ptr<VertexBuffer> mVB { nullptr };
	Shader* mShader { nullptr };

public:
	TextQuad() = default;
	TextQuad(glm::vec3 anchorPos, AnchorPoints anchorPoint, glm::vec3 color);

	inline glm::vec3 anchorPos() { return mAnchorPos; }
	inline void set_anchorPos(glm::vec3 pos) { mAnchorPos = pos; set_need_graphicUpdate(); }

	inline int anchorPoint() { return mAnchorPoint; }
	inline void set_anchorPoint(AnchorPoints anchorPoint) { mAnchorPoint = anchorPoint; set_need_redraw(); }

	inline glm::vec3 color() { return mColor; }
	inline void set_color(glm::vec3 color) { mColor = color; set_need_redraw(); }

protected:
	void init_impl();
	void draw_impl(Camera* cam, int frame, draw_type type = draw_type::ALL);
	void graphicUpdate_impl();
	
	void init_buffers();
};

#endif
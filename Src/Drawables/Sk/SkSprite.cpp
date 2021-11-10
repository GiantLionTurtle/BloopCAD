
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

#include "SkSprite.hpp"

#include <Utils/Expunge.hpp>
#include <Graphics_utils/Image.hpp>
#include <Graphics_utils/ShadersPool.hpp>
#include <Graphics_utils/TexturesPool.hpp>
#include <Graphics_utils/GLCall.hpp>
#include <Utils/Debug_util.hpp>
#include <Utils/Maths_util.hpp>
#include <Utils/Preferences.hpp>
#include <ConstraintsSolver/Expression.hpp>
#include <Graphics_utils/Camera.hpp>
#include <Graphics_utils/VertexBuffer.hpp>
#include <Graphics_utils/VertexBufferLayout.hpp>
#include <Graphics_utils/VertexArray.hpp>

bool SkSprite::kFisrstInst = true;
glm::vec3 SkSprite::kColorHovered = glm::vec3(0.0f);

SkSprite::SkSprite(Geom3d::Plane_abstr* basePlane_, glm::vec2 dims, std::string const& texturePath):
    SkIntDrawable(basePlane_),
    mTexture(nullptr),
    mDimensions(dims),
    mTexturePath(texturePath),
    mPixelOffset(0.0f, 0.0f),
	mPos({ExpVar::make(0.0f), ExpVar::make(0.0f)}),
    mVA(nullptr),
    mVB(nullptr),
    mShader(nullptr)
{
	mType != ANNOTATION;
	set_name("SkSprite");
}
SkSprite::~SkSprite()
{
	if(mInited) {
		expunge(mVA);
		expunge(mVB);
		// Shader and texture are managed by their pools
	}
}

bool SkSprite::closest_2d(SelPoint& selP, glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
{
	if(mType & filter) {
		glm::vec3 worldPos = mBasePlane->to_worldPos(pos());
		glm::vec2 screenPos = cam->world_to_screen(worldPos);
		if(std::abs(screenPos.x - cursorPos.x) <= mDimensions.x && std::abs(screenPos.y - cursorPos.y) <= mDimensions.y) {
			selP.ent = this;
			selP.dist_to_cam = glm::distance(cam->pos(), worldPos);
			return true;
		}
	}
	return false;
}
bool SkSprite::closest_2d_draggable(SkExpSelPoint& selP, glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
{
	if(mType & filter) {
		glm::vec3 worldPos = mBasePlane->to_worldPos(pos());
		glm::vec2 screenPos = cam->world_to_screen(worldPos);
		if(std::abs(screenPos.x - cursorPos.x) <= mDimensions.x && std::abs(screenPos.y - cursorPos.y) <= mDimensions.y) {
			selP.ent = this;
			return true;
		}
	}
	return false;
}

void SkSprite::move(glm::vec2 from, glm::vec2 to, glm::vec2 pixel_move)
{
	set(to);
}

void SkSprite::set(glm::vec2 pos_)
{
	mPos[0]->set(pos_.x);
	mPos[1]->set(pos_.y);
	update();
}
void SkSprite::set_pixelOffset(glm::vec2 offset)
{
	mPixelOffset = offset;
	update();
}

void SkSprite::init_impl()
{
	mTexture = TexturesPool::get_instance().get(mTexturePath);
	if(!mTexture) {
		mTexture = new Image(mTexturePath);
		TexturesPool::get_instance().add(mTexturePath, mTexture);
	}
	
	mNeed_graphicUpdate = false;
	mVA = new VertexArray();
	VertexBufferLayout layout;
	layout.add_proprety_float(3);
	layout.add_proprety_float(2);
	mVA->bind();

	pos_offseted tmpPos= { mBasePlane->to_worldPos(pos()), mPixelOffset };
	mVB = new VertexBuffer(&tmpPos.pos[0], sizeof(pos_offseted));
	mVA->add_buffer(*mVB, layout);
	mVA->unbind();

	mShader = ShadersPool::get_instance().get("fixedSizeTexturedQuad");
	if(!mShader) {
		mShader = Shader::fromFiles_ptr({
		{"Resources/Shaders/OffsetPoint.vert", GL_VERTEX_SHADER},
		{"Resources/Shaders/QuadExpand.geom", GL_GEOMETRY_SHADER}, 
		{"Resources/Shaders/TextureBacklit.frag", GL_FRAGMENT_SHADER}}); // Geometry Shader is needed because point is expanded on the gpu
		ShadersPool::get_instance().add("fixedSizeTexturedQuad", mShader);
	}
	if(kFisrstInst) {
		kColorHovered = Preferences::get_instance().get_vec3("sketchEntityColorHovered");
		Preferences::get_instance().add_callback("sketchEntityColorHovered", 
		std::function<void(glm::vec3)>([this](glm::vec3 val) { kColorHovered = val; }));
		kFisrstInst = false;
	}
}

void SkSprite::draw_impl(Camera* cam, int frame, draw_type type)
{
	mShader->bind();

	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
	if(hovered()) 
		color = kColorHovered;

	mShader->setUniform3f("u_Color", color);
	mShader->setUniform1f("u_Width", mDimensions.x);
	mShader->setUniform1f("u_Height", mDimensions.y);

	if(mShader->lastUsed() != frame) {
		mShader->setUniformMat4f("u_MVP", cam->mvp());
		mShader->setUniform2f("u_Viewport", cam->viewport());
		mShader->set_used(frame);
	}

	mTexture->bind();
	mVA->bind();

	glm::vec3 t;
	GLCall(glGetNamedBufferSubData(mVB->id(), 0, sizeof(glm::vec3), &t[0]));

	GLCall(glDrawArrays(GL_POINTS, 0, 1));

	mVA->unbind();
	mTexture->unbind();
	mShader->unbind();
}

void SkSprite::graphicUpdate_impl()
{
	pos_offseted tmpPos= { mBasePlane->to_worldPos(pos()), mPixelOffset };
	mVB->bind();
	mVB->set(&tmpPos, sizeof(pos_offseted));
	mVB->unbind();
}
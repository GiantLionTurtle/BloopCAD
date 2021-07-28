
#include "SkSprite.hpp"

#include <graphics_utils/Image.hpp>
#include <graphics_utils/ShadersPool.hpp>
#include <graphics_utils/GLCall.hpp>
#include <utils/DebugUtils.hpp>
#include <utils/mathUtils.hpp>
#include <utils/preferences.hpp>

bool SkSprite::kFisrstInst = true;
glm::vec3 SkSprite::kColorHovered = glm::vec3(0.0f);
std::map<std::string, std::shared_ptr<Texture>> SkSprite::kTextures = {};

SkSprite::SkSprite(Geom3d::plane_abstr* basePlane_, glm::vec2 dims, std::string const& texturePath):
    SkDrawable(basePlane_),
    mTexture(nullptr),
    mDimensions(dims),
    mTexturePath(texturePath),
    mPixelOffset(0.0f, 0.0f),
	mPos(0.0f, 0.0f),
    mVA(nullptr),
    mVB(nullptr),
    mShader(nullptr)
{
	mType != ANNOTATION;
	set_name("SkSprite");
}
SkSprite::~SkSprite()
{

}

void SkSprite::init()
{
	if(kTextures.find(mTexturePath) == kTextures.end()) {
		mTexture = std::make_shared<Image>(mTexturePath);
		kTextures[mTexturePath] = mTexture;
	} else {
		mTexture = kTextures[mTexturePath];
	}

	mNeed_update = false;
	mVA = new VertexArray();
	VertexBufferLayout layout;
	layout.add_proprety_float(3);
	layout.add_proprety_float(2);
	mVA->bind();

	pos_offseted tmpPos= { mBasePlane->to_worldPos(pos()), mPixelOffset };
	mVB = new VertexBuffer(&tmpPos, sizeof(pos_offseted));
	mVA->add_buffer(*mVB, layout);

	mShader = ShadersPool::get_instance().get("fixedSizeTexturedQuad");
	if(!mShader) {
		mShader = Shader::fromFiles_ptr({
		{"resources/shaders/offsetPointShader.vert", GL_VERTEX_SHADER},
		{"resources/shaders/fixedSizeQuad.geom", GL_GEOMETRY_SHADER}, 
		{"resources/shaders/annotationShader.frag", GL_FRAGMENT_SHADER}}); // Geometry Shader is needed because point is expanded on the gpu
		ShadersPool::get_instance().add("fixedSizeTexturedQuad", mShader);
	}
	if(kFisrstInst) {
		kColorHovered = preferences::get_instance().get_vec3("sketchEntityColorHovered");
		preferences::get_instance().add_callback("sketchEntityColorHovered", std::function<void(glm::vec3)>([this](glm::vec3 val) { kColorHovered = val; }));
		kFisrstInst = false;
	}
}

void SkSprite::draw(Camera_ptr cam, int frame, draw_type type)
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

	GLCall(glDrawArrays(GL_POINTS, 0, 1));

	mVA->unbind();
	mShader->unbind();
}

void SkSprite::update()
{
	if(!mInited)
		return;
	mNeed_update = false;
	pos_offseted tmpPos= { mBasePlane->to_worldPos(pos()), mPixelOffset };
	mVB->bind();
	mVB->set(&tmpPos, sizeof(pos_offseted));
	mVB->unbind();
}

SelectionPoint SkSprite::closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
{
	if(mType & filter) {
		glm::vec3 worldPos = mBasePlane->to_worldPos(pos());
		glm::vec2 screenPos = cam->world_to_screen(worldPos);
		if(std::abs(screenPos.x - cursorPos.x) <= mDimensions.x && std::abs(screenPos.y - cursorPos.y) <= mDimensions.y)
			return { this, glm::distance(cam->pos(), worldPos) };
	}
	return SelectionPoint();
}
DraggableSelectionPoint SkSprite::closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
{
	if(mType & filter) {
		glm::vec3 worldPos = mBasePlane->to_worldPos(pos());
		glm::vec2 screenPos = cam->world_to_screen(worldPos);
		if(std::abs(screenPos.x - cursorPos.x) <= mDimensions.x && std::abs(screenPos.y - cursorPos.y) <= mDimensions.y)
			return { this };
	}
	return DraggableSelectionPoint();
}

void SkSprite::move(glm::vec2 from, glm::vec2 to, glm::vec2 pixel_move)
{
	set(to);
}

void SkSprite::set(glm::vec2 pos_)
{
	mPos = pos_;
	set_need_update();
}
void SkSprite::set_pixelOffset(glm::vec2 offset)
{
	mPixelOffset = offset;
	set_need_update();
}
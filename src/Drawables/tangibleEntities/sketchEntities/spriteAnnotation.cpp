#include "spriteAnnotation.hpp"

#include <constraintsSolver/constraint.hpp>
#include <graphics_utils/Image.hpp>
#include <graphics_utils/ShadersPool.hpp>
#include <graphics_utils/GLCall.hpp>
#include <utils/DebugUtils.hpp>
#include <utils/mathUtils.hpp>
#include <utils/preferences.hpp>

bool spriteAnnotation::kFisrstInst = true;
glm::vec3 spriteAnnotation::kColorHovered = glm::vec3(0.0f);
std::map<std::string, std::shared_ptr<Texture>> spriteAnnotation::kTextures = {};

spriteAnnotation::spriteAnnotation(Geom3d::plane_abstr* basePlane_, glm::vec2 dims, std::string const& texturePath):
    sketchEntity(basePlane_, SPRITE),
    mTexture(nullptr),
    mDimensions(dims),
    mTexturePath(texturePath),
    mPixelOffset(0.0f, 0.0f),
	mPos(0.0f)
{
	set_name("spriteAnnotation");
}
spriteAnnotation::~spriteAnnotation()
{

}

void spriteAnnotation::init()
{
	if(kTextures.find(mTexturePath) == kTextures.end()) {
		mTexture = std::make_shared<Image>(mTexturePath);
		kTextures[mTexturePath] = mTexture;
	} else {
		mTexture = kTextures[mTexturePath];
	}

	mRequire_VBUpdate = false;
	mVA = std::make_shared<VertexArray>();
	VertexBufferLayout layout;
	layout.add_proprety_float(3);
	layout.add_proprety_float(2);
	mVA->bind();

	pos_offseted tmpPos= { mBasePlane->to_worldPos(pos()), mPixelOffset };
	mVB = std::make_shared<VertexBuffer>(&tmpPos, sizeof(pos_offseted));
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

void spriteAnnotation::move(glm::vec2 from, glm::vec2 to, glm::vec2 pixel_move)
{
	mPixelOffset += pixel_move;
	set_require_VBUpdate();
}

bool spriteAnnotation::in_selection_range(glm::vec2 planepos, Camera_ptr cam, glm::vec2 cursor)
{
	glm::vec4 onscreen_ndc = cam->mvp()     * glm::vec4(mBasePlane->to_worldPos(pos()), 1.0f);
	glm::vec2 onscreen(     map(onscreen_ndc.x / onscreen_ndc.w, -1.0f, 1.0f, 0.0f, cam->viewport().x),
						map(onscreen_ndc.y / onscreen_ndc.w, -1.0f, 1.0f, cam->viewport().y, 0.0f));
	onscreen += glm::vec2(mPixelOffset.x, -mPixelOffset.y);

	if(cursor.x < onscreen.x + mDimensions.x/2 && cursor.x > onscreen.x - mDimensions.x/2 &&
	cursor.y < onscreen.y + mDimensions.y/2 && cursor.y > onscreen.y - mDimensions.y/2) {
		return true;
	}
	return false;
}
void spriteAnnotation::set_pos(glm::vec2 pos_)
{
	mPos = pos_;
	set_require_VBUpdate();	
}
void spriteAnnotation::set_pixelOffset(glm::vec2 offset)
{
	mPixelOffset = offset;
	set_require_VBUpdate();
}

void spriteAnnotation::update_VB()
{
	if(!mInited)
		return;
	mRequire_VBUpdate = false;
	pos_offseted tmpPos= { mBasePlane->to_worldPos(pos()), mPixelOffset };
	mVB->bind();
	mVB->set(&tmpPos, sizeof(pos_offseted));
	mVB->unbind();
}

void spriteAnnotation::draw_impl(Camera_ptr cam, int frame)
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


#include "constraintAnnotation.hpp"

#include <constraintsSolver/constraint.hpp>
#include <graphics_utils/textures/image.hpp>
#include <graphics_utils/shadersPool.hpp>
#include <graphics_utils/GLCall.hpp>

constraintAnnotation::constraintAnnotation(glm::vec2 pos_, geom_3d::plane_abstr_ptr basePlane_, int constraintType):
	sketchEntity(basePlane_, -1),
	mType(constraintType),
	mTexture(nullptr),
	mWidth(20),
	mHeight(20),
	mPos(pos_)
{

}
constraintAnnotation::~constraintAnnotation()
{

}

void constraintAnnotation::init()
{
	if(kTextures.find(mType) == kTextures.end()) {
		switch(mType) {
		case constraint_abstract::COINCIDENCE:
			mTexture = std::make_shared<image>("resources/textures/images/icons/sketch/constraints/coincidence.png");
			break;
		case constraint_abstract::HORIZONTALITY:
			mTexture = std::make_shared<image>("resources/textures/images/icons/sketch/constraints/horizontality.png");
			break;
		case constraint_abstract::VERTICALITY:
			mTexture = std::make_shared<image>("resources/textures/images/icons/sketch/constraints/verticality.png");
			break;
		case constraint_abstract::PARALLELISM:
			mTexture = std::make_shared<image>("resources/textures/images/icons/sketch/constraints/parallelism.png");
			break;
		case constraint_abstract::PERPENDICULARITY:
			mTexture = std::make_shared<image>("resources/textures/images/icons/sketch/constraints/perpendicularity.png");
			break;
		default:
			mTexture = std::make_shared<image>("resources/textures/images/icons/icon.png");
			break;
		}
		kTextures[mType] = mTexture;
	} else {
		mTexture = kTextures[mType];
	}

	mRequire_VBUpdate = false;
	mVA = std::make_shared<vertexArray>();
	vertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();

	glm::vec3 tmpPos = mBasePlane->to_worldPos(mPos + mWorldOffset);
	mVB = std::make_shared<vertexBuffer>(&tmpPos[0], sizeof(glm::vec3));
	mVA->add_buffer(*mVB, layout);

	mShader = shadersPool::get_instance().get("fixedSizeTexturedQuad");
	if(!mShader) {
		mShader = shader::fromFiles_ptr({
		{"resources/shaders/pointShader.vert", GL_VERTEX_SHADER},
		{"resources/shaders/fixedSizeQuad.geom", GL_GEOMETRY_SHADER}, 
		{"resources/shaders/textureShader.frag", GL_FRAGMENT_SHADER}}); // Geometry shader is needed because point is expanded on the gpu
		shadersPool::get_instance().add("fixedSizeTexturedQuad", mShader);
	}
}

glm::vec2 constraintAnnotation::pos()
{
	return mPos;
}
void constraintAnnotation::set_pos(glm::vec2 const& pos_)
{
	mPos = pos_;
	mRequire_VBUpdate = true;
}

void constraintAnnotation::set_pixelOffset(glm::vec2 const& offset)
{
	mPixelOffset = offset;
}

void constraintAnnotation::update_VB()
{
	if(!mInited)
		return;
	mRequire_VBUpdate = false;
	glm::vec3 tmpPos = mBasePlane->to_worldPos(mPos + mWorldOffset);
	mVB->bind();
	mVB->set(&tmpPos[0], sizeof(glm::vec3) * 1);
	mVB->unbind();
}

void constraintAnnotation::draw_impl(camera_ptr cam, int frame)
{
	if(mRequire_VBUpdate) // very sketch
		update_VB();
		
	mShader->bind();
	// glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f);

	// mShader->setUniform3f("u_Color", color);
	mShader->setUniform1f("u_Width", 40);
	mShader->setUniform1f("u_Height", 40);
	mShader->setUniform2f("u_PixelOffset", mPixelOffset);

	if(mShader->lastUsed() != frame) {
		mShader->setUniformMat4f("u_MVP", cam->mvp());
		mShader->setUniform2f("u_Viewport", cam->viewport());
		mShader->set_used(frame);
	}

	mTexture->bind();
	mVA->bind();

	GLCall(glDrawArrays(GL_POINTS, 0, 1)); // No indexing needed, a line only has one vertex

	mVA->unbind();
	mShader->unbind();
}



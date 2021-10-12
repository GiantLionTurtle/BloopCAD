
#include "fixedSizeTexturedQuad.hpp"

#include "image.hpp"
#include <graphics_utils/shadersPool.hpp>
#include <graphics_utils/vertexBufferLayout.hpp>
#include <graphics_utils/GLCall.hpp>

fixedSizeTexturedQuad::fixedSizeTexturedQuad(glm::vec3 pos_, unsigned int px_width, unsigned int px_height, texture* texture_, bool owns_texture):
	mTexture(texture_),
	mRequire_VBUpdate(false),
	mWidth(px_width),
	mHeight(px_height),
	mPos(pos_)
{
	init();
}
fixedSizeTexturedQuad::fixedSizeTexturedQuad(glm::vec3 pos_, unsigned int px_width, unsigned int px_height, std::string const& filepath):
	mTexture(new image(filepath)),
	mWidth(px_width),
	mHeight(px_height),
	mPos(pos_)
{
	init();
}
fixedSizeTexturedQuad::~fixedSizeTexturedQuad()
{

}

void fixedSizeTexturedQuad::init()
{
	mRequire_VBUpdate = false;
	mVA = new vertexArray();
	vertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();

	mVB = new vertexBuffer(&mPos[0], sizeof(glm::vec3));
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

glm::vec3 fixedSizeTexturedQuad::pos()
{
	return mPos;
}
void fixedSizeTexturedQuad::set_pos(glm::vec3 const& pos_)
{
	mPos = pos_;
	mRequire_VBUpdate = true;
}

void fixedSizeTexturedQuad::update_VB()
{
	mRequire_VBUpdate = false;
}

void fixedSizeTexturedQuad::draw(camera_ptr cam, int frame)
{
	if(mRequire_VBUpdate) { // very sketch
		update_VB();
	}
	mShader->bind();
	// glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f);

	// mShader->setUniform3f("u_Color", color);
	mShader->setUniform1f("u_Width", 20);
	mShader->setUniform1f("u_Height", 20);

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
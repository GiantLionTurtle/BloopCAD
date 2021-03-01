
#ifndef FIXEDSIZETEXTUREDQUAD_HPP_
#define FIXEDSIZETEXTUREDQUAD_HPP_

#include "texture.hpp"
#include <graphics_utils/shader.hpp>
#include <graphics_utils/vertexArray.hpp>
#include <graphics_utils/vertexBuffer.hpp>
#include <graphics_utils/camera.hpp>

#include <string>
#include <memory>

class fixedSizeTexturedQuad {
private:
	bool mOwns_texture;
	texture* mTexture;
	int mWidth, mHeight;
	glm::vec3 mPos;

	std::shared_ptr<shader> mShader;
	vertexArray* mVA;
	vertexBuffer* mVB;
	bool mRequire_VBUpdate;
public: 
	fixedSizeTexturedQuad(glm::vec3 pos_, unsigned int px_width, unsigned int px_height, texture* texture_, bool owns_texture = true);
	fixedSizeTexturedQuad(glm::vec3 pos_, unsigned int px_width, unsigned int px_height, std::string const& filepath);
	~fixedSizeTexturedQuad();

	void init();
	
	glm::vec3 pos();
	void set_pos(glm::vec3 const& pos_);

	void update_VB();

	void draw(camera_ptr cam, int frame);
};

#endif
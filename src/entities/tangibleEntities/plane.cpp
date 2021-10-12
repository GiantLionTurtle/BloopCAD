
#include "plane.hpp"

#include <graphics_utils/GLCall.hpp>
#include <graphics_utils/shadersPool.hpp>

plane::plane(plane_abstr const& plane_):
	plane_abstr(plane_)
{
	// init();
}

void plane::init()
{
	set_name("plane");
	init_buffers(); // Set all the vertices to the right values
	
	mVB = std::shared_ptr<vertexBuffer>(new vertexBuffer(mVertices, sizeof(glm::vec3) * 4)); // Upload the vertices
	mVA = std::shared_ptr<vertexArray>(new vertexArray());

	vertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();
	mVA->add_buffer(*mVB.get(), layout);
	mIB = std::shared_ptr<indexBuffer>(new indexBuffer(mIndices, 6));

	// Create the shaders
	mShader = shadersPool::get_instance().get("plane");
	if(!mShader) {
		mShader = shader::fromFiles_ptr("resources/shaders/planeShader.vert", "resources/shaders/planeShader.frag");
		shadersPool::get_instance().add("plane", mShader);
	}
}

void plane::update_VB()
{
	init_buffers();
	mVB->bind();
	mVB->set(mVertices, sizeof(glm::vec3) * 4);
	mVB->unbind();
}

void plane::draw_impl(camera_ptr cam, int frame)
{
	// TODO: make this less sketch?
	// GLCall(glDisable(GL_DEPTH_TEST)); // Disable the depth buffer to draw the whole quad, even if it is hidden by another semi-transparent quad
	mShader->bind();

	glm::vec3 color;
	if(dist_to_point_signed(cam->pos()) >= 0) {
		color = mColorA;
	} else {
		color = mColorB;
	}
	if(selected()) 
		color += glm::vec3(0.0f, 0.0f, 0.3f); // Make it blue-er if selected
	mShader->setUniform4f("u_Color", color.r, color.g, color.b, hovered() ? 0.5 : 0.3);

	if(mShader->lastUsed() != frame) {
		mShader->setUniformMat4f("u_MVP", cam->mvp());
		mShader->set_used(frame);
	}
	
	mVA->bind();
	mIB->bind();

	GLCall(glDrawElements(GL_TRIANGLES, mIB->count(), GL_UNSIGNED_INT, nullptr)); // Actual draw call

	// Clean up
	mIB->unbind();
	mVA->unbind();
	mShader->unbind();
	// GLCall(glEnable(GL_DEPTH_TEST));
}

float plane::selection_depth(camera_ptr cam, glm::vec2 cursor_pos)
{
	glm::vec3 pos = cam->pos();
	glm::vec3 inter = line_intersection(pos, cam->cast_ray(cursor_pos));
	glm::vec2 plane_pos = to_planePos(inter);
	if(std::abs(plane_pos.x) <= glm::length(mV) && std::abs(plane_pos.y) <= glm::length(mW)) {
		return glm::length(pos-inter);
	}
	return -1.0f;
}

void plane::init_buffers()
{
	// Set all vertices in anti-clockwise fashion
	mVertices[0] = mOrigin + mV + mW;
	mVertices[1] = mOrigin - mV + mW;
	mVertices[2] = mOrigin - mV - mW;
	mVertices[3] = mOrigin + mV - mW;

	mIndices[0] = 0;
	mIndices[1] = 1;
	mIndices[2] = 2; // First triangle
	mIndices[3] = 2;
	mIndices[4] = 3;
	mIndices[5] = 0; // Second triangle
}
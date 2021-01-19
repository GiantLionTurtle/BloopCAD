
#include "sketchCircle.hpp"

#include <graphics_utils/GLCall.hpp>
#include <graphics_utils/shadersPool.hpp>

sketchCircle::sketchCircle(circle_abstract const& baseCircle, plane_abstract_ptr basePlane_):
	circle_abstract(baseCircle),
	sketchElement(basePlane_)
{
	set_name("sketchCircle");
	init_buffers(); // Set all the vertices to the right values
	
	mVB = std::shared_ptr<vertexBuffer>(new vertexBuffer(mVertices, sizeof(glm::vec3) * CIRCLE_RES)); // Upload the vertices
	mVA = std::shared_ptr<vertexArray>(new vertexArray());

	vertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();
	mVA->add_buffer(*mVB.get(), layout);
	mIB = std::shared_ptr<indexBuffer>(new indexBuffer(mIndices, 2*CIRCLE_RES));

	// // Create the shader
	// mShader = shadersPool::get_instance().get("circle");
	// if(!mShader) {
	// 	mShader = shader::fromFiles_ptr("resources/shaders/circleShader.vert", "resources/shaders/circleShader.frag");
	// 	shadersPool::get_instance().add("circle", mShader);
	// }

	mShader = shadersPool::get_instance().get("line");
	if(!mShader) {
		mShader = shader::fromFiles_ptr({
		{"resources/shaders/lineShader.vert", GL_VERTEX_SHADER},
		{"resources/shaders/lineShader.geom", GL_GEOMETRY_SHADER}, 
		{"resources/shaders/lineShader.frag", GL_FRAGMENT_SHADER}}); // Geometry shader is needed because line is expanded on the gpu
		shadersPool::get_instance().add("line", mShader);
	}
	

	std::shared_ptr<point> center_as_drawable = std::dynamic_pointer_cast<point>(mCenter);
	if(!center_as_drawable) {
		center_as_drawable = std::shared_ptr<point>(new point(mCenter));
		mCenter = center_as_drawable;
	}

	add(center_as_drawable);
}

void sketchCircle::update_VB()
{
	init_buffers();
	mVB->bind();
	mVB->set(mVertices, sizeof(glm::vec3) * CIRCLE_RES);
	mVB->unbind();
}

void sketchCircle::draw_impl(camera_ptr cam, int frame)
{
	mShader->bind();

	glm::vec4 p1 = cam->mvp() * glm::vec4(mVertices[0], 1.0f);
	glm::vec2 p1_v = p1 /= p1.w;
	glm::vec4 p2 = cam->mvp() * glm::vec4(mVertices[3], 1.0f);
	glm::vec2 p2_v = p2 /= p2.w;
	glm::vec4 c = cam->mvp() * glm::vec4(mCenter->pos_val(), 1.0f);
	c /= c.w;

	p1_v = (p1_v + glm::vec2(1.0f, 1.0f)) / 2.0f * cam->viewport();
	p2_v = (p2_v + glm::vec2(1.0f, 1.0f)) / 2.0f * cam->viewport();

	float len = glm::length(p2_v - p1_v) / 2.0f;

	glm::vec2 sentcent = (glm::vec2(c) + glm::vec2(1.0f, 1.0f)) / 2.0f * cam->viewport();

	mShader->setUniform1f("u_R2", len * len);
	mShader->setUniform2f("u_Center", sentcent);
	mShader->setUniform4f("u_Color", glm::vec4(mColor, 1.0f));

	if(mShader->lastUsed() != frame) {
		mShader->setUniformMat4f("u_MVP", cam->mvp());
		mShader->set_used(frame);
	}
	
	mVA->bind();
	mIB->bind();

	GLCall(glDrawElements(GL_LINES, mIB->count(), GL_UNSIGNED_INT, nullptr)); // Actual draw call

	// Clean up
	mIB->unbind();
	mVA->unbind();
	mShader->unbind();
}

float sketchCircle::selection_depth(camera_ptr cam, glm::vec2 cursor_pos)
{

	return -1.0f;
}

void sketchCircle::init_buffers()
{
	// // Set all vertices in anti-clockwise fashion
	// mVertices[0] = mCenter->pos_val() + glm::vec3( mRadius->val(),  mRadius->val(), 0.0f);
	// mVertices[1] = mCenter->pos_val() + glm::vec3(-mRadius->val(),  mRadius->val(), 0.0f);
	// mVertices[2] = mCenter->pos_val() + glm::vec3(-mRadius->val(), -mRadius->val(), 0.0f);
	// mVertices[3] = mCenter->pos_val() + glm::vec3( mRadius->val(), -mRadius->val(), 0.0f);

	// mIndices[0] = 0;
	// mIndices[1] = 1;
	// mIndices[2] = 2; // First triangle
	// mIndices[3] = 2;
	// mIndices[4] = 3;
	// mIndices[5] = 0; // Second triangle

	for(int i = 0; i < CIRCLE_RES; ++i) {
		float angle = M_PI * 2.0f / (float)CIRCLE_RES * (float)i;
		mVertices[i] = mBasePlane->point_2d_to_3d(at(angle));
		mIndices[2*i] = i;
		mIndices[2*i+1] = i+1;
	}
	mIndices[2*CIRCLE_RES-1] = 0;
}
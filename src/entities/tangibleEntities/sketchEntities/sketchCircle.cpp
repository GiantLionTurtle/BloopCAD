
#include "sketchCircle.hpp"
#include "sketchPoint.hpp"

#include <graphics_utils/GLCall.hpp>
#include <graphics_utils/shadersPool.hpp>
#include <utils/mathUtils.hpp>

sketchCircle::sketchCircle(circle_abstr const& baseCircle, geom_3d::plane_abstr_ptr basePlane_):
	circle_abstr(baseCircle),
	sketchEntity(basePlane_)
{
	init();
}

void sketchCircle::init()
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
	

	std::shared_ptr<sketchPoint> center_as_drawable = std::dynamic_pointer_cast<sketchPoint>(mCenter);
	if(!center_as_drawable) {
		center_as_drawable = std::shared_ptr<sketchPoint>(new sketchPoint(mCenter->var(), mBasePlane));
		mCenter = center_as_drawable;
	}

	add(center_as_drawable);
}

void sketchCircle::move(glm::vec2 from, glm::vec2 to)
{
	set_radius(glm::length(to - mCenter->vec()));
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
	if(mRequire_VBUpdate)
		update_VB();

	mShader->bind();
	glm::vec4 color = glm::vec4(mColor, 1.0f);
	if(hovered()) {
		color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	} else if(selected()) {
		color = glm::vec4(0.01f, 0.70f, 0.99f, 1.0f);
	}
	mShader->setUniform4f("u_Color", color);
	
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

void sketchCircle::post_set_behavior()
{
	mRequire_VBUpdate = true;
	set_require_redraw();
}

float sketchCircle::selection_depth(camera_ptr cam, glm::vec2 cursor_pos)
{
	glm::vec3 inter = mBasePlane->line_intersection(cam->pos(), cam->cast_ray(cursor_pos));
	glm::vec2 on_plane = mBasePlane->to_planePos(inter);
	glm::vec4 closest_screen(mBasePlane->to_worldPos(closest_to_point(on_plane)), 1.0f);
	closest_screen = cam->mvp() * closest_screen;
	closest_screen /= closest_screen.w;
	glm::vec2 on_screen_pix(map(closest_screen.x, -1.0f, 1.0f, 0.0f, cam->viewport().x), 
							map(closest_screen.y, -1.0f, 1.0f, cam->viewport().y, 0.0f));
	if(glm::length2(cursor_pos - on_screen_pix) < 50) {
		return glm::length(cam->pos() - inter);
	}
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
		float t = (float)i / CIRCLE_RES;
		mVertices[i] = mBasePlane->to_worldPos(at(t));
		mIndices[2*i] = i;
		mIndices[2*i+1] = i+1;
	}
	mIndices[2*CIRCLE_RES-1] = 0;
}
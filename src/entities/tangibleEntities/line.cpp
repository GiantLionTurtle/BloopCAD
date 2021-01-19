
#include "line.hpp"

#include <graphics_utils/GLCall.hpp>
#include <graphics_utils/shadersPool.hpp>
#include <utils/mathUtils.hpp>

line::line(line_abstract const& baseLine):
	line_abstract(baseLine)
{	
	std::shared_ptr<point> pointA_as_drawable = std::dynamic_pointer_cast<point>(mPointA);
	std::shared_ptr<point> pointB_as_drawable = std::dynamic_pointer_cast<point>(mPointB);
	if(!pointA_as_drawable) {
		pointA_as_drawable = std::shared_ptr<point>(new point(baseLine.pointA()));
		mPointA = pointA_as_drawable;
	}
	if(!pointB_as_drawable) {
		pointB_as_drawable = std::shared_ptr<point>(new point(baseLine.pointB()));
		mPointB = pointB_as_drawable;
	}

	set_name("line");
	mVA = std::shared_ptr<vertexArray>(new vertexArray());
	vertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();

	glm::vec3 tmp[2] = {mPointA->pos_val(), mPointB->pos_val()};
	mVB = std::shared_ptr<vertexBuffer>(new vertexBuffer(&tmp[0], sizeof(glm::vec3) * 2));
	mVA->add_buffer(*mVB.get(), layout);

	mShader = shadersPool::get_instance().get("line");
	if(!mShader) {
		mShader = shader::fromFiles_ptr({
		{"resources/shaders/lineShader.vert", GL_VERTEX_SHADER},
		{"resources/shaders/lineShader.geom", GL_GEOMETRY_SHADER}, 
		{"resources/shaders/lineShader.frag", GL_FRAGMENT_SHADER}}); // Geometry shader is needed because line is expanded on the gpu
		shadersPool::get_instance().add("line", mShader);
	}
	
	add(pointA_as_drawable);
	add(pointB_as_drawable);
}

void line::move(glm::vec3 from, glm::vec3 to)
{
	glm::vec3 delta = to - from;
	mPointA->set(mPointA->pos_val() + delta);
	mPointB->set(mPointB->pos_val() + delta);
}
void line::set_constant()
{
	mPointA->set_constant();
	mPointB->set_constant();
}
void line::set_tmpConstant(bool const_)
{
	mPointA->set_tmpConstant(const_);
	mPointB->set_tmpConstant(const_);
}

void line::update_VB()
{
	mRequire_VBUpdate = false;
	glm::vec3 tmp[2] = {mPointA->pos_val(), mPointB->pos_val()};
	mVB->bind();
	mVB->set(&tmp[0], sizeof(glm::vec3) * 2);
	mVB->unbind();
}

void line::draw_impl(camera_ptr cam, int frame)
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
	mShader->setUniform1f("u_LineWidth", 5);
	mShader->setUniform1f("u_Feather", 0.6);

	if(mShader->lastUsed() != frame) {
		mShader->setUniformMat4f("u_MVP", cam->mvp());
		mShader->setUniform2f("u_Viewport", cam->viewport());
		mShader->set_used(frame);
	}

	mVA->bind();

	GLCall(glDrawArrays(GL_LINES, 0, 2)); // No indexing needed, a line only has two vertices

	mVA->unbind();
	mShader->unbind();
}

float line::selection_depth(camera_ptr cam, glm::vec2 cursor_pos)
{
	glm::vec4 screenPos_a = cam->mvp() * glm::vec4(mPointA->pos_val(), 1.0f);
	glm::vec3 screenPos_a_2d(	map(screenPos_a.x / screenPos_a.w, -1.0f, 1.0f, 0.0f, cam->viewport().x),
								map(screenPos_a.y / screenPos_a.w, -1.0f, 1.0f, cam->viewport().y, 0.0f),
								0.0f);

	glm::vec4 screenPos_b = cam->mvp() * glm::vec4(mPointB->pos_val(), 1.0f);
	glm::vec3 screenPos_b_2d(	map(screenPos_b.x / screenPos_b.w, -1.0f, 1.0f, 0.0f, cam->viewport().x),
								map(screenPos_b.y / screenPos_b.w, -1.0f, 1.0f, cam->viewport().y, 0.0f), 
								0.0f);

	line_abstract screen_line(screenPos_a_2d, screenPos_b_2d);
	float line_point = 0;
	if(screen_line.closest_point(glm::vec3(cursor_pos, 0.0f), line_point)->dist2(glm::vec3(cursor_pos, 0.0f)) < 25) {
		return glm::length(cam->pos() - plane_abstract(at(line_point)->pos_val(), cam->right(), cam->up()).line_intersection(cam->pos(), cam->cast_ray(cursor_pos)));		
	}
	return -1.0f;
}

void line::post_set_update()
{
	set_require_redraw();
	mRequire_VBUpdate = true;
}
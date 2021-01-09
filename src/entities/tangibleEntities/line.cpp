
#include "line.hpp"

#include <graphics_utils/GLCall.hpp>
#include <graphics_utils/shadersPool.hpp>

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

	glm::vec3 tmp[2] = {mPointA->pos_vec(), mPointB->pos_vec()};
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

void line::set_pointA(std::shared_ptr<point_abstract> ptA)
{
	mRequire_VBUpdate = true;
	set_require_redraw();
	mPointA->set_pos(ptA);
}
void line::set_pointB(std::shared_ptr<point_abstract> ptB)
{
	mRequire_VBUpdate = true;
	set_require_redraw();
	mPointB->set_pos(ptB);
}

void line::update_VB()
{
	mRequire_VBUpdate = false;
	glm::vec3 tmp[2] = {mPointA->pos_vec(), mPointB->pos_vec()};
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
		color = glm::vec4(0.0f, 0.8, 0.2, 1.0f);
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

void line::draw_selection_impl(camera_ptr cam, int frame)
{
	mShader->bind();
	mShader->setUniform4f("u_Color", mSelectionColor.r, mSelectionColor.g, mSelectionColor.b, 1.0f);
	mShader->setUniform1f("u_LineWidth", 5);
	mShader->setUniform1f("u_Feather", 0.0);

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

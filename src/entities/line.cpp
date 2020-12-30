
#include "line.hpp"

#include <graphics_utils/GLCall.hpp>
#include <graphics_utils/shadersPool.hpp>

line::line(line_abstract const& baseLine):
	line_abstract(new point(baseLine.pointA()), new point(baseLine.pointB())),
	mVB(nullptr),
	mVA(nullptr)
{	
	set_name("line");	
	mVA = std::shared_ptr<vertexArray>(new vertexArray());
	vertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();

	glm::vec3 tmp[2] = {mPointA->pos(), mPointB->pos()};
	mVB = std::shared_ptr<vertexBuffer>(new vertexBuffer(tmp, sizeof(glm::vec3) * 2));
	mVA->add_buffer(*mVB.get(), layout);

	mShader = shadersPool::get_instance().get("line");
	if(!mShader) {
		mShader = shader::fromFiles_ptr({
		{"resources/shaders/lineShader.vert", GL_VERTEX_SHADER},
		{"resources/shaders/lineShader.geom", GL_GEOMETRY_SHADER}, 
		{"resources/shaders/lineShader.frag", GL_FRAGMENT_SHADER}}); // Geometry shader is needed because line is expanded on the gpu
		shadersPool::get_instance().add("line", mShader);
	}
	
	add(std::shared_ptr<entity>(dynamic_cast<point*>(mPointA)));
	add(std::shared_ptr<entity>(dynamic_cast<point*>(mPointB)));
}

void line::set_pointA(point_abstract ptA)
{
	mRequire_VBUpdate = true;
	set_require_redraw();
	mPointA->set_pos(ptA);
}
void line::set_pointB(point_abstract ptB)
{
	mRequire_VBUpdate = true;
	set_require_redraw();
	mPointB->set_pos(ptB);
}

void line::update_VB()
{
	glm::vec3 tmp[2] = {mPointA->pos(), mPointB->pos()};
	mVB->bind();
	mVB->set(tmp, sizeof(glm::vec3) * 2);
	mVB->unbind();
}

void line::draw_impl(std::shared_ptr<camera> cam, int frame)
{
	if(mRequire_VBUpdate) {
		update_VB();
		mRequire_VBUpdate = false;
	}
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

void line::draw_selection_impl(std::shared_ptr<camera> cam, int frame)
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

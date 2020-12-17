
#include "line.hpp"

#include <graphics_utils/GLCall.hpp>
#include <graphics_utils/shadersPool.hpp>

line::line(line_abstract const& baseLine):
    line_abstract(baseLine),
	mVB(nullptr),
	mVA(nullptr)
{	

	mVA = std::shared_ptr<vertexArray>(new vertexArray());
	vertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();

    glm::vec3 tmp[2] = {mPointA, mPointB};
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
}

void line::update_VB()
{
    glm::vec3 tmp[2] = {mPointA, mPointB};
	mVB->bind();
	mVB->set(tmp, sizeof(glm::vec3) * 2);
	mVB->unbind();
	set_require_redraw();
}

void line::draw_impl(std::shared_ptr<camera> cam, int frame)
{
	mShader->bind();
	glm::vec4 color = glm::vec4(mColor, 1.0f);
	if(hovered()) {
		color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	} else if(selected()) {
		color = glm::vec4(0.0f, 0.8, 0.2, 1.0f);
	}
	mShader->setUniform4f("u_Color", color);
    mShader->setUniform1f("u_LineWidth", 30);
	mShader->setUniform1f("u_Feather", 0.4);

	if(mShader->lastUsed() != frame) {
		mShader->setUniformMat4f("u_MVP", cam->mvp());
		mShader->setUniform2f("u_Viewport", cam->viewport().get());
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
    mShader->setUniform1f("u_LineWidth", 30);
	mShader->setUniform1f("u_Feather", 1.0);

	if(mShader->lastUsed() != frame) {
		mShader->setUniformMat4f("u_MVP", cam->mvp());
		mShader->setUniform2f("u_Viewport", cam->viewport().get());
		mShader->set_used(frame);
	}
	
	mVA->bind();

	GLCall(glDrawArrays(GL_LINES, 0, 2)); // No indexing needed, a line only has two vertices

    mVA->unbind();
	mShader->unbind();
}


#include "point.hpp"

#include <graphics_utils/GLCall.hpp>
#include <graphics_utils/shadersPool.hpp>

point::point(point_abstract const& basePoint):
	point_abstract(basePoint)
{	
	create();
}
point::point(std::shared_ptr<point_abstract> basePoint):
	point_abstract(basePoint->pos_var())
{
	create();
}

void point::set_pos(glm::vec3 pos)
{
	mRequire_VBUpdate = true;
	set_require_redraw();
	mPos->set(pos);
}
void point::set_pos(point_abstract const& other)
{
	mRequire_VBUpdate = true;
	set_require_redraw();
	mPos->set(other.pos_vec());
}

void point::update_VB()
{
	mVB->bind();
	glm::vec3 pos_tmp = pos_vec();
	mVB->set(&pos_tmp[0], sizeof(glm::vec3));
	mVB->unbind();
	set_require_redraw();
	if(mParent)
            mParent->notify_childUpdate();
}

void point::draw_impl(camera_ptr cam, int frame)
{
	if(mRequire_VBUpdate) { // very sketch
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
	mShader->setUniform1f("u_Diameter", 5);
	// mShader->setUniform1f("u_Feather", 0.6);

	if(mShader->lastUsed() != frame) {
		mShader->setUniformMat4f("u_MVP", cam->mvp());
		mShader->setUniform2f("u_Viewport", cam->viewport());
		mShader->set_used(frame);
	}

	mVA->bind();

	GLCall(glDrawArrays(GL_POINTS, 0, 1)); // No indexing needed, a line only has one vertex

	mVA->unbind();
	mShader->unbind();
}

void point::draw_selection_impl(camera_ptr cam, int frame)
{
	mShader->bind();
	mShader->setUniform4f("u_Color", mSelectionColor.r, mSelectionColor.g, mSelectionColor.b, 1.0f);
	mShader->setUniform1f("u_Diameter", 5.0f / 0.6f);
	// mShader->setUniform1f("u_Feather", 0.0);

	if(mShader->lastUsed() != frame) {
		mShader->setUniformMat4f("u_MVP", cam->mvp());
		mShader->setUniform2f("u_Viewport", cam->viewport());
		mShader->set_used(frame);
	}
	
	mVA->bind();

	GLCall(glDrawArrays(GL_POINTS, 0, 1)); // No indexing needed, a line only has two vertices

	mVA->unbind();
	mShader->unbind();
}

void point::create()
{
	mVA = std::shared_ptr<vertexArray>(new vertexArray());
	vertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();

	glm::vec3 pos_tmp = pos_vec();
	mVB = std::shared_ptr<vertexBuffer>(new vertexBuffer(&pos_tmp[0], sizeof(glm::vec3)));
	mVA->add_buffer(*mVB.get(), layout);

	mShader = shadersPool::get_instance().get("point");
	if(!mShader) {
		mShader = shader::fromFiles_ptr({
		{"resources/shaders/pointShader.vert", GL_VERTEX_SHADER},
		{"resources/shaders/pointShader.geom", GL_GEOMETRY_SHADER}, 
		{"resources/shaders/pointShader.frag", GL_FRAGMENT_SHADER}}); // Geometry shader is needed because point is expanded on the gpu
		shadersPool::get_instance().add("point", mShader);
	}
	set_name("point");
}

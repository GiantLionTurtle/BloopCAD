
#include "point.hpp"

#include <entities/geometry/plane_abstract.hpp>
#include <graphics_utils/GLCall.hpp>
#include <graphics_utils/shadersPool.hpp>
#include <utils/mathUtils.hpp>

point::point(point_abstract const& basePoint):
	point_abstract(basePoint)
{	
	create();
}
point::point(point_abstract_ptr basePoint):
	point_abstract(basePoint->pos())
{
	create();
}

void point::move(glm::vec3 from, glm::vec3 to) 
{
	mPos->set(to);
}
void point::set_constant()
{
	point_abstract::set_constant();
}
void point::set_tmpConstant(bool const_)
{
	point_abstract::set_tmpConstant(const_);
}

void point::update_VB()
{
	mVB->bind();
	glm::vec3 pos_tmp = pos_val();
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
		color = glm::vec4(0.01f, 0.70f, 0.99f, 1.0f);
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

float point::selection_depth(camera_ptr cam, glm::vec2 cursor_pos)
{
	glm::vec4 screenPos = cam->mvp() * glm::vec4(pos_val(), 1.0f);
	glm::vec2 screenPos_2d(	map(screenPos.x / screenPos.w, -1.0f, 1.0f, 0.0f, cam->viewport().x),
							map(screenPos.y / screenPos.w, -1.0f, 1.0f, cam->viewport().y, 0.0f));
	
	if(glm::length(screenPos_2d - cursor_pos) < 5) {
		return glm::length(cam->pos() - plane_abstract(pos_val(), cam->right(), cam->up()).line_intersection(cam->pos(), cam->cast_ray(cursor_pos)));
	}
	return -1.0f;
}

void point::post_set_update()
{
	set_require_redraw();
	mRequire_VBUpdate = true;
}

void point::create()
{
	mVA = std::shared_ptr<vertexArray>(new vertexArray());
	vertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();

	glm::vec3 pos_tmp = pos_val();
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

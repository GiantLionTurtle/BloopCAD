
#include "Point.hpp"

#include <Geom/Geom3d/Plane_abstr.hpp>
#include <graphics_utils/GLCall.hpp>
#include <graphics_utils/ShadersPool.hpp>
#include <utils/mathUtils.hpp>

Point::Point(glm::vec3 const& basePoint):
	mVB(nullptr),
	mVA(nullptr),
	mShader(nullptr)
{
	// glm::vec3 pos = basePoint.pos();
	mX = ExpVar::make(basePoint.x);
	mY = ExpVar::make(basePoint.y);
	mZ = ExpVar::make(basePoint.z);

	mType |= Drawable_types::POINT;
}
Point::~Point()
{
	delete mVB;
	delete mVA;
}

SelPoint Point::closest(glm::vec2 cursor, Camera* cam, int filter)
{
	if(!(mType & filter))
		return SelPoint();
	
	// NOTE: 50 is arbitrary and should be an option eventually
	if(glm::distance2(cam->world_to_screen(pos()), cursor) < 50)
		return SelPoint(this, glm::distance(pos(), cam->pos()));
	return SelPoint();
}

void Point::move(glm::vec3 from, glm::vec3 to) 
{
	set(to);
}

glm::vec3 Point::pos() const
{
	return glm::vec3(mX->eval(), mY->eval(), mZ->eval());
}
void Point::set(glm::vec3 const& p)
{

}
void Point::set_constant()
{
	mX->set_is_coef(true);
	mY->set_is_coef(true);
}
void Point::set_tmpConstant(bool const_)
{
	mX->set_as_coef();
	mY->set_as_coef();
}


void Point::init_impl()
{
	mVA = new VertexArray();
	VertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();

	glm::vec3 pos_tmp = pos();
	mVB = new VertexBuffer(&pos_tmp[0], sizeof(glm::vec3));
	mVA->add_buffer(*mVB, layout);

	mShader = ShadersPool::get_instance().get("point");
	if(!mShader) {
		mShader = Shader::fromFiles_ptr({
		{"resources/shaders/pointShader.vert", GL_VERTEX_SHADER},
		{"resources/shaders/pointShader.geom", GL_GEOMETRY_SHADER}, 
		{"resources/shaders/pointShader.frag", GL_FRAGMENT_SHADER}}); // Geometry Shader is needed because point is expanded on the gpu
		ShadersPool::get_instance().add("point", mShader);
	}
	set_name("point");
}

void Point::draw_impl(Camera_ptr cam, int frame, draw_type type)
{
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
void Point::update_impl()
{
	mVB->bind();
	glm::vec3 pos_tmp = pos();
	mVB->set(&pos_tmp[0], sizeof(glm::vec3));
	mVB->unbind();
	set_need_redraw();
	if(mParent)
		mParent->notify(this, UPDATED, true);
}

#include "Plane.hpp"

#include <graphics_utils/GLCall.hpp>
#include <graphics_utils/ShadersPool.hpp>

Plane::Plane(plane_abstr const& plane_):
	plane_abstr(plane_),
	mVA(nullptr),
	mVB(nullptr),
	mShader(nullptr)
{
	mType |= Drawable_types::PLANE;
}

SelectionPoint Plane::closest(glm::vec2 cursor, Camera* cam, glm::vec3 cam_ray, int filter)
{
	if(!(mType & filter))
		return SelectionPoint();
	glm::vec3 inter = line_intersection(cam->pos(), cam_ray);
	glm::vec2 plane_pos = to_planePos(inter);
	if(std::abs(plane_pos.x) <= glm::length(mV) && std::abs(plane_pos.y) <= glm::length(mW)) {
		return SelectionPoint(this, glm::length(cam->pos()-inter));
	}
	return SelectionPoint();
}

void Plane::init_impl()
{
	set_need_redraw();
	set_name("plane");
	init_buffers(); // Set all the vertices to the right values
	
	mVB = new VertexBuffer(mVertices, sizeof(glm::vec3) * 4); // Upload the vertices
	mVA = new VertexArray();

	VertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();
	mVA->add_buffer(*mVB, layout);
	mIB = new IndexBuffer(mIndices, 6);

	// Create the Shaders
	mShader = ShadersPool::get_instance().get("plane");
	if(!mShader) {
		mShader = Shader::fromFiles_ptr("resources/shaders/planeShader.vert", "resources/shaders/planeShader.frag");
		ShadersPool::get_instance().add("plane", mShader);
	}
}

void Plane::draw_impl(Camera_ptr cam, int frame, draw_type type)
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

void Plane::update_impl()
{
	init_buffers();
	mVB->bind();
	mVB->set(mVertices, sizeof(glm::vec3) * 4);
	mVB->unbind();
}


void Plane::init_buffers()
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

#include "plane.hpp"
#include "../graphics_utils/GLCall.hpp"

plane::plane(plane_abstract const& plane_, std::string const& label) :
	plane_abstract(plane_),
	mVB(nullptr),
	mVA(nullptr),
	mIB(nullptr),
	mOutlineIB(nullptr)
{
	init_buffers();
	
	mVB = std::shared_ptr<vertexBuffer>(new vertexBuffer(mVertices, sizeof(glm::vec3) * 4));
	mVA = std::shared_ptr<vertexArray>(new vertexArray());
	vertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();
	mVA->add_buffer(*mVB.get(), layout);
	mIB = std::shared_ptr<indexBuffer>(new indexBuffer(mIndices, 6));
	mShader = shader::fromFiles_ptr("resources/shaders/planeShader.vert", "resources/shaders/planeShader.frag");
	mSelectionShader = shader::fromFiles_ptr("resources/shaders/planeShader.vert", "resources/shaders/plainColor.frag");
}

plane plane::from_1Point2Vectors(glm::vec3 const& origin, glm::vec3 const& v, glm::vec3 const& w, std::string const& label)
{
	return plane(plane_abstract::from_1Point2Vectors(origin, v, w), label);
}

std::shared_ptr<plane> plane::from_1Point2Vectors_ptr(glm::vec3 const& origin, glm::vec3 const& v, glm::vec3 const& w, std::string const& label)
{
	return std::shared_ptr<plane>(new plane(plane_abstract::from_1Point2Vectors(origin, v, w), label));
}

void plane::draw(std::shared_ptr<camera> cam)
{
	// TODO: make this less sketch?
	GLCall(glDisable(GL_DEPTH_TEST));
	mShader->bind();
	glm::vec3 color;
	if(dist_signed(cam->pos_cartesian()) >= 0) {
		color = glm::vec3(0.34f, 0.17f, 0.05f);
	} else {
		color = glm::vec3(0.15f, 0.0f, 0.25f);
	}
	mShader->setUniform4f("u_Color", color.r, color.g, color.b, mHovered ? 0.7 : 0.5);
	mShader->setUniformMat4f("u_MVP", cam->projection() * cam->view());
	mVA->bind();
	mIB->bind();

	GLCall(glDrawElements(GL_TRIANGLES, mIB->count(), GL_UNSIGNED_INT, nullptr));

	mIB->unbind();
	mShader->unbind();

	GLCall(glEnable(GL_DEPTH_TEST));
}

void plane::draw_selection(std::shared_ptr<camera> cam)
{
	mSelectionShader->bind();

	mShader->setUniform3f("u_Color", mSelectionColor.r, mSelectionColor.g, mSelectionColor.b);
	mShader->setUniformMat4f("u_MVP", cam->projection() * cam->view());
	mVA->bind();
	mIB->bind();

	GLCall(glDrawElements(GL_TRIANGLES, mIB->count(), GL_UNSIGNED_INT, nullptr));

	mIB->unbind();
	mShader->unbind();
}

void plane::init_buffers()
{
	mVertices[0] = mOrigin + mV + mW;
	mVertices[1] = mOrigin - mV + mW;
	mVertices[2] = mOrigin - mV - mW;
	mVertices[3] = mOrigin + mV - mW;

	mIndices[0] = 0;
	mIndices[1] = 1;
	mIndices[2] = 2;
	mIndices[3] = 2;
	mIndices[4] = 3;
	mIndices[5] = 0;
}
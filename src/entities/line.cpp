
#include "line.hpp"
#include <graphics_utils/GLCall.hpp>

line::line(glm::vec3 ptA, glm::vec3 ptB):
    line_abstract(ptA, ptB),
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

	mShader = shader::fromFiles_ptr({
        {"resources/shaders/lineShader.vert", GL_VERTEX_SHADER},
        {"resources/shaders/lineShader.geom", GL_GEOMETRY_SHADER}, 
        {"resources/shaders/lineShader.frag", GL_FRAGMENT_SHADER}}); // Geometry shader is needed because line is expanded on the gpu
}

line line::from_2Points(glm::vec3 ptA, glm::vec3 ptB)
{
    return line(ptA, ptB);
}

std::shared_ptr<line> line::from_2Points_ptr(glm::vec3 ptA, glm::vec3 ptB)
{
    return std::shared_ptr<line>(new line(ptA, ptB));
}

void line::update_VB()
{
    glm::vec3 tmp[2] = {mPointA, mPointB};
	mVB->bind();
	mVB->set(tmp, sizeof(glm::vec3) * 2);
	mVB->unbind();
}

void line::draw_impl(std::shared_ptr<camera> cam)
{
	mShader->bind();
	mShader->setUniform4f("u_Color", mColor.r, mColor.g, mColor.b, 1.0f);
    // mShader->setUniform1f("u_AspectRatio", cam->aspectRatio().get());

	glm::mat4 mvp = (cam->projection() * cam->view() * cam->model());

	// glm::vec4 pA = mvp * glm::vec4(mPointA, 1.0f);
	// pA /= pA.w;
	// glm::vec4 pB = mvp * glm::vec4(mPointB, 1.0f);
	// pB /= pB.w;
	// std::cout<<"Pos: "<<glm::to_string(pA)<<",  "<<glm::to_string(pB)<<"\n";
	mShader->setUniformMat4f("u_MVP", mvp);
	
	mVA->bind();

	GLCall(glDrawArrays(GL_LINES, 0, 2)); // No indexing needed, a line only has two vertices

    mVA->unbind();
	mShader->unbind();
}

void line::draw_selection_impl(std::shared_ptr<camera> cam)
{

}

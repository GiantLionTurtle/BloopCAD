
#include "selectionRectangle.hpp"

#include <graphics_utils/GLCall.hpp>
#include <graphics_utils/shadersPool.hpp>

selectionRectangle::selectionRectangle(glm::vec2 start, glm::vec2 end, geom_3d::plane_abstr_ptr basePlane_):
	sketchEntity(basePlane_, -1),
	mStartPt(start),
	mEndPt(end),
	mMode(TOUCH)
{
	init_buffers();
	set_points(start, end);
}

void selectionRectangle::init()
{
	set_name("selection");
	
	mVB = std::shared_ptr<vertexBuffer>(new vertexBuffer(mVertices, sizeof(glm::vec3) * 4)); // Upload the vertices
	mVA = std::shared_ptr<vertexArray>(new vertexArray());

	vertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();
	mVA->add_buffer(*mVB.get(), layout);
	mIB = std::shared_ptr<indexBuffer>(new indexBuffer(mIndices, 6));

	// Create the shaders
	mShader = shadersPool::get_instance().get("plane");
	if(!mShader) {
		mShader = shader::fromFiles_ptr("resources/shaders/planeShader.vert", "resources/shaders/planeShader.frag");
		shadersPool::get_instance().add("plane", mShader);
	}
}

void selectionRectangle::update_VB()
{
	init_buffers();
	mVB->bind();
	mVB->set(mVertices, sizeof(glm::vec3) * 4);
	mVB->unbind();
	mRequire_VBUpdate = false;
}

void selectionRectangle::set_startPoint(glm::vec2 pt)
{
	mStartPt = pt;
    glm::vec2 diff = mEndPt - mStartPt;
	mVertices[0] = mBasePlane->to_worldPos(mStartPt);
	mVertices[1] = mBasePlane->to_worldPos(mStartPt + glm::vec2(0.0f, diff.y));
	mVertices[3] = mBasePlane->to_worldPos(mStartPt + glm::vec2(diff.x, 0.0f));
	mRequire_VBUpdate = true;
	set_require_redraw();
}
void selectionRectangle::set_endPoint(glm::vec2 pt)
{
	mEndPt = pt;
    glm::vec2 diff = mEndPt - mStartPt;
	mVertices[1] = mBasePlane->to_worldPos(mStartPt + glm::vec2(0.0f, diff.y));
	mVertices[2] = mBasePlane->to_worldPos(mEndPt);
	mVertices[3] = mBasePlane->to_worldPos(mStartPt + glm::vec2(diff.x, 0.0f));
	mRequire_VBUpdate = true;
	set_require_redraw();
}
void selectionRectangle::set_points(glm::vec2 start_, glm::vec2 end_)
{
	mStartPt = start_;
	mEndPt = end_;
	glm::vec2 diff = end_ - start_;
	mVertices[0] = mBasePlane->to_worldPos(mStartPt);
	mVertices[1] = mBasePlane->to_worldPos(mStartPt + glm::vec2(0.0f, diff.y));
	mVertices[2] = mBasePlane->to_worldPos(mEndPt);
	mVertices[3] = mBasePlane->to_worldPos(mStartPt + glm::vec2(diff.x, 0.0f));
	mRequire_VBUpdate = true;
	set_require_redraw();
}

void selectionRectangle::draw_impl(camera_ptr cam, int frame)
{
	if(mRequire_VBUpdate)
		update_VB();

	mShader->bind();

	glm::vec3 color;
	if(mMode == COVER) {
		color = mColorA;
	} else {
		color = mColorB;
	}
	mShader->setUniform4f("u_Color", color.r, color.g, color.b, 0.3f);

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
}

void selectionRectangle::init_buffers()
{
	mIndices[0] = 0;
	mIndices[1] = 1;
	mIndices[2] = 2; // First triangle
	mIndices[3] = 2;
	mIndices[4] = 3;
	mIndices[5] = 0; // Second triangle
}
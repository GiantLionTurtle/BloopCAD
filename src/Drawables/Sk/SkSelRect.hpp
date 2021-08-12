
#ifndef SELECTIONRECTANGLE_HPP_
#define SELECTIONRECTANGLE_HPP_

#include "SkDrawable.hpp"
#include <Geom/Geom3d/Plane_abstr.hpp>

class SkSelRect : public SkDrawable {
public:
	enum mode_types { TOUCH, COVER };
private:
	const glm::vec3 mColorA = glm::vec3(1.0f, 0.0f, 0.0f), 
					mColorB = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 mVertices[4];	// The four vertices of the rectangle that represents the plane
	unsigned int mIndices[6];
	glm::vec2 mStartPt, mEndPt;

	int mMode;

	VertexArray* mVA;
	VertexBuffer* mVB;
	IndexBuffer* mIB;
	std::shared_ptr<Shader> mShader;
public:
	SkSelRect(glm::vec2 start_, glm::vec2 end_, Geom3d::Plane_abstr* basePlane_);
	virtual ~SkSelRect() {}

	glm::vec2 start() const { return mStartPt; }
	void set_startPoint(glm::vec2 pt);
	glm::vec2 end() const { return mEndPt; }
	void set_endPoint(glm::vec2 pt);
	void set_points(glm::vec2 start_, glm::vec2 end_);

	void set_mode(int md) { mMode = md; }
	int mode() const { return mMode; }

	int selection_rank() { return -1; }

	// These should not be called
	virtual SelectionPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) { return SelectionPoint(); }
	virtual DraggableSelectionPoint closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) { return DraggableSelectionPoint(); }
protected:	
	void init_impl();
	void draw_impl(Camera_ptr cam, int frame, draw_type type = draw_type::ALL);
	void update_impl();

	void init_buffers();
};

#endif

#ifndef SELECTIONRECTANGLE_HPP_
#define SELECTIONRECTANGLE_HPP_

#include "sketchEntity.hpp"
#include <geometry/geometry_3d/plane_abstr.hpp>

class selectionRectangle : public sketchEntity {
public:
	enum mode_types { TOUCH, COVER };
private:
	const glm::vec3 mColorA = glm::vec3(1.0f, 0.0f, 0.0f), 
					mColorB = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 mVertices[4];	// The four vertices of the rectangle that represents the plane
	unsigned int mIndices[6];
	glm::vec2 mStartPt, mEndPt;

	int mMode;
public:
	selectionRectangle(glm::vec2 start_, glm::vec2 end_, geom_3d::plane_abstr_ptr basePlane_);
	virtual ~selectionRectangle() {}

	void init();
	void update_VB();

	glm::vec2 start() const { return mStartPt; }
	void set_startPoint(glm::vec2 pt);
	glm::vec2 end() const { return mEndPt; }
	void set_endPoint(glm::vec2 pt);
	void set_points(glm::vec2 start_, glm::vec2 end_);

	void set_mode(int md) { mMode = md; }
	int mode() const { return mMode; }

	int selection_rank() { return -1; }
protected:
	void draw_impl(camera_ptr cam, int frame);
	
	void init_buffers();
};

#endif
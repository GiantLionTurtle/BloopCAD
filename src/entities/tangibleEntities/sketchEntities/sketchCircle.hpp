
#ifndef SKETCHCIRCLE_HPP_
#define SKETCHCIRCLE_HPP_

#include <entities/geometry/circle_abstract.hpp>
#include <entities/tangibleEntities/point.hpp>
#include "sketchElement.hpp"

#define CIRCLE_RES 100

class sketchCircle : public tangibleEntity, public sketchElement, public circle_abstract {
private:
	const glm::vec3 mColor = glm::vec3(0.0f, 0.89f, 0.725f); // Curve color
	glm::vec3 mVertices[CIRCLE_RES]; // The vertices describing the quand containing the circle
	unsigned int mIndices[2*CIRCLE_RES];
public:
	sketchCircle(circle_abstract const& baseCircle, plane_abstract_ptr basePlane_);
	virtual ~sketchCircle() {}

	virtual int selection_rank() { return 8; }

	virtual void update_VB();
protected:
	virtual void draw_impl(camera_ptr cam, int frame);

	virtual void post_set_update() {}
	
	virtual float selection_depth(camera_ptr cam, glm::vec2 cursor_pos);

	void init_buffers();
};

#endif
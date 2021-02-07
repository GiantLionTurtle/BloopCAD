
#ifndef SKETCHCIRCLE_HPP_
#define SKETCHCIRCLE_HPP_

#include <geometry/geometry_2d/circle_abstr.hpp>
#include "sketchPoint.hpp"
#include "sketchEntity.hpp"

#define CIRCLE_RES 100

class sketchCircle;
using sketchCircle_ptr = std::shared_ptr<sketchCircle>;

class sketchCircle : public sketchEntity, public geom_2d::circle_abstr {
private:
	const glm::vec3 mColor = glm::vec3(0.0f, 0.89f, 0.725f); // Curve color
	glm::vec3 mVertices[CIRCLE_RES]; // The vertices describing the quand containing the circle
	unsigned int mIndices[2*CIRCLE_RES];

	sketchPoint_ptr mCenter;
	variable_ptr mRadius;
public:
	sketchCircle(glm::vec2 center_, float radius_, geom_3d::plane_abstr_ptr basePlane_);
	virtual ~sketchCircle();

	virtual void init();

	virtual void move(glm::vec2 from, glm::vec2 to);

	virtual void for_each(std::function<void (entity_ptr)> func);

	virtual glm::vec2 center() { return mCenter->pos(); }
	virtual float radius() { return mRadius->val(); }
	void set_radius(float newval);

	virtual int selection_rank() { return 8; }

	virtual void update_VB();
protected:
	virtual void draw_impl(camera_ptr cam, int frame);

	virtual void post_set_behavior();
	
	virtual float selection_depth(camera_ptr cam, glm::vec2 cursor_pos);

	void init_buffers();
};

#endif
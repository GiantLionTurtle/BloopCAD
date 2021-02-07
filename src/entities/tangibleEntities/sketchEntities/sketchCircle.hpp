
#ifndef SKETCHCIRCLE_HPP_
#define SKETCHCIRCLE_HPP_

#include <geometry/geometry_2d/circle_abstr.hpp>
#include "sketchPoint.hpp"
#include "sketchEntity.hpp"
#include <entities/sketch.hpp>

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
	sketchCircle(glm::vec2 center_, float radius_, sketch* parent_sk);
	virtual ~sketchCircle();

	void init();

	void move(glm::vec2 from, glm::vec2 to);

	bool in_selection_range(glm::vec2 planepos, camera_ptr cam, glm::vec2 cursor);

	void for_each(std::function<void (entity_ptr)> func);

	glm::vec2 center() { return mCenter->pos(); }
	float radius() { return mRadius->val(); }
	void set_radius(float newval);

	int selection_rank() { return 4; }

	void update_VB();
protected:
	void draw_impl(camera_ptr cam, int frame);

	void post_set_behavior();
	
	// float selection_depth(camera_ptr cam, glm::vec2 cursor_pos);

	void init_buffers();
};

#endif
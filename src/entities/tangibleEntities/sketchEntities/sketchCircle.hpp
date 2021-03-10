
#ifndef SKETCHCIRCLE_HPP_
#define SKETCHCIRCLE_HPP_

#include <geometry/geometry_2d/circle_abstr.hpp>
#include "sketchPoint.hpp"
#include "sketchGeometry.hpp"

#define CIRCLE_RES 100

class sketchCircle;
using sketchCircle_ptr = std::shared_ptr<sketchCircle>;

class sketchCircle : public sketchGeometry, public geom_2d::circle_abstr {
private:
	static float kSelDist2;
	static bool kFisrstInst;
	static glm::vec3 kColor, kColorHovered, kColorSelected; // Circle color

	glm::vec3 mVertices[CIRCLE_RES]; // The vertices describing the quand containing the circle
	unsigned int mIndices[2*CIRCLE_RES];

	sketchPoint_ptr mCenter;
	variable_ptr mRadius;
public:
	sketchCircle(glm::vec2 center_, float radius_, geom_3d::plane_abstr_ptr basePlane_);
	virtual ~sketchCircle();

	void init();

	void move(glm::vec2 from, glm::vec2 to);

	bool in_selection_range(glm::vec2 planepos, camera_ptr cam, glm::vec2 cursor);
	bool in_selection_range(glm::vec2 a, glm::vec2 b, bool contained);

	void set_constant();
	void set_tmpConstant(bool const_);

	void for_each(std::function<void (entity_ptr)> func);
	void for_each(std::function<void(sketchEntity_ptr ent)> func);

	glm::vec2 posCenter() { return mCenter->pos(); }
	sketchPoint_ptr center() { return mCenter; }
	float valRadius() { return mRadius->eval(); }
	variable_ptr radius() { return mRadius; }
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
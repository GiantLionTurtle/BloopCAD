
#ifndef SKETCHCIRCLE_HPP_
#define SKETCHCIRCLE_HPP_

#include <geometry/geometry_2d/circle_abstr.hpp>
#include "sketchPoint.hpp"
#include "sketchGeometry.hpp"

#define CIRCLE_RES 100

class sketchCircle;
using sketchCircle_ptr = std::shared_ptr<sketchCircle>;

class sketchCircle : public sketchGeometry, public Geom2d::circle_abstr, public std::enable_shared_from_this<sketchCircle> {
private:
	static float kSelDist2;
	static bool kFisrstInst;
	static glm::vec3 kColor, kColorHovered, kColorSelected; // Circle color

	glm::vec3 mVertices[CIRCLE_RES]; // The vertices describing the quand containing the circle
	unsigned int mIndices[2*CIRCLE_RES];

	sketchPoint_ptr mCenter;
	var_ptr mRadius;
public:
	sketchCircle(glm::vec2 center_, float radius_, Geom3d::plane_abstr* basePlane_);
	virtual ~sketchCircle();

	void init();

	void print(int depth = 0);

	void move(glm::vec2 from, glm::vec2 to, glm::vec2 pixel_move);

	virtual entityPosSnapshot_ptr posSnapshot();

	bool in_selection_range(glm::vec2 planepos, Camera_ptr cam, glm::vec2 cursor);
	bool in_selection_range(glm::vec2 a, glm::vec2 b, bool contained);

	void set_constant();
	void set_tmpConstant(bool const_);

	void for_each(std::function<void (Drawable*)> func);
	void for_each(std::function<void(sketchEntity_ptr ent)> func);
	void for_each(std::function<void(sketchGeometry_ptr geom)> func);

	glm::vec2 posCenter() { return mCenter->pos(); }
	sketchPoint_ptr center() { return mCenter; }
	float valRadius() { return mRadius->eval(); }
	var_ptr radius() { return mRadius; }
	void set_radius(float newval);

	int selection_rank() { return 4; }

	void update_VB();
	
	void set_exists_vars(bool ex);
protected:
	void draw_impl(Camera_ptr cam, int frame);

	void post_set_behavior();
	
	void init_buffers();
};

class circleSnapshot : public entityPosSnapshot_abstract {
private:
	sketchCircle_ptr mCircle;
	double x, y, r;
public:
	circleSnapshot(sketchCircle_ptr pt);
	circleSnapshot(sketchCircle_ptr source, sketchCircle_ptr target);

	sketchEntity_ptr ent() { return mCircle; }
	void apply();
};

#endif
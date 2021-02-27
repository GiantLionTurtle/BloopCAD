
#ifndef SKETCHPOINT_HPP_
#define SKETCHPOINT_HPP_

#include "sketchEntity.hpp"
#include <geometry/geometry_2d/point_abstr.hpp>
#include <geometry/geometry_3d/plane_abstr.hpp>
#include <constraintsSolver/expression.hpp>

class sketchPoint;
using sketchPoint_ptr = std::shared_ptr<sketchPoint>;

class sketchPoint : public sketchEntity, public geom_2d::point_abstr {
private:
	const glm::vec3 mColor = glm::vec3(0.0f, 0.89f, 0.725f); // Point color

	variable_ptr mX, mY;
public:
	sketchPoint(glm::vec2 pos_2d, geom_3d::plane_abstr_ptr basePlane_, bool immovable = false);
	sketchPoint(variable_ptr x_, variable_ptr y_, geom_3d::plane_abstr_ptr basePlane_, bool immovable = false);
	virtual ~sketchPoint() {}

	void print(int depth = 0);

	void init();

	glm::vec2 pos() const { return glm::vec2(mX->eval(), mY->eval()); }
	variable_ptr x() { return mX; }
	variable_ptr y() { return mY; }

	void move(glm::vec2 from, glm::vec2 to);

	bool in_selection_range(glm::vec2 planepos, camera_ptr cam, glm::vec2 cursor);

	void set(glm::vec2 p);
	
	void set_constant();
	void set_tmpConstant(bool const_);

	int selection_rank() { return 5; }

	void update_VB();
protected:
	void draw_impl(camera_ptr cam, int frame);

	// float selection_depth(camera_ptr cam, glm::vec2 cursor_pos);

	void post_set_behavior();
};

#endif 
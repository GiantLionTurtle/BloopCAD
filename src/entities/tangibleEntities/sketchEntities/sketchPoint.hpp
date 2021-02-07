
#ifndef SKETCHPOINT_HPP_
#define SKETCHPOINT_HPP_

#include "sketchEntity.hpp"
#include <geometry/geometry_2d/point_abstr.hpp>
#include <geometry/geometry_3d/plane_abstr.hpp>

class sketchPoint;
using sketchPoint_ptr = std::shared_ptr<sketchPoint>;

class sketchPoint : public sketchEntity, public geom_2d::point_abstr {
private:
	const glm::vec3 mColor = glm::vec3(0.0f, 0.89f, 0.725f); // Point color

	variableVector2_ptr mPos;
public:
	sketchPoint(glm::vec2 pos_2d, geom_3d::plane_abstr_ptr basePlane_, bool immovable = false);
	sketchPoint(variableVector2_ptr pos_2d, geom_3d::plane_abstr_ptr basePlane_, bool immovable = false);
	virtual ~sketchPoint() {}

	virtual void init();

	virtual glm::vec2 pos() { return mPos->get(); }

	virtual void move(glm::vec2 from, glm::vec2 to);
	variableVector2_ptr var() { return mPos; }
	void set(glm::vec2 pt) { mPos->set(pt); set_require_VBUpdate(); }

	virtual void set_constant() { mPos->set_constant(); }
	virtual void set_tmpConstant(bool const_) { mPos->set_tmpConstant(const_); }

	virtual int selection_rank() { return 5; }

	virtual void update_VB();

	virtual std::vector<variable_ptr> variables();

	virtual subEquationsSystem coincidence();
	virtual std::vector<subEquationsSystem> verticality();
	virtual std::vector<subEquationsSystem> horizontality();
protected:
	virtual void draw_impl(camera_ptr cam, int frame);

	virtual float selection_depth(camera_ptr cam, glm::vec2 cursor_pos);

	virtual void post_set_behavior();
};

#endif 
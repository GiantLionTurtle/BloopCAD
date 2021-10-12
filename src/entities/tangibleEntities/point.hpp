
#ifndef POINT_HPP_
#define POINT_HPP_

#include "tangibleEntity.hpp"
#include <geometry/geometry_3d/point_abstr.hpp>
#include <constraintsSolver/expression.hpp>

#include <glm/glm.hpp>

class point : public tangibleEntity, public geom_3d::point_abstr {
private:
	const glm::vec3 mColor = glm::vec3(0.0f, 0.89f, 0.725f); // Point color

	var_ptr mX, mY, mZ;
public:
	point(glm::vec3 const& basePoint);
	// point(geom_3d::point_abstr_ptr basePoint);
	virtual ~point() {}

	virtual void init();

	virtual int selection_rank() { return 5; }

	virtual void move(glm::vec3 from, glm::vec3 to);

	glm::vec3 pos() const;
	void set(glm::vec3 const& p);

	virtual void set_constant();
	virtual void set_tmpConstant(bool const_);

	virtual void update_VB();
protected:
	virtual void draw_impl(camera_ptr cam, int frame);

	virtual float selection_depth(camera_ptr cam, glm::vec2 cursor_pos);

	virtual void post_set_update();
};

#endif
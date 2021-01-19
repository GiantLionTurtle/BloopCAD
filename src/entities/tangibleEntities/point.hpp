
#ifndef POINT_HPP_
#define POINT_HPP_

#include "tangibleEntity.hpp"
#include <entities/geometry/point_abstract.hpp>

#include <glm/glm.hpp>

class point : public tangibleEntity, public point_abstract {
private:
	const glm::vec3 mColor = glm::vec3(0.0f, 0.89f, 0.725f); // Point color
public:
	point(point_abstract const& basePoint);
	point(point_abstract_ptr basePoint);
	virtual ~point() {}

	virtual int selection_rank() { return 5; }

	virtual void move(glm::vec3 from, glm::vec3 to);
	virtual void set_constant();
	virtual void set_tmpConstant(bool const_);

	virtual void update_VB();
protected:
	virtual void draw_impl(camera_ptr cam, int frame);

	virtual float selection_depth(camera_ptr cam, glm::vec2 cursor_pos);

	virtual void post_set_update();

	void create();
};

#endif
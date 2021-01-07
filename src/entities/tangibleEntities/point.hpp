
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
	point(std::shared_ptr<point_abstract> basePoint);
	virtual ~point() {}

	virtual void set_pos(glm::vec3 pos);
	virtual void set_pos(point_abstract const& other);

	virtual void update_VB();
protected:
	void draw_impl(std::shared_ptr<camera> cam, int frame);
	void draw_selection_impl(std::shared_ptr<camera> cam, int frame);

	void create();
};

#endif
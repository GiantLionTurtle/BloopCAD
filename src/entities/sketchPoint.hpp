
#ifndef SKETCHPOINT_HPP_
#define SKETCHPOINT_HPP_

#include "point.hpp"

class sketchPoint : public point {
public:
	sketchPoint(point_abstract const& basePoint);
	~sketchPoint() {}

	virtual void set_pos(glm::vec3 pos);
	virtual void set_pos(point_abstract const& other);

	void update_VB();
};

#endif 
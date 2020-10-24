
#ifndef PART_HPP_
#define PART_HPP_

#include "entity.hpp"
#include "plane_abstract.hpp"
#include "plane.hpp"
#include "editable.hpp"

#include <memory>

class sketch : public plane_abstract, public entity, public editable {
private:
	std::vector<std::shared_ptr<entity>> mEntities;
public:
	sketch(plane_abstract const& plane_);

	void add_entity();

    virtual bool is_sketch() { return true; }
};

class part : public entity {
private:
	std::shared_ptr<plane> mXY_plane, mYZ_plane, mZX_plane;
public:
	part() 
	{
		mXY_plane = plane::from_1Point2Vectors_ptr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.0f, 0.0f), glm::vec3(0.0f, 0.8f, 0.0f));
		mYZ_plane = plane::from_1Point2Vectors_ptr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.8f, 0.0f), glm::vec3(0.0f, 0.0f, 0.8f));
		mZX_plane = plane::from_1Point2Vectors_ptr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.8f), glm::vec3(0.8f, 0.0f, 0.0f));
	};


	virtual void draw(std::shared_ptr<camera> cam)
	{
		mXY_plane->draw(cam);
		mYZ_plane->draw(cam);
		mZX_plane->draw(cam);
	}

    virtual std::string type() { return "part"; }
};

#endif
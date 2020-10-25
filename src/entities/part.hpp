
#ifndef PART_HPP_
#define PART_HPP_

#include "entity.hpp"
#include "plane_abstract.hpp"
#include "plane.hpp"
#include "editable.hpp"

#include "entitiesIndexer.hpp"

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
	entitiesIndexer mEntities;
public:
	part():
		mEntities(glm::ivec3(100, 0, 0)) 
	{
		mEntities.add(plane::from_1Point2Vectors_ptr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.0f, 0.0f), glm::vec3(0.0f, 0.8f, 0.0f)));
		mEntities.add(plane::from_1Point2Vectors_ptr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.8f, 0.0f), glm::vec3(0.0f, 0.0f, 0.8f)));
		mEntities.add(plane::from_1Point2Vectors_ptr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.8f), glm::vec3(0.8f, 0.0f, 0.0f)));
	};


	virtual void draw(std::shared_ptr<camera> cam)
	{
		mEntities.for_each([cam](std::shared_ptr<entity> ent) { ent->draw(cam); });
	}

	virtual void draw_selection(std::shared_ptr<camera> cam, glm::ivec3 ind)
	{
		mEntities.for_each([cam](std::shared_ptr<entity> ent, glm::ivec3 ind) { ent->draw_selection(cam, ind); });
	}

    virtual std::string type() { return "part"; }
};

#endif
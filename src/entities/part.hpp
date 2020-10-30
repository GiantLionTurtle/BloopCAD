
#ifndef PART_HPP_
#define PART_HPP_

#include "entity.hpp"
#include "plane_abstract.hpp"
#include "plane.hpp"
#include "editable.hpp"

#include "entitiesIndexer.hpp"

#include "../errorLogger.hpp"

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
	std::shared_ptr<entitiesIndexer> mEntities;
	std::shared_ptr<plane> mXY, mYZ, mZX;
public:
	part():
		mEntities(new entitiesIndexer()) 
	{
		init_scene();
	}
	part(std::shared_ptr<entitiesIndexer> indexer):
		mEntities(indexer)
	{
		init_scene();
	}

	virtual void init_scene()
	{
		mXY = plane::from_1Point2Vectors_ptr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.0f, 0.0f), glm::vec3(0.0f, 0.8f, 0.0f));
		mYZ = plane::from_1Point2Vectors_ptr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.8f, 0.0f), glm::vec3(0.0f, 0.0f, 0.8f));
		mZX = plane::from_1Point2Vectors_ptr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.8f), glm::vec3(0.8f, 0.0f, 0.0f));
		mEntities->add(mXY);
		mEntities->add(mYZ);
		mEntities->add(mZX);
	}

	virtual void draw(std::shared_ptr<camera> cam)
	{
		update_transforms();
		mXY->draw(cam);
		mYZ->draw(cam);
		mZX->draw(cam);
	}

	virtual void draw_selection(std::shared_ptr<camera> cam)
	{
		update_transforms();
		// mEntities->for_each([cam](std::shared_ptr<entity> ent, glm::ivec3 ind) { ent->draw_selection(cam, ind); });
		mXY->draw_selection(cam);
		mYZ->draw_selection(cam);
		mZX->draw_selection(cam);
	}

	void update_transforms()
	{
		mXY->set_transform(mTransform);
		mYZ->set_transform(mTransform);
		mZX->set_transform(mTransform);
	}

    virtual std::string type() { return "part"; }
};

#endif
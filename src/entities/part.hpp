
#ifndef PART_HPP_
#define PART_HPP_

#include "entity.hpp"
#include "plane_abstract.hpp"
#include "plane.hpp"
#include "editable.hpp"

#include "entitiesIndexer.hpp"

#include <errorLogger.hpp>

#include <memory>

class sketch : public entity {
private:
	std::shared_ptr<entitiesIndexer> mIndexer;
	std::vector<std::shared_ptr<entity>> mEntities;

	std::shared_ptr<plane_abstract> mBasePlane;
public:
	sketch(std::shared_ptr<plane_abstract> base_plane);

	void add_entity();

    virtual bool is_sketch() { return true; }
};

class part : public entity {
private:
	std::shared_ptr<entitiesIndexer> mIndexer;
	std::shared_ptr<plane> mXY, mYZ, mZX;

	std::vector<std::shared_ptr<entity>> mEntities;
public:
	part();
	part(std::shared_ptr<entitiesIndexer> indexer);

	virtual void init_scene();

	virtual void draw(std::shared_ptr<camera> cam);
	virtual void draw_selection(std::shared_ptr<camera> cam);

	void update_transforms();

	
};

#endif
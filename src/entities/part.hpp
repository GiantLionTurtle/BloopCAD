
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
	part();
	part(std::shared_ptr<entitiesIndexer> indexer);

	virtual void init_scene();

	virtual void draw(std::shared_ptr<camera> cam);

	virtual void draw_selection(std::shared_ptr<camera> cam);

	void update_transforms();

    virtual std::string type() { return "part"; }
};

#endif
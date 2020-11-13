
#ifndef PART_HPP_
#define PART_HPP_

#include "entity.hpp"
#include "entitiesIndexer.hpp"

#include "plane_abstract.hpp"
#include "plane.hpp"
#include "sketch.hpp"

#include <memory>

class part : public entity, public entitiesIndexer {
private:
	std::shared_ptr<plane> mXY, mYZ, mZX;
public:
	part();
	part(std::shared_ptr<entitiesIndexer> indexer);

	virtual void init_scene();

	virtual void draw(std::shared_ptr<camera> cam);
	virtual void draw_selection(std::shared_ptr<camera> cam);

	void update_transforms();
};

#endif
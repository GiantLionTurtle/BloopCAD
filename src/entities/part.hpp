
#ifndef PART_HPP_
#define PART_HPP_

#include "entity.hpp"
#include "entitiesIndexer.hpp"

#include "plane_abstract.hpp"
#include "plane.hpp"
#include "sketch.hpp"

#include <memory>

class document;

class part : public entity, public entitiesIndexer {
private:
	std::shared_ptr<plane> mXY, mYZ, mZX;
	std::vector<std::shared_ptr<sketch>> mSketches;
public:
	part();
	part(std::shared_ptr<entitiesIndexer> indexer);

	virtual void init_scene();

	void add_sketch(std::shared_ptr<sketch> sketch_);
	std::shared_ptr<sketch> get_sketch(int ind = -1);
protected:
	virtual void draw_impl(std::shared_ptr<camera> cam);
	virtual void draw_selection_impl(std::shared_ptr<camera> cam);
};

#endif
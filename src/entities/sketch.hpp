
#ifndef SKETCH_HPP_
#define SKETCH_HPP_

#include "entity.hpp"
#include "entitiesIndexer.hpp"
#include "plane_abstract.hpp"

#include <memory>
#include <vector>

class sketch : public entity, public entitiesIndexer {
private:
	std::shared_ptr<plane_abstract> mBasePlane;
public:
	sketch(std::shared_ptr<plane_abstract> base_plane);
	sketch(std::shared_ptr<plane_abstract> base_plane, std::shared_ptr<entitiesIndexer> indexer);

	std::shared_ptr<plane_abstract> basePlane() { return mBasePlane; }
protected:
	virtual void draw_impl(std::shared_ptr<camera> cam);
	virtual void draw_selection_impl(std::shared_ptr<camera> cam);
};

#endif

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

    virtual bool is_sketch() { return true; }
};

#endif
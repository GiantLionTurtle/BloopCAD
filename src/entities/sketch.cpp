
#include "sketch.hpp"

sketch::sketch(std::shared_ptr<plane_abstract> base_plane):
    mBasePlane(base_plane),
    entitiesIndexer()
{

}
sketch::sketch(std::shared_ptr<plane_abstract> base_plane, std::shared_ptr<entitiesIndexer> indexer):
    mBasePlane(base_plane),
    entitiesIndexer(indexer)
{

}
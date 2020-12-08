
#include "sketch.hpp"
#include <utils/errorLogger.hpp>

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

void sketch::draw_impl(std::shared_ptr<camera> cam)
{
    for_each([cam](std::shared_ptr<entity> ent) { ent->draw(cam); });
}
void sketch::draw_selection_impl(std::shared_ptr<camera> cam)
{
    for_each([cam](std::shared_ptr<entity> ent) { ent->draw_selection(cam); });
}
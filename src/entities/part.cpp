
#include "part.hpp"

part::part():
    entitiesIndexer()
{
    init_scene();
}
part::part(std::shared_ptr<entitiesIndexer> indexer):
    entitiesIndexer(indexer)
{
    init_scene();
}

void part::init_scene()
{
    mXY = plane::from_1Point2Vectors_ptr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    mYZ = plane::from_1Point2Vectors_ptr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    mZX = plane::from_1Point2Vectors_ptr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    add(mXY);
    add(mYZ);
    add(mZX);
}

void part::draw(std::shared_ptr<camera> cam)
{
    update_transforms();
    mXY->draw(cam);
    mYZ->draw(cam);
    mZX->draw(cam);
}

void part::draw_selection(std::shared_ptr<camera> cam)
{
    update_transforms();
    // mEntities->for_each([cam](std::shared_ptr<entity> ent, glm::ivec3 ind) { ent->draw_selection(cam, ind); });
    mXY->draw_selection(cam);
    mYZ->draw_selection(cam);
    mZX->draw_selection(cam);
}

void part::update_transforms()
{
    transform();
    
    mXY->set_transform(mTranslation, mScale);
    mYZ->set_transform(mTranslation, mScale);
    mZX->set_transform(mTranslation, mScale);
}

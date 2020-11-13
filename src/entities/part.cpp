
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

void part::draw(std::shared_ptr<camera> cam, glm::mat4 additionalTransform)
{
	additionalTransform *= transform(); // TOOD: right order?
	mXY->draw(cam, additionalTransform);
	mYZ->draw(cam, additionalTransform);
	mZX->draw(cam, additionalTransform);
}

void part::draw_selection(std::shared_ptr<camera> cam, glm::mat4 additionalTransform)
{
	additionalTransform *= transform(); // TOOD: right order?
	mXY->draw_selection(cam, additionalTransform);
	mYZ->draw_selection(cam, additionalTransform);
	mZX->draw_selection(cam, additionalTransform);
}

void part::update_transforms()
{
	transform();
	
	mXY->set_transform(mTranslation, mScale);
	mYZ->set_transform(mTranslation, mScale);
	mZX->set_transform(mTranslation, mScale);
}


#include "part.hpp"
#include <utils/errorLogger.hpp>

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
	mYZ = plane::from_1Point2Vectors_ptr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), true);
	mZX = plane::from_1Point2Vectors_ptr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	add(mXY);
	add(mYZ);
	add(mZX);
}

void part::add_sketch(std::shared_ptr<sketch> sketch_)
{
	if(sketch_) {
		mSketches.push_back(sketch_);
	} else {
		LOG_WARNING("Trying to add null sketch.");
	}
}
std::shared_ptr<sketch> part::get_sketch(int ind)
{
	if(ind < 0 || ind >= mSketches.size()) {
		if(mSketches.empty())
			return nullptr;
		return mSketches.back();
	}
	return mSketches[ind]; 
}

void part::draw_impl(std::shared_ptr<camera> cam)
{
	mXY->draw(cam);
	mYZ->draw(cam);
	mZX->draw(cam);
}

void part::draw_selection_impl(std::shared_ptr<camera> cam)
{
	mXY->draw_selection(cam);
	mYZ->draw_selection(cam);
	mZX->draw_selection(cam);
}
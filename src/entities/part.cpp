
#include "part.hpp"
#include <utils/errorLogger.hpp>
#include <entities/line.hpp>
#include <actions/sketchDesign/addLine_action.hpp>

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
	add(plane::from_1Point2Vectors_ptr(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.5f)));

	add(mXY);
	add(mYZ);
	add(mZX);
}

void part::add_sketch(std::shared_ptr<sketch> sketch_)
{
	if(sketch_) {
		mSketches.push_back(sketch_);
		add(sketch_);
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
	for_each([cam](std::shared_ptr<entity> ent) { ent->draw(cam); });
	// if(!mSketches.empty())
		// mSketches.back()->draw(cam);
}

void part::draw_selection_impl(std::shared_ptr<camera> cam)
{
	for_each([cam](std::shared_ptr<entity> ent) { ent->draw_selection(cam); });
}
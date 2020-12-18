
#include "part.hpp"
#include <utils/errorLogger.hpp>
#include <entities/line.hpp>
#include <actions/sketchDesign/addLine_action.hpp>

part::part():
	entity(glm::ivec3(200, 0, 1))
{
	init_scene();
}
part::part(entity* parent):
	entity(parent)
{
	init_scene();
}


void part::init_scene()
{
	// Maybe this whole function should just be in the constructor

	// Create the 3 planes of the origin
	mXY = std::shared_ptr<plane>(new plane(
		plane_abstract::from_1Point2Vectors(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))));
	mYZ = std::shared_ptr<plane>(new plane(
		plane_abstract::from_1Point2Vectors(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), true))); // Inverted due to openGL's z axis being kind of backward
	mZX = std::shared_ptr<plane>(new plane(
		plane_abstract::from_1Point2Vectors(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f))));
	
	add(std::shared_ptr<plane>(new plane(
		plane_abstract::from_1Point2Vectors(glm::vec3(-3.0f, 0.5f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f))))); // Temporary test plane

	// Add the three points
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
	// It's kind of a funny behavior, I might change it to just return nullptr on out of bounds
	if(ind < 0 || ind >= mSketches.size()) {
		if(mSketches.empty())
			return nullptr;
		return mSketches.back();
	}
	return mSketches[ind]; 
}

void part::draw_impl(std::shared_ptr<camera> cam, int frame)
{

}

void part::draw_selection_impl(std::shared_ptr<camera> cam, int frame)
{

}
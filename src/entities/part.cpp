
#include "part.hpp"

#include "entityView.hpp"
#include <utils/errorLogger.hpp>
#include <entities/line.hpp>
#include <actions/sketchDesign/addLine_action.hpp>

part::part():
	entity(glm::ivec3(0, 0, 1))
{
	init_scene();
	set_name("Part");
}
part::part(entity* parent):
	entity(parent)
{
	init_scene();
	set_name("Part");
}


void part::init_scene()
{
	// Maybe this whole function should just be in the constructor
	
	// Create the 3 planes of the origin
	mXY = std::shared_ptr<plane>(new plane(
		plane_abstract::from_1Point2Vectors(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))));
	mXY->set_name("XY plane");
	mYZ = std::shared_ptr<plane>(new plane(
		plane_abstract::from_1Point2Vectors(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), true))); // Inverted due to openGL's z axis being kind of backward
	mYZ->set_name("YZ plane");
	mZX = std::shared_ptr<plane>(new plane(
		plane_abstract::from_1Point2Vectors(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f))));
	mZX->set_name("ZX plane");

	mOrigin = std::shared_ptr<folder>(new folder("origin"));
	// mOrigin->add(mXY);
	// mOrigin->add(mYZ);
	// mOrigin->add(mZX);

	// add(mOrigin);

	add(mXY);
	add(mYZ);
	add(mZX);

	add(std::shared_ptr<plane>(new plane(
		plane_abstract::from_1Point2Vectors(glm::vec3(-3.0f, 0.5f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f))))); // Temporary test plane
}

void part::set_handle(entityHandle* handle_)
{
	mHandle = handle_;
	for(int i = 0; i < mChildren.size(); ++i) {
		if(std::get<1>(mChildren[i])->handle())
			delete std::get<1>(mChildren[i])->handle();
		std::get<1>(mChildren[i])->set_handle(new entityHandle(std::get<1>(mChildren[i]), mHandle->view(), mHandle));
	}
}

void part::add(std::shared_ptr<entity> elem)
{
	entity::add(elem);
	if(elem && mHandle) {
		elem->set_handle(new entityHandle(elem, mHandle->view(), mHandle));
	}
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
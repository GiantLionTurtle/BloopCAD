
#include "part.hpp"

#include "entityView.hpp"
#include <utils/errorLogger.hpp>
#include "tangibleEntities/point.hpp"

part::part():
	entity()
{
	set_name("Part");
	// init();
	// init_scene();
}
part::part(entity* parent):
	entity(parent)
{
	set_name("Part");
	// init();
	// init_scene();
}


void part::init()
{
	// Maybe this whole function should just be in the constructor
	
	// Create the 3 planes of the origin
	mXY = std::shared_ptr<plane>(new plane(
		geom_3d::plane_abstr::from_1Point2Vectors(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))));
	mXY->set_name("XY plane");
	mYZ = std::shared_ptr<plane>(new plane(
		geom_3d::plane_abstr::from_1Point2Vectors(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), true))); // Inverted due to openGL's z axis being kind of backward
	mYZ->set_name("YZ plane");
	mZX = std::shared_ptr<plane>(new plane(
		geom_3d::plane_abstr::from_1Point2Vectors(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f))));
	mZX->set_name("ZX plane");

	std::shared_ptr<point> center = std::make_shared<point>(glm::vec3(0.0f, 0.0f, 0.0f));
	center->set_name("originPoint");

	mOrigin = folder_ptr(new folder("origin", this));
	
	mOrigin->add(center);
	mOrigin->add(mXY);
	mOrigin->add(mYZ);
	mOrigin->add(mZX);
	mOrigin->add(std::shared_ptr<plane>(new plane(
		geom_3d::plane_abstr::from_1Point2Vectors(glm::vec3(-3.0f, 0.5f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f))))); // Temporary test plane
}

void part::set_handle(entityHandle* handle_)
{
	mHandle = handle_;
	
	for_each([this](entity_ptr ent) {
		if(ent->handle())
			delete ent->handle();
			
		ent->set_handle(new entityHandle(ent, mHandle->view(), mHandle));	
	});
}
void part::add_sketch(sketch_ptr sk)
{
	if(sk) {
		set_require_redraw();
		sk->set_parent(this);
		mSketches.push_back(sk);
		if(mHandle)
			sk->set_handle(new entityHandle(sk, mHandle->view(), mHandle));
	}
}

void part::for_each(std::function<void (entity_ptr)> func)
{
	for(sketch_ptr sk : mSketches) {
		func(sk);
	}
	if(mOrigin)
		func(mOrigin);
}

sketch_ptr part::get_sketch(int ind)
{
	// It's kind of a funny behavior, I might change it to just return nullptr on out of bounds
	if(ind < 0 || ind >= mSketches.size()) {
		if(mSketches.empty())
			return nullptr;
		return mSketches.back();
	}
	return mSketches[ind]; 
}

void part::draw_impl(camera_ptr cam, int frame)
{

}

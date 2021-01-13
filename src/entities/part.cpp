
#include "part.hpp"

#include "entityView.hpp"
#include <utils/errorLogger.hpp>
#include "tangibleEntities/point.hpp"

part::part():
	entity(glm::ivec3(100, 0, 100))
{
	set_name("Part");
	init_scene();
}
part::part(entity* parent):
	entity(parent)
{
	set_name("Part");
	init_scene();
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

	std::shared_ptr<point> center = std::make_shared<point>(point_abstract(glm::vec3(0.0f, 0.0f, 0.0f)));
	center->set_name("originPoint");

	mOrigin = folder_ptr(new folder("origin"));
	
	add(mOrigin);
	mOrigin->add(center);
	mOrigin->add(mXY);
	mOrigin->add(mYZ);
	mOrigin->add(mZX);

	// std::cout<<glm::to_string(mXY->selectionID())<<"\n";
	// std::cout<<glm::to_string(mYZ->selectionID())<<"\n";
	// std::cout<<glm::to_string(mZX->selectionID())<<"\n";

	// add(mXY);
	// add(mYZ);
	// add(mZX);

	add(std::shared_ptr<plane>(new plane(
		plane_abstract::from_1Point2Vectors(glm::vec3(-3.0f, 0.5f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f))))); // Temporary test plane
}

void part::set_handle(entityHandle* handle_)
{
	mHandle = handle_;
	for(int i = 0; i < mChildren.size(); ++i) {
		if(std::get<2>(mChildren[i])) {
			if(std::get<1>(mChildren[i])->handle())
				delete std::get<1>(mChildren[i])->handle();
		
			std::get<1>(mChildren[i])->set_handle(new entityHandle(std::get<1>(mChildren[i]), mHandle->view(), mHandle));
		}
	}
}
void part::add(entity_ptr elem)
{
	sketch_ptr sk = std::dynamic_pointer_cast<sketch>(elem);
	if(sk)
		mSketches.push_back(sk);
	entity::add(elem);
	if(elem && mHandle) {
		elem->set_handle(new entityHandle(elem, mHandle->view(), mHandle));
	}
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

void part::draw_selection_impl(camera_ptr cam, int frame)
{

}
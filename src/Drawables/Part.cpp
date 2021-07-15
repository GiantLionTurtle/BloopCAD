
#include "Part.hpp"

#include "entityView.hpp"
#include <utils/DebugUtils.hpp>
#include "tangibleEntities/Point.hpp"

Part::Part():
	Drawable()
{
	set_name("Part");
	// init();
	// init_scene();
}

void Part::init()
{
	// Maybe this whole function should just be in the constructor
	
	// Create the 3 planes of the origin
	auto xy = new Plane(
		geom_3d::plane_abstr::from_1Point2Vectors(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	xy->set_name("XY plane");
	auto yz = new Plane(
		geom_3d::plane_abstr::from_1Point2Vectors(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), true)); // Inverted due to openGL's z axis being kind of backward
	yz->set_name("YZ plane");
	auto zx = new Plane(
		geom_3d::plane_abstr::from_1Point2Vectors(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	zx->set_name("ZX plane");

	Point* center = new Point(glm::vec3(0.0f, 0.0f, 0.0f));
	center->set_name("originPoint");

	mDrawList.add_origin(center);
	mDrawList.add_origin(xy);
	mDrawList.add_origin(yz);
	mDrawList.add_origin(zx);
	mDrawList.add_origin(new Plane(
		geom_3d::plane_abstr::from_1Point2Vectors(glm::vec3(-3.0f, 0.5f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f)))); // Temporary test plane
}

void Part::set_handle(entityHandle* handle_)
{
	mHandle = handle_;
	
	for(int i = 0; i < mDrawList.size(); ++i) {
		auto drw = mDrawList.at(i);
		if(drw->handle())
			delete drw->handle();
		drw->set_handle(new entityHandle(drw, mHandle->view(), mHandle));
	}
}
void Part::add_sketch(Sketch* sk)
{
	if(!sk) {
		LOG_WARNING("Tried adding a null sketch.");
		return;
	}
	set_need_redraw();
	sk->set_parent(this);
	mDrawList.add_sketch(sk);
	
	if(mHandle)
		sk->set_handle(new entityHandle(sk, mHandle->view(), mHandle));
}

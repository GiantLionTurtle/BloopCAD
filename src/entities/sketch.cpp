
#include "sketch.hpp"

#include <entities/tangibleEntities/line.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchPoint.hpp>
#include <utils/errorLogger.hpp>

sketch::sketch(plane_abstract_ptr base_plane):
	mBasePlane(base_plane)
{
	set_name("sketch");
	create_origin();
}
sketch::sketch(plane_abstract_ptr base_plane, entity* parent):
	mBasePlane(base_plane),
	entity(parent) // Follow that entity
{
	set_name("sketch");
	create_origin();
}

bool sketch::add_constraint(std::shared_ptr<constraint> cons) 
{
	mSystem.add_equations(cons->equations());
	mSystem.add_variables(cons->variables());
	bool success = mSystem.solve() >= 0;
	update();
	return success;
}

void sketch::draw_impl(camera_ptr cam, int frame)
{

}
void sketch::draw_selection_impl(camera_ptr cam, int frame)
{

}

void sketch::create_origin()
{
	mOrigin = folder_ptr(new folder("skorigin"));
	add(mOrigin);
	mOrigin->add(std::make_shared<sketchPoint>(glm::vec2(0.0f, 0.0f), mBasePlane));
	mOrigin->add(std::make_shared<line>(line_abstract(mBasePlane->origin() - mBasePlane->v(), mBasePlane->origin() + mBasePlane->v())));
	mOrigin->add(std::make_shared<line>(line_abstract(mBasePlane->origin() - mBasePlane->w(), mBasePlane->origin() + mBasePlane->w())));
}

#include "sketch.hpp"

#include <entities/tangibleEntities/sketchEntities/sketchLine.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchPoint.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchCircle.hpp>
#include <actions/common/serial_action.hpp>
#include <actions/common/moveCamera_action.hpp>
#include <actions/sketchDesign/enterSketchDesign_action.hpp>
#include <actions/partDesign/quitPartDesign_action.hpp>
#include <utils/errorLogger.hpp>
#include <bloop.hpp>
#include <document.hpp>

sketch::sketch(geom_3d::plane_abstr_ptr base_plane):
	mBasePlane(base_plane)
{
	set_name("sketch");
	create_origin();
}
sketch::sketch(geom_3d::plane_abstr_ptr base_plane, entity* parent):
	mBasePlane(base_plane),
	entity(parent) // Follow that entity
{
	set_name("sketch");
	create_origin();
}

void sketch::add_geometry(sketchEntity_ptr ent)
{
	if(ent) {
		set_require_redraw();
		ent->set_parent(this);
		mGeometries.push_back(ent);
	}
}
void sketch::for_each(std::function<void (entity_ptr)> func)
{
	for(sketchEntity_ptr ent : mGeometries) {
		func(ent);
	}
	func(mOrigin);
}

bool sketch::add_constraint(std::shared_ptr<constraint> cons) 
{
	mSystem.add_equations(cons->equations());
	mSystem.add_variables(cons->variables());

	if(mSystem.solve() >= 0) {
		update();
		// backup_system();
		return true;
	}
	return false;
}
bool sketch::add_constraint(subEquationsSystem const& subSystem)
{
	mSystem.add_equations(subSystem.equations);
	mSystem.add_variables(subSystem.variables);

	if(mSystem.solve() >= 0) {
		update();
		// backup_system();
		return true;
	}
	return false;
}
bool sketch::update_constraints()
{
	if(mSystem.solve() >= 0) {
		update();
		// backup_system();
		return true;
	}
	return false;
}

void sketch::backup_system()
{
	mSystemBackup = mSystem.state();
}
void sketch::revert_system_to_backup()
{
	mSystem.set(mSystemBackup);
	update();
}

void sketch::invoke_workspace(document* doc)
{
	doc->push_action(std::shared_ptr<serial_action>(new serial_action({
		std::shared_ptr<action>(new enterSketchDesign_action(shared_from_this(), true)),
		std::shared_ptr<action>(new quitPartDesign_action()),
		moveCamera_action::create_from_facingPlane(basePlane(), 8.0, doc->state()->cam->state(), nullptr)
	})));
}

void sketch::draw_impl(camera_ptr cam, int frame)
{

}

void sketch::create_origin()
{
	mOrigin = folder_ptr(new folder("skorigin"));

	add_geometry(std::make_shared<sketchLine>(glm::vec2(0.0f,  1.0f), glm::vec2(0.0f, -1.0f), mBasePlane, true));
	add_geometry(std::make_shared<sketchLine>(glm::vec2( 1.0f, 0.0f), glm::vec2(-1.0f, 0.0f), mBasePlane, true));
	add_geometry(std::make_shared<sketchPoint>(glm::vec2(0.0f, 0.0f), mBasePlane, true));

	// add(std::make_shared<sketchCircle>(circle_abstract(std::make_shared<sketchPoint>(glm::vec2(0.75f, 0.75f), mBasePlane), variable_ptr(new variable(0.5f))), mBasePlane));
}
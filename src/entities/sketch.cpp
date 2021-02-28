
#include "sketch.hpp"

#include <entities/tangibleEntities/sketchEntities/sketchLine.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchPoint.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchCircle.hpp>
#include <actions/common/serial_action.hpp>
#include <actions/common/moveCamera_action.hpp>
#include <actions/sketchDesign/enterSketchDesign_action.hpp>
#include <actions/partDesign/quitPartDesign_action.hpp>
#include <utils/errorLogger.hpp>
#include <utils/mathUtils.hpp>
#include <bloop.hpp>
#include <document.hpp>

sketch::sketch(geom_3d::plane_abstr_ptr base_plane):
	mBasePlane(base_plane)
{
	set_name("sketch");
	create_origin();

	// mSystem.set_solver(constraintSystem::LevenbergMarquardt);
}
sketch::sketch(geom_3d::plane_abstr_ptr base_plane, entity* parent):
	mBasePlane(base_plane),
	entity(parent) // Follow that entity
{
	set_name("sketch");
	create_origin();

	// mSystem.set_solver(constraintSystem::LevenbergMarquardt);
}

void sketch::print(int depth)
{
	for(int i = 0; i < depth; ++i) {
		std::cout<<'\t';
	}
	std::cout<<name()<<'\n';

	for(int i = 0; i < mGeometries.size(); ++i) {
		mGeometries[i]->print(depth+1);
	}
}

sketchEntity_ptr sketch::geometry_at_point(camera_ptr cam, glm::vec2 cursor)
{
	glm::vec2 planepos = mBasePlane->to_planePos(mBasePlane->line_intersection(cam->pos(), cam->cast_ray(cursor)));
	int maxpriority = -1;
	sketchEntity_ptr candidate = nullptr;
	for(sketchEntity_ptr geom : mGeometries) {
		geom->for_each([&](sketchEntity_ptr subgeom) {
			if(subgeom->selection_rank() > maxpriority && subgeom->in_selection_range(planepos, cam, cursor)) {
				maxpriority = subgeom->selection_rank();
				candidate = subgeom;
			}
		});
		if(geom->selection_rank() > maxpriority && geom->in_selection_range(planepos, cam, cursor)) {
			maxpriority = geom->selection_rank();
			candidate = geom;
		}
	}
	return candidate;
}

void sketch::add_geometry(sketchEntity_ptr ent)
{
	if(!ent) {
		LOG_WARNING("Trying to add null geometry.");
		return;
	}
	set_require_redraw();
	ent->set_parent(this);
	mGeometries.push_back(ent);
}
void sketch::add_toolPreviewGeometry(sketchEntity_ptr ent)
{
	if(!ent) {
		LOG_WARNING("Trying to add null geometry.");
		return;
	}
	set_require_redraw();
	ent->set_parent(this);
	mToolPreviewGeometries.push_back(ent);
}
void sketch::clear_toolPreviewGeometries()
{
	mToolPreviewGeometries.clear();
}

void sketch::for_each(std::function<void (entity_ptr)> func)
{
	for(sketchEntity_ptr ent : mGeometries) {
		func(ent);
	}
	for(sketchEntity_ptr ent : mToolPreviewGeometries) {
		func(ent);
	}
	func(mOrigin);
}

bool sketch::add_constraint(std::shared_ptr<constraint_abstract> cons, sketchEntity_ptr immovable_hint) 
{
	mSystem.add_constraint(cons);

	if(immovable_hint) {
		immovable_hint->set_tmpConstant(true);
		backup_system();
		int output = mSystem.solve();
		immovable_hint->set_tmpConstant(false);
		if(output == constraintSystem::SUCCESS) {
			update();
			return true;
		}
		revert_system_to_backup();
	}

	backup_system();
	if(mSystem.solve() == constraintSystem::solveOutput::SUCCESS) {
		update();
		return true;
	} 
	revert_system_to_backup();
	LOG_WARNING("*cries*");
	return false;
}

bool sketch::update_constraints()
{
	backup_system();
	if(mSystem.solve() == constraintSystem::SUCCESS) {
		update();
		return true;
	}
	revert_system_to_backup();
	return false;
}

void sketch::backup_system()
{
	mSystem.varState(mSystemBackup);
}
void sketch::revert_system_to_backup()
{
	mSystem.set_varState(mSystemBackup);
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

	add_geometry(std::make_shared<sketchLine>(glm::vec2(0.0f,  1.0f), glm::vec2(0.0f, -1.0f), basePlane(), true));
	add_geometry(std::make_shared<sketchLine>(glm::vec2( 1.0f, 0.0f), glm::vec2(-1.0f, 0.0f), basePlane(), true));
	add_geometry(std::make_shared<sketchPoint>(glm::vec2(0.0f, 0.0f), mBasePlane, true));

	// add(std::make_shared<sketchCircle>(circle_abstract(std::make_shared<sketchPoint>(glm::vec2(0.75f, 0.75f), mBasePlane), variable_ptr(new variable(0.5f))), mBasePlane));
}
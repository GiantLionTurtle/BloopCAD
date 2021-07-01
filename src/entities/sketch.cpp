
#include "sketch.hpp"

#include <constraintsSolver/solverState.hpp>
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
	mBasePlane(base_plane),
	mSystem(1)
{
	set_name("sketch");
	// create_origin();

	// mSystem.set_solver(constraintSystem::LevenbergMarquardt);
}
sketch::sketch(geom_3d::plane_abstr_ptr base_plane, entity* parent):
	entity(parent), // Follow that entity
	mBasePlane(base_plane),
	mSystem(1)
{
	set_name("sketch");
	// create_origin();

	// mSystem.set_solver(constraintSystem::LevenbergMarquardt);
}

void sketch::init()
{
	mOrigin = folder_ptr(new folder("skorigin"));

	add_geometry(std::make_shared<sketchLine>(glm::vec2(0.0f,  1.0f), glm::vec2(0.0f, -1.0f), basePlane(), true));
	add_geometry(std::make_shared<sketchLine>(glm::vec2( 1.0f, 0.0f), glm::vec2(-1.0f, 0.0f), basePlane(), true));
	add_geometry(std::make_shared<sketchPoint>(glm::vec2(0.0f, 0.0f), mBasePlane, true));

	// add(std::make_shared<sketchCircle>(circle_abstract(std::make_shared<sketchPoint>(glm::vec2(0.75f, 0.75f), mBasePlane), var_ptr(new variable(0.5f))), mBasePlane));
}

void sketch::notify(int msg)
{
	if(msg == DELETED || msg == RESURRECTED)
		mSystem.updatedSystem();
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

sketchEntity_ptr sketch::entity_at_point(camera_ptr cam, glm::vec2 cursor)
{
	// TODO: make this whole thingy less sketch
	glm::vec2 planepos = mBasePlane->to_planePos(mBasePlane->line_intersection(cam->pos(), cam->cast_ray(cursor)));
	int maxpriority = -1;
	sketchEntity_ptr candidate = nullptr;
	for(sketchGeometry_ptr geom : mGeometries) {
		geom->for_each([&](sketchGeometry_ptr subgeom) {
			if(subgeom->selection_rank() > maxpriority && subgeom->visible() && subgeom->in_selection_range(planepos, cam, cursor)) {
				maxpriority = subgeom->selection_rank();
				candidate = subgeom;
			}
			subgeom->for_each_annot([&](sketchEntity_ptr annot) {
				if(annot->selection_rank() > maxpriority && annot->visible() && annot->in_selection_range(planepos, cam, cursor)) {
					maxpriority = annot->selection_rank();
					candidate = annot;
				}
			});
		});
		if(geom->selection_rank() > maxpriority && geom->visible() && geom->in_selection_range(planepos, cam, cursor)) {
			maxpriority = geom->selection_rank();
			candidate = geom;
		}
		geom->for_each_annot([&](sketchEntity_ptr annot) {
			if(annot->selection_rank() > maxpriority && annot->visible() && annot->in_selection_range(planepos, cam, cursor)) {
				maxpriority = annot->selection_rank();
				candidate = annot;
			}
		});
	}
	return candidate;
}

void sketch::add_geometry(sketchGeometry_ptr ent)
{
	if(!ent) {
		LOG_WARNING("Trying to add null geometry.");
		return;
	}
	set_require_redraw();
	ent->set_parent(this);
	mGeometries.push_back(ent);
}
std::vector<entityPosSnapshot_ptr> sketch::geometriesSnapshots()
{
	std::vector<entityPosSnapshot_ptr> snapshots(0);
	for(int i = 0; i < mGeometries.size(); ++i) {
		auto snp = mGeometries[i]->posSnapshot();
		if(snp)
			snapshots.push_back(snp);
	}
	return snapshots;
}
std::vector<entityPosSnapshot_ptr> sketch::selectedGeometriesSnapshots()
{
	std::vector<entityPosSnapshot_ptr> snapshots(0);
	for(int i = 0; i < mSelectedEntities.size(); ++i) {
		auto snp = mSelectedEntities[i]->posSnapshot();
		if(snp)
			snapshots.push_back(snp);
	}
	return snapshots;
}
void sketch::add_toolPreview(entity_ptr ent)
{
	if(!ent) {
		LOG_WARNING("Trying to add null entity.");
		return;
	}
	set_require_redraw();
	ent->set_parent(this);
	mToolPreview.push_back(ent);
}
void sketch::clear_toolPreview()
{
	mToolPreview.clear();
	set_require_redraw();
}

void sketch::add_selectedGeometry(sketchEntity_ptr ent)
{
	if(!ent) {
		LOG_WARNING("Trying to add null geometry.");
		return;
	}
	ent->select();
	set_require_redraw();
	mSelectedEntities.push_back(ent);
}
void sketch::remove_selectedGeometry(sketchEntity_ptr ent)
{
	for(int i = 0; i < mSelectedEntities.size(); ++i) {
		if(ent == mSelectedEntities[i]) {
			ent->unselect();
			mSelectedEntities.erase(std::find(mSelectedEntities.begin(), mSelectedEntities.end(), ent));
		}
	}
}
void sketch::clear_selectedGeometries()
{
	for_each_selected([](sketchEntity_ptr ent) { ent->unselect(); });
	mSelectedEntities.clear();
}

bool sketch::can_delete(sketchEntity_ptr ent)
{
	if((ent->parent() == this && ent != mGeometries[0] && ent != mGeometries[1] && ent != mGeometries[2]) || ent->type() < 0) {
		return true;
	}
	return false;
}

void sketch::for_each(std::function<void (entity_ptr)> func)
{
	for(sketchGeometry_ptr geom : mGeometries) {
		func(geom);
	}
	for(entity_ptr ent : mToolPreview) {
		func(ent);
	}
	for(auto constr : mConstraints) {
		func(constr);
	}
	// func(mOrigin);
}
void sketch::for_each_selected(std::function<void (sketchEntity_ptr)> func)
{
	for(sketchEntity_ptr geom : mSelectedEntities) {
		func(geom);
	}
}
void sketch::toggle_selection_from_area(glm::vec2 a, glm::vec2 b, bool contained)
{
	glm::vec2 low_right(std::max(a.x, b.x), std::min(a.y, b.y)), high_left(std::min(a.x, b.x), std::max(a.y, b.y));

	for(sketchEntity_ptr geom : mGeometries) {
		bool in_sel = geom->in_selection_range(low_right, high_left, contained);
		if(in_sel && !geom->selected()) {
			add_selectedGeometry(geom);
		} else if(!in_sel && geom->selected()) {
			remove_selectedGeometry(geom);
		}
			
		geom->for_each([&](sketchEntity_ptr subgeom) {
			in_sel = subgeom->in_selection_range(low_right, high_left, contained);
			if(in_sel && !subgeom->selected()) {
				add_selectedGeometry(subgeom);
			} else if(!in_sel && subgeom->selected()) {
				remove_selectedGeometry(subgeom);
			}
		});
	}
}

bool sketch::add_constraint(std::shared_ptr<constraint_entity> constr, sketchEntity_ptr immovable_hint) 
{
	if(!constr)
		return false;
	mConstraints.push_back(constr);
	mSystem.add_constraint(constr);
	constr->set_parent(this);
	mHandle->update_name(mName + "(" + std::to_string(mSystem.num_constraints()) + ",  " + std::to_string(mSystem.num_variables()) + ")");

	if(immovable_hint) {
		immovable_hint->set_tmpConstant(true);
		bool out = update_constraints(true, false);
		immovable_hint->set_tmpConstant(false);
		if(out) 
			return true;
	}

	if(update_constraints(true, false))
		return true;
	LOG_WARNING("*cries*");
	return false;
}

bool sketch::toggle_constraint(std::shared_ptr<constraint_entity> constr, bool enable)
{
	constr->set_exists(enable);
	mSystem.toggle_constraint(constr, enable);
	mHandle->update_name(mName + "(" + std::to_string(mSystem.num_constraints()) + ",  " + std::to_string(mSystem.num_variables()) + ")");

	if(update_constraints(true, false))
		return true;

	LOG_WARNING(std::string("Could not ") + std::string(enable == true ? "enable" : "disable") + std::string("constraint T_T"));
	return false;
}

bool sketch::update_constraints(bool safeUpdate, bool update_on_solveFail)
{
	if(safeUpdate)
		backup_system();
	int solve_out = mSystem.solve();
	if(update_on_solveFail || solve_out == solverState::SUCCESS) {
		update();
	} else if(safeUpdate && solve_out != solverState::SUCCESS) {
		revert_system_to_backup();
	}
	return solve_out == solverState::SUCCESS;
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
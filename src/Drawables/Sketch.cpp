
#include "Sketch.hpp"

#include <constraintsSolver/solverState.hpp>
#include "sk/SkPoint.hpp"
#include "sk/SkLine.hpp"
#include <actions/common/serial_action.hpp>
#include <actions/common/moveCamera_action.hpp>
#include <actions/sketchDesign/enterSketchDesign_action.hpp>
#include <actions/partDesign/quitPartDesign_action.hpp>
#include <actions/common/toggleBaseObject_action.hpp>
#include <utils/DebugUtils.hpp>
#include <utils/mathUtils.hpp>
#include <bloop.hpp>
#include <document.hpp>

Sketch::Sketch(Geom3d::plane_abstr* base_plane):
	mBasePlane(base_plane),
	mSystem(1)
{
	set_name("Sketch");
	// create_origin();

	// mSystem.set_solver(constraintSystem::LevenbergMarquardt);
}

void Sketch::init_impl()
{
	// mOrigin = new Folder("skorigin");

	add_geometry(new SkLine(glm::vec2(0.0f,  1.0f), glm::vec2(0.0f, -1.0f), basePlane(), true));
	add_geometry(new SkLine(glm::vec2( 1.0f, 0.0f), glm::vec2(-1.0f, 0.0f), basePlane(), true));
	add_geometry(new SkPoint(glm::vec2(0.0f, 0.0f), mBasePlane, true));

	mDrawList.init_newElems();
	// add(std::make_shared<sketchCircle>(circle_abstract(std::make_shared<sketchPoint>(glm::vec2(0.75f, 0.75f), mBasePlane), var_ptr(new variable(0.5f))), mBasePlane));
}

void Sketch::notify(Drawable* who, int msg, bool child)
{
	if(!child)
		return;
	switch(msg) {
	case DELETED:
	case RESURRECTED:
		mSystem.updatedSystem();
		break;
	default:
		break;
	}
}

SelectionPoint Sketch::closest(glm::vec2 cursor, Camera* cam, int filter)
{
	glm::vec2 planepos = mBasePlane->to_planePos(mBasePlane->line_intersection(cam->pos(), cam->cast_ray(cursor)));
	int maxpriority = -1;
	SelectionPoint selPt;
	for(size_t i = 0; i < mDrawList.size(); ++i) {
		SkDrawable* ch = mDrawList.at(i);
		if(ch->visible() && ch->selection_rank() > maxpriority) {
			SelectionPoint tmpSelPt = ch->closest_2d(planepos, cam, cursor, filter);
			if(tmpSelPt.ent) {
				selPt = tmpSelPt;
				maxpriority = tmpSelPt.ent->selection_rank();
			}
		}
	}
	return selPt;
}

DraggableSelectionPoint Sketch::closest_draggable(glm::vec2 cursor, Camera* cam, int filter)
{
	glm::vec2 planepos = mBasePlane->to_planePos(mBasePlane->line_intersection(cam->pos(), cam->cast_ray(cursor)));
	int maxpriority = -1;
	DraggableSelectionPoint selPt;
	for(size_t i = 0; i < mDrawList.size(); ++i) {
		SkDrawable* ch = mDrawList.at(i);
		if(ch->visible() && ch->selection_rank() > maxpriority) {
			DraggableSelectionPoint tmpSelPt = ch->closest_2d_draggable(planepos, cam, cursor, filter);
			if(tmpSelPt.ent) {
				selPt = tmpSelPt;
				maxpriority = tmpSelPt.ent->selection_rank();
			}
		}
	}
	return selPt;
}

void Sketch::move_selected(glm::vec2 start, glm::vec2 end, glm::vec2 pix_mov)
{
	for(size_t i = 0; i < mDrawList.size(); ++i) {
		auto ch = mDrawList.at(i);
		if(ch->exists())
			ch->move_selected(start, end, pix_mov);	
	}
}
std::shared_ptr<serial_action> Sketch::delete_selected()
{
	auto act = std::make_shared<serial_action>();
	for(size_t i = 0; i < mDrawList.size(); ++i) {
		auto ch = mDrawList.at(i);
		if(ch->selected() && ch->exists())
			act->add_action(std::make_shared<toggleBaseObject_action>(ch, false));
	}
	return act;
}
void Sketch::unselect_all()
{
	for(size_t i = 0; i < mDrawList.size(); ++i) {
		auto ch = mDrawList.at(i);
		if(ch->selected() && ch->exists())
			ch->unselect();
	}
}

void Sketch::add_geometry(SkGeometry* ent)
{
	if(!ent) {
		LOG_WARNING("Trying to add null geometry.");
		return;
	}
	set_need_redraw();
	ent->set_parent(this);
	mDrawList.add_geom(ent);
}

// void Sketch::add_selectedGeometry(sketchEntity* ent)
// {
// 	if(!ent) {
// 		LOG_WARNING("Trying to add null geometry.");
// 		return;
// 	}
// 	mSelectedEntities.insert(ent);
// }
// void Sketch::remove_selectedGeometry(sketchEntity* ent)
// {
// 	mSelectedEntities.erase(ent);
// }
// void Sketch::clear_selectedGeometries()
// {
// 	for(auto ent : mSelectedEntities) {
// 		ent->unselect();
// 	}
// 	mSelectedEntities.clear();
// }

bool Sketch::can_delete(SkDrawable* ent)
{
	if((ent->parent() == this && ent != mDrawList.geom(0) && ent != mDrawList.geom(1) && ent != mDrawList.geom(2))) {
		return true;
	}
	return false;
}

// void Sketch::toggle_selection_from_area(glm::vec2 a, glm::vec2 b, bool contained)
// {
// 	glm::vec2 low_right(std::max(a.x, b.x), std::min(a.y, b.y)), high_left(std::min(a.x, b.x), std::max(a.y, b.y));

// 	for(size_t i = 0; i < mDrawList.num_geom(); ++i) {
// 		auto geom = mDrawList.geom(i);
// 		bool in_sel = geom->in_selection_range(low_right, high_left, contained);
// 		if(in_sel && !geom->selected()) {
// 			geom->select();
// 		} else if(!in_sel && geom->selected()) {
// 			geom->unselect();
// 		}
// 	}
// }

bool Sketch::add_constraint(SkConstraint* constr, SkDrawable* immovable_hint) 
{
	if(!constr)
		return false;
	mDrawList.add_constr(constr);
	mSystem.add_constraint(constr);
	constr->set_parent(this);
	mHandle->update_name(mName + "(" + std::to_string(mSystem.num_constraints()) + ",  " + std::to_string(mSystem.num_variables()) + ")");

	if(immovable_hint) {
		// BLOOPBLOOPCHECK

		// immovable_hint->set_tmpConstant(true);
		// bool out = update_constraints(true, false);
		// immovable_hint->set_tmpConstant(false);
		// if(out) 
			// return true;
	}

	if(update_constraints(true, false))
		return true;
	LOG_WARNING("*cries*");
	return false;
}

bool Sketch::toggle_constraint(SkConstraint* constr, bool enable)
{
	if(!constr)
		return false;
	constr->set_exists(enable);
	mSystem.toggle_constraint(constr, enable);
	mHandle->update_name(mName + "(" + std::to_string(mSystem.num_constraints()) + ",  " + std::to_string(mSystem.num_variables()) + ")");

	if(update_constraints(true, false))
		return true;

	LOG_WARNING(std::string("Could not ") + std::string(enable == true ? "enable" : "disable") + std::string("constraint T_T"));
	return false;
}

bool Sketch::update_constraints(bool safeUpdate, bool update_on_solveFail)
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

std::map<var_ptr, float> Sketch::snapshot()
{
	std::map<var_ptr, float> shot;
	mSystem.varState(shot);
	return shot;
}
std::vector<VarState> Sketch::deltaSnapshot(std::map<var_ptr, float> first)
{
	std::vector<VarState> delta;
	mSystem.varDelta(first, delta);
	return delta;
}
void Sketch::apply_snapshot(std::map<var_ptr, float> shot)
{
	for(auto it = shot.begin(); it != shot.end(); ++it) {
		it->first->set(it->second);
	}
}
void Sketch::apply_snapshot(std::vector<VarState> shot)
{
	for(auto vst : shot) {
		vst.var->set(vst.st);
	}
}
void Sketch::apply_deltaSnapshot(std::vector<VarState> deltaShot)
{
	for(auto vst : deltaShot) {
		vst.var->set(vst.var->eval() + vst.st);
	}
}
void Sketch::apply_invDeltaSnapshot(std::vector<VarState> deltaShot)
{
	for(auto vst : deltaShot) {
		vst.var->set(vst.var->eval() - vst.st);
	}
}
void Sketch::backup_system()
{
	mSystem.varState(mSystemBackup);
}
void Sketch::revert_system_to_backup()
{
	apply_snapshot(mSystemBackup);
}

void Sketch::invoke_workspace(document* doc)
{
	doc->push_action(std::shared_ptr<serial_action>(new serial_action({
		std::shared_ptr<action>(new enterSketchDesign_action(this, true)),
		std::shared_ptr<action>(new quitPartDesign_action()),
		moveCamera_action::create_from_facingPlane(basePlane(), 8.0, doc->state()->cam->state(), nullptr)
	})));
}
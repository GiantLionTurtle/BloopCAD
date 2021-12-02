
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "Sketch.hpp"

#include <ConstrsSolver/SolverState.hpp>
#include <Drawables/Sk/SkPoint.hpp>
#include <Drawables/Sk/SkLine.hpp>

#include <Actions/Common/Serial_action.hpp>
#include <Actions/Common/MoveCamera_action.hpp>
#include <Actions/Sketch/EnterSketch_action.hpp>
#include <Actions/Part/QuitPart_action.hpp>
#include <Actions/Common/ToggleBaseObject_action.hpp>

#include <Utils/Debug_util.hpp>
#include <Utils/Maths_util.hpp>
#include <Bloop/Bloop.hpp>
#include <Workspaces/Document.hpp>

Sketch::Sketch(Geom3d::Plane_abstr* base_plane)
	: mBasePlane(base_plane)
	, mConstrSystem(SolverState::algorithm::DogLeg)//this),
	, mSolve_allowed(true)
{
	set_name("Sketch");
	// create_origin();

	// mConstrSystem.set_solver(ConstraintsSystem::LevenbergMarquardt);
}

void Sketch::init_impl()
{
	// mOrigin = new Folder("skorigin");

	add_geometry(new SkLine(glm::vec2(0.0f,  1.0f), glm::vec2(0.0f, -1.0f), basePlane(), true));
	add_geometry(new SkLine(glm::vec2( 1.0f, 0.0f), glm::vec2(-1.0f, 0.0f), basePlane(), true));
	add_geometry(new SkPoint(glm::vec2(0.0f, 0.0f), mBasePlane, true));

	mDrawList.init_newElems();
}

void Sketch::notify(Drawable* who, int msg, bool child)
{
	if(!child)
		return;
	switch(msg) {
	case DELETED:
	case RESURRECTED:
		mConstrSystem.force_decomposition();
		break;
	default:
		break;
	}
}
void Sketch::dragUpdate()
{
	mConstrSystem.force_decomposition();
	update();
}

SelPoint Sketch::closest(glm::vec2 cursor, Camera* cam, int filter)
{
	glm::vec2 planepos = mBasePlane->to_planePos(mBasePlane->line_intersection(cam->pos(), cam->cast_ray(cursor)));
	int maxpriority = -1;
	SelPoint selPt;
	for(size_t i = 0; i < mDrawList.size(); ++i) {
		auto ch = mDrawList.at(i);
		if(ch->visible() && ch->selection_rank() > maxpriority && ch->closest_2d(selPt, planepos, cam, cursor, filter)) {
			maxpriority = selPt.ent->selection_rank();
		}
	}
	return selPt;
}

bool Sketch::closest_draggable(SkGeomDragPoint& selP, glm::vec2 cursor, Camera* cam, int filter)
{
	glm::vec2 planepos = mBasePlane->to_planePos(mBasePlane->line_intersection(cam->pos(), cam->cast_ray(cursor)));
	int maxpriority = -1;
	for(size_t i = 0; i < mDrawList.n_geoms(); ++i) {
		SkGeometry* ch = mDrawList.geom(i);
		if(ch->visible() && ch->selection_rank() > maxpriority && ch->closest_2d_draggable(selP, planepos, cam, cursor, filter)) {
			maxpriority = selP.geom->selection_rank();
		}
	}
	return maxpriority != -1; // Success if maxpriority has been updated
}
SkConstrAnnot* Sketch::closest_annot(glm::vec2 cursor, Camera* cam)
{
	glm::vec2 planepos = mBasePlane->to_planePos(mBasePlane->line_intersection(cam->pos(), cam->cast_ray(cursor)));
	for(int i = 0; i < mDrawList.n_annots(); ++i) {
		SkConstrAnnot* annot = mDrawList.annot(i);
		if(annot->visible() && annot->inbound(planepos, cam, cursor))
			return annot;
	}
	return nullptr;
}
void Sketch::move_selGeom(glm::vec2 delta)
{
	for(size_t i = 0; i < mDrawList.size(); ++i) {
		auto ch = mDrawList.at(i);
		if(ch->exists())
			ch->move_selected(delta);	
	}
	update_constraints(false, true);
}
std::shared_ptr<Serial_action> Sketch::delete_selected()
{
	auto act = std::make_shared<Serial_action>();
	for(size_t i = 0; i < mDrawList.size(); ++i) {
		auto ch = mDrawList.at(i);
		if(ch->selected() && ch->exists())
			act->add_action(std::make_shared<ToggleBaseObject_action>(ch, false));
	}
	return act;
}
void Sketch::unselect_all()
{
	for(size_t i = 0; i < mDrawList.size(); ++i) {
		auto ch = mDrawList.at(i);
		ch->unselect();
	}
}
size_t Sketch::n_selGeom()
{
	size_t running_sum = 0;
	for(size_t i = 0; i < mDrawList.size(); ++i) {
		if(mDrawList.at(i)->selected())
			running_sum++;
	}
	return running_sum;
}

void Sketch::add_geometry(SkGeometry* geom)
{
	if(!geom) {
		LOG_WARNING("Trying to add null geometry.");
		return;
	}
	set_need_redraw();
	mDrawList.add_geom(geom);	
}

bool Sketch::can_delete(SkDrawable* ent)
{
	if((ent->parent() == this && ent != mDrawList.geom(0) && ent != mDrawList.geom(1) && ent != mDrawList.geom(2))) {
		return true;
	}
	return false;
}

void Sketch::add_annot(SkConstrAnnot* annot)
{
	mDrawList.add_annot(annot);
	set_need_redraw();
}
bool Sketch::add_constraint(Constraint* constr, SkDrawable* immovable_hint) 
{
	if(!constr)
		return false;
	// mDrawList.add_constr(constr);
	mConstrSystem.add_constr(constr);
	// constr->set_parent(this);
	// mHandle->update_name(mName + "(" + std::to_string(mConstrSystem.n_constraints()) + ",  " + std::to_string(mConstrSystem.n_variables()) + ")");

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

bool Sketch::toggle_constraint(Constraint* constr, bool enable)
{
	if(!constr)
		return false;
	constr->set_exists(enable);
	// mConstrSystem.toggle_constraint(constr, enable);
	// mHandle->update_name(mName + "(" + std::to_string(mConstrSystem.n_constraints()) + ",  " + std::to_string(mConstrSystem.n_variables()) + ")");

	if(update_constraints(true, false))
		return true;

	LOG_WARNING(std::string("Could not ") + std::string(enable == true ? "enable" : "disable") + std::string("constraint T_T"));
	return false;
}

bool Sketch::update_constraints(bool safeUpdate, bool update_on_solveFail)
{
	if(!mSolve_allowed)
		return true;
	if(safeUpdate)
		backup_system();
	int solve_out = mConstrSystem.solve();
	if(update_on_solveFail || solve_out != SolverState::FAILURE) {
		update(true);
	} else if(safeUpdate && solve_out == SolverState::FAILURE) {
		revert_system_to_backup();
	}
	return solve_out == SolverState::SUCCESS;
}

std::map<Param*, float> Sketch::snapshot()
{
	std::map<Param*, float> shot;
	// mConstrSystem.varState(shot);
	return shot;
}
std::vector<ParamDualState> Sketch::deltaSnapshot(std::map<Param*, float> first)
{
	std::vector<ParamDualState> delta;
	// mConstrSystem.varDelta(first, delta);
	return delta;
}
void Sketch::apply_snapshot(std::map<Param*, float> shot)
{
	for(auto it = shot.begin(); it != shot.end(); ++it) {
		it->first->set(it->second);
	}
	update(true);
}
void Sketch::apply_snapshot(std::vector<ParamState> shot)
{
	for(auto vst : shot) {
		vst.var->set(vst.st);
	}
	update(true);
}
void Sketch::apply_deltaSnapshot(std::vector<ParamDualState> deltaShot, bool first)
{
	for(auto vst : deltaShot) {
		vst.var->set(first ? vst.st1 : vst.st2);
	}
	update(true);
}

void Sketch::backup_system()
{
	// mConstrSystem.varState(mConstrSystemBackup);
}
void Sketch::revert_system_to_backup()
{
	// apply_snapshot(mConstrSystemBackup);
}

void Sketch::invoke_workspace(Document* doc)
{
	doc->push_action(std::shared_ptr<Serial_action>(new Serial_action({
		Action_ptr(new EnterSketch_action(this, true)),
		Action_ptr(new QuitPart_action()),
		MoveCamera_action::create_from_facingPlane(basePlane(), 8.0, doc->state()->cam->state(), nullptr)
	})));
}
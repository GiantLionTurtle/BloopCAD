
// BloopCAD
// Copyright (C) 2020  BloopCorp

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

#ifndef SKETCH_HPP_
#define SKETCH_HPP_

#include <Drawables/Base/Collection.hpp>
#include <Geometry/3d/Plane_abstr.hpp>
#include <Drawables/Sk/SkDrawable.hpp>
#include <Drawables/Sk/SkGeometry.hpp>
#include <Drawables/Sk/SkConstrAnnot.hpp>
#include <Actions/Common/Serial_action.hpp>
#include <Drawables/Sk/SkConstrAnnot.hpp>
#include <ConstrsSolver/ConstrSyst.hpp>

#include <memory>
#include <vector>

class SkIndexer : public Indexer_abstr {
private:
	std::vector<SkGeometry*> mGeometries;
	std::vector<SkConstrAnnot*> mAnnotations;
	int mInitInd_geom, mInitInd_constr;
public:
	SkIndexer(Drawable* driven)
		: Indexer_abstr(driven)
		, mInitInd_geom(0)
		, mInitInd_constr(0)
	{

	}
	/*
		NOTE: 	the indices validity is never checked within this class
				because std::vector has a proper try-catch system (if the at() function is used), 
				to not cause overhead in other parts of the code (checking for nullptr)
				and to simplify debugging
	*/
	size_t size() { return n_geoms() + n_annots(); }
	SkIntDrawable* at(size_t ind)
	{
		if(ind < n_geoms()) {
			return mGeometries.at(ind);
		} else {
			return mAnnotations.at(ind - n_geoms());
		}
	}

	bool has_newElems() { return mInitInd_geom < n_geoms() || mInitInd_constr < n_annots(); }
	void init_newElems()
	{
		init_newElems_stat(mGeometries, mInitInd_geom, driven());
		init_newElems_stat(mAnnotations, mInitInd_constr, driven());
	}

	size_t n_geoms() { return mGeometries.size(); }
	SkGeometry* geom(size_t ind) { return mGeometries.at(ind); }
	void add_geom(SkGeometry* g) { mGeometries.push_back(g); mDriven->update(); }

	size_t n_annots() { return mAnnotations.size(); }
	SkConstrAnnot* annot(size_t ind) { return mAnnotations.at(ind); }
	void add_annot(SkConstrAnnot* c) { mAnnotations.push_back(c); mDriven->update(); }
};


/*
	@class sketch describes a sketch which is a bunch of geometries with constraints
	@parent : entity
*/
class Sketch : public Collection_abstr<SkIndexer>/*, public DragSystemHandler*/ {
public:
private:
	Geom3d::Plane_abstr* mBasePlane; // Plane onto which every geometry is added, maybe it should descend from Plane_abstract..

	// Folder* mOrigin;
	// DragEnabled_ConstraintsSystem<2> mConstrSystem;
	ConstrSyst mConstrSystem;
	std::map<Constraint*, Drawable*> mConstr2Annot;
	std::vector<VarState> mConstrSystemBackup;

	bool mSolve_allowed; // THIS IS FOR DEBUG IT SHOULD GO AWAY
public:
	/*
		@function sketch creates a sketch

		@param base_plane : The plane onto which everything is constructed in the sketch
	*/
	Sketch(Geom3d::Plane_abstr* base_plane);

	void init_impl();

	void notify(Drawable* who, int msg, bool child);
	void dragUpdate();

	SelPoint closest(glm::vec2 cursor, Camera* cam, int filter);
	bool closest_draggable(SkGeomDragPoint& selP, glm::vec2 cursor, Camera* cam, int filter);
	SkConstrAnnot* closest_annot(glm::vec2 cursor, Camera* cam);
	void move_selGeom(glm::vec2 delta);
	std::shared_ptr<Serial_action> delete_selected(); 
	void unselect_all();
	size_t n_selGeom();

	void add_geometry(SkGeometry* geom);

	// BLOOPBLOOPWACK
	bool can_delete(SkDrawable* ent);

	/*
		@function basePlane

		@return : The base plane of the sketch
	*/
	Geom3d::Plane_abstr* basePlane() { return mBasePlane; }

	// Folder* origin() const { return mOrigin; }

	void add_annot(SkConstrAnnot* annot);
	bool add_constraint(Constraint* constr, SkDrawable* immovable_hint = nullptr);
	bool toggle_constraint(Constraint* constr, bool enable);
	bool update_constraints(bool safeUpdate, bool update_on_solveFail);

	std::map<Param*, float> snapshot();
	std::vector<ParamDualState> deltaSnapshot(std::map<Param*, float> first);
	void apply_snapshot(std::map<Param*, float> shot);
	void apply_snapshot(std::vector<ParamState> shot);
	void apply_deltaSnapshot(std::vector<ParamDualState> deltaShot, bool first);
	void backup_system();
	void revert_system_to_backup();

	virtual void invoke_workspace(Document* doc);

	void toggle_solve() { mSolve_allowed = !mSolve_allowed; }
	bool solve_allowed() { return mSolve_allowed; }
};

#endif
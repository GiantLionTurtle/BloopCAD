
#ifndef SKETCH_HPP_
#define SKETCH_HPP_

#include "Collection.hpp"
#include <geometry/geometry_3d/plane_abstr.hpp>
#include <Drawables/Folder.hpp>
#include <constraintsSolver/constraintSystem.hpp>
#include "sk/SkDrawable.hpp"
#include "sk/SkGeometry.hpp"
#include "sk/SkConstraint.hpp"
#include <actions/common/serial_action.hpp>

#include <memory>
#include <vector>

class SkIndexer : public Indexer_abstr {
private:
	std::vector<SkGeometry*> mGeometries;
	std::vector<SkConstraint*> mConstraints;
	int mInitInd_geom, mInitInd_constr;
public:
	SkIndexer(Drawable* driven):
		Indexer_abstr(driven),
		mInitInd_geom(0),
		mInitInd_constr(0)
	{

	}
	/*
		NOTE: 	the indices validity is never checked within this class
				because std::vector has a proper try-catch system (if the at() function is used), 
				to not cause overhead in other parts of the code (checking for nullptr)
				and to simplify debugging
	*/
	size_t size() { return num_geom() + num_constr(); }
	SkDrawable* at(size_t ind)
	{
		if(ind < num_geom()) {
			return mGeometries.at(ind);
		} else {
			return mConstraints.at(ind - num_geom());
		}
	}

	bool has_newElems() { return mInitInd_geom < mGeometries.size() || mInitInd_constr < mConstraints.size(); }
	void init_newElems()
	{
		init_newElems_stat(mGeometries, mInitInd_geom, driven());
		init_newElems_stat(mConstraints, mInitInd_constr, driven());
	}

	size_t num_geom() { return mGeometries.size(); }
	SkGeometry* geom(size_t ind) { return mGeometries.at(ind); }
	void add_geom(SkGeometry* g) { mGeometries.push_back(g); mDriven->set_need_update(); }

	size_t num_constr() { return mConstraints.size(); }
	SkConstraint* constr(size_t ind) { return mConstraints.at(ind); }
	void add_constr(SkConstraint* c) { mConstraints.push_back(c); mDriven->set_need_update(); }
};

/*
	@class sketch describes a sketch which is a bunch of geometries with constraints
	@parent : entity
*/
class Sketch : public Collection_abstr<SkIndexer> {
public:
private:
	Geom3d::plane_abstr* mBasePlane; // Plane onto which every geometry is added, maybe it should descend from plane_abstract..

	// Folder* mOrigin;
	constraintSystem mSystem;
	std::vector<VarState> mSystemBackup;
public:
	/*
		@function sketch creates a sketch

		@param base_plane : The plane onto which everything is constructed in the sketch
	*/
	Sketch(Geom3d::plane_abstr* base_plane);

	void init_impl();

	void notify(Drawable* who, int msg, bool child);

	SelectionPoint closest(glm::vec2 cursor, Camera* cam, int filter);
	DraggableSelectionPoint closest_draggable(glm::vec2 cursor, Camera* cam, int filter);
	void move_selected(glm::vec2 start, glm::vec2 end, glm::vec2 pix_mov);
	std::shared_ptr<serial_action> delete_selected(); 
	void unselect_all();

	void add_geometry(SkGeometry* geom);
	
	// void add_toolPreview(Drawable* ent);
	// void clear_toolPreview();

	// void add_selectedGeometry(sketchEntity* ent);
	// void remove_selectedGeometry(sketchEntity* ent);
	// void clear_selectedGeometries();
	// size_t num_selectedGeometries() { return mSelectedEntities.size(); }

	// BLOOPBLOOPWACK
	bool can_delete(SkDrawable* ent);

	// void toggle_selection_from_area(glm::vec2 a, glm::vec2 b, bool contained);

	/*
		@function basePlane

		@return : The base plane of the sketch
	*/
	Geom3d::plane_abstr* basePlane() { return mBasePlane; }

	// Folder* origin() const { return mOrigin; }

	bool add_constraint(SkConstraint* constr, SkDrawable* immovable_hint = nullptr);
	bool toggle_constraint(SkConstraint* constr, bool enable);
	bool update_constraints(bool safeUpdate, bool update_on_solveFail);

	std::map<var_ptr, float> snapshot();
	std::vector<VarDualState> deltaSnapshot(std::map<var_ptr, float> first);
	static void apply_snapshot(std::map<var_ptr, float> shot);
	static void apply_snapshot(std::vector<VarState> shot);
	static void apply_deltaSnapshot(std::vector<VarDualState> deltaShot, bool first);
	void backup_system();
	void revert_system_to_backup();

	virtual void invoke_workspace(document* doc);
};

#endif
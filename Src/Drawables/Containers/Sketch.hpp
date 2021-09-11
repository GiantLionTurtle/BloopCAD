
#ifndef SKETCH_HPP_
#define SKETCH_HPP_

#include <Drawables/Base/Collection.hpp>
#include <Geometry/3d/Plane_abstr.hpp>
#include <Drawables/Containers//Folder.hpp>
#include <ConstraintsSolver/ConstraintsSystem.hpp>
#include <Drawables/Sk/SkDrawable.hpp>
#include <Drawables/Sk/SkGeometry.hpp>
#include <Drawables/Sk/SkConstraint.hpp>
#include <Actions/Common/Serial_action.hpp>

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
	size_t size() { return n_geom() + n_constr(); }
	SkIntDrawable* at(size_t ind)
	{
		if(ind < n_geom()) {
			return mGeometries.at(ind);
		} else {
			return mConstraints.at(ind - n_geom());
		}
	}

	bool has_newElems() { return mInitInd_geom < mGeometries.size() || mInitInd_constr < mConstraints.size(); }
	void init_newElems()
	{
		init_newElems_stat(mGeometries, mInitInd_geom, driven());
		init_newElems_stat(mConstraints, mInitInd_constr, driven());
	}

	size_t n_geom() { return mGeometries.size(); }
	SkGeometry* geom(size_t ind) { return mGeometries.at(ind); }
	void add_geom(SkGeometry* g) { mGeometries.push_back(g); mDriven->set_need_update(); }

	size_t n_constr() { return mConstraints.size(); }
	SkConstraint* constr(size_t ind) { return mConstraints.at(ind); }
	void add_constr(SkConstraint* c) { mConstraints.push_back(c); mDriven->set_need_update(); }
};

/*
	@class sketch describes a sketch which is a bunch of geometries with constraints
	@parent : entity
*/
class Sketch : public Collection_abstr<SkIndexer>, public DragSystemHandler {
public:
private:
	Geom3d::Plane_abstr* mBasePlane; // Plane onto which every geometry is added, maybe it should descend from Plane_abstract..

	// Folder* mOrigin;
	DragEnabled_ConstraintsSystem<2> mConstrSystem;
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
	SkExpSelPoint closest_draggable(glm::vec2 cursor, Camera* cam, int filter);
	void move_selected(glm::vec2 delta);
	std::shared_ptr<Serial_action> delete_selected(); 
	void unselect_all();
	size_t n_selected();

	void add_geometry(SkGeometry* geom);
	
	// void add_toolPreview(Drawable* ent);
	// void clear_toolPreview();

	// void add_selectedGeometry(sketchEntity* ent);
	// void remove_selectedGeometry(sketchEntity* ent);
	// void clear_selectedGeometries();
	// size_t n_selectedGeometries() { return mSelectedEntities.size(); }

	// BLOOPBLOOPWACK
	bool can_delete(SkDrawable* ent);

	// void toggle_selection_from_area(glm::vec2 a, glm::vec2 b, bool contained);

	/*
		@function basePlane

		@return : The base plane of the sketch
	*/
	Geom3d::Plane_abstr* basePlane() { return mBasePlane; }

	// Folder* origin() const { return mOrigin; }

	DraggedPoint<2>* dragConstr() { return mConstrSystem.dragConstr(); }
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

	virtual void invoke_workspace(Document* doc);

	void toggle_solve() { mSolve_allowed = !mSolve_allowed; }
	bool solve_allowed() { return mSolve_allowed; }
};

#endif
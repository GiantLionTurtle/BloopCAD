
#ifndef SKETCH_HPP_
#define SKETCH_HPP_

#include "Collection.hpp"
#include <geometry/geometry_3d/plane_abstr.hpp>
#include <Drawables/Folder.hpp>
#include <constraintsSolver/constraintSystem.hpp>
#include "sk/SkDrawable.hpp"
#include "sk/SkGeometry.hpp"
#include "sk/SkConstraint.hpp"

#include <memory>
#include <vector>

#define RECORD_SNAPSHOT_DELTAS(sk, call, delta_in, delta_out) { \
	delta_in.clear(); \
	delta_out.clear(); \
	std::vector<entityPosSnapshot_ptr> init, final; \
	init = sk->geometriesSnapshots(); \
	call; \
	final = sk->geometriesSnapshots(); \
	for(int i = 0; i < init.size() && i < final.size(); ++i) { \
		delta_in.push_back(init[i]); \
		delta_out.push_back(final[i]);\
	} \
}

#define RECORD_SNAPSHOT_DELTAS_IN(sk, call, delta_in) { \
	delta_in.clear(); \
	std::vector<entityPosSnapshot_ptr> init, final; \
	init = sk->geometriesSnapshots(); \
	call; \
	final = sk->geometriesSnapshots(); \
	for(int i = 0; i < init.size() && i < final.size(); ++i) { \
		delta_in.push_back(init[i]); \
	} \
}

#define RECORD_SNAPSHOT_DELTAS_OUT(sk, call, delta_out) { \
	delta_out.clear(); \
	std::vector<entityPosSnapshot_ptr> init, final; \
	init = sk->geometriesSnapshots(); \
	call; \
	final = sk->geometriesSnapshots(); \
	for(int i = 0; i < init.size() && i < final.size(); ++i) { \
		delta_out.push_back(final[i]);\
	} \
}

class SkIndexer {
private:
	std::vector<SkGeometry*> mGeometries;
	std::vector<SkConstraint*> mConstraints;
public:

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

	size_t num_geom() { return mGeometries.size(); }
	SkGeometry* geom(size_t ind) { return mGeometries.at(ind); }
	void add_geom(SkGeometry* g) { mGeometries.push_back(g); }

	size_t num_constr() { return mConstraints.size(); }
	SkConstraint* constr(size_t ind) { return mConstraints.at(ind); }
	void add_constr(SkConstraint* c) { mConstraints.push_back(c); }
};

/*
	@class sketch describes a sketch which is a bunch of geometries with constraints
	@parent : entity
*/
class Sketch : public Discrete_Collection<SkIndexer> {
private:
	geom_3d::plane_abstr_ptr mBasePlane; // Plane onto which every geometry is added, maybe it should descend from plane_abstract..


	// Folder* mOrigin;

	constraintSystem mSystem;
	std::vector<double> mSystemBackup;
public:
	/*
		@function sketch creates a sketch

		@param base_plane : The plane onto which everything is constructed in the sketch
	*/
	Sketch(geom_3d::plane_abstr_ptr base_plane);

	void init();

	void notify(Drawable* who, int msg, bool child);

	SelectionPoint closest(glm::vec2 cursor, Camera* cam, glm::vec3 cam_ray, int filter);
	DraggableSelectionPoint closest_draggable(glm::vec2 cursor, Camera* cam, glm::vec3 cam_ray, int filter);

	void add_geometry(SkGeometry* ent);
	// std::vector<entityPosSnapshot_ptr> geometriesSnapshots(int state_mask = 0); // TODO: Check that it is really 0 and not the max int that is needed

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
	geom_3d::plane_abstr_ptr basePlane() { return mBasePlane; }

	// Folder* origin() const { return mOrigin; }

	bool add_constraint(SkConstraint* constr, SkDrawable* immovable_hint = nullptr);
	bool toggle_constraint(SkConstraint* constr, bool enable);
	bool update_constraints(bool safeUpdate, bool update_on_solveFail);

	void backup_system();
	void revert_system_to_backup();

	virtual void invoke_workspace(document* doc);
};

#endif
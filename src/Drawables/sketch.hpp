
#ifndef SKETCH_HPP_
#define SKETCH_HPP_

#include "Drawable.hpp"
#include <geometry/geometry_3d/plane_abstr.hpp>
#include <Drawables/folder.hpp>
#include <constraintsSolver/constraintSystem.hpp>
#include <Drawables/tangibleEntities/sketchEntities/sketchGeometry.hpp>
#include <Drawables/tangibleEntities/sketchEntities/sketchConstraint.hpp>

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


class sketch;
using sketch_ptr = std::shared_ptr<sketch>;

/*
	@class sketch describes a sketch which is a bunch of geometries with constraints
	@parent : entity
*/
class sketch : public Drawable, public std::enable_shared_from_this<sketch> {
private:
	geom_3d::plane_abstr_ptr mBasePlane; // Plane onto which every geometry is added, maybe it should descend from plane_abstract..

	std::vector<sketchGeometry_ptr> mGeometries;
	std::vector<Drawable_ptr> mToolPreview;
	std::vector<sketchEntity_ptr> mSelectedEntities;
	std::vector<std::shared_ptr<constraint_entity>> mConstraints;

	folder_ptr mOrigin;

	constraintSystem mSystem;
	std::vector<double> mSystemBackup;
public:
	/*
		@function sketch creates a sketch

		@param base_plane : The plane onto which everything is constructed in the sketch
	*/
	sketch(geom_3d::plane_abstr_ptr base_plane);
	/*
		@function sketch creates a sketch

		@param base_plane : The plane onto which everything is constructed in the sketch
		@param parent : 	An entity that contains this sketch
	*/
	sketch(geom_3d::plane_abstr_ptr base_plane, Drawable* parent);

	void init();

	void notify(int msg);

	void print(int depth = 0);

	sketchEntity_ptr entity_at_point(Camera_ptr cam, glm::vec2 cursor);

	void add_geometry(sketchGeometry_ptr ent);
	std::vector<entityPosSnapshot_ptr> geometriesSnapshots();
	std::vector<entityPosSnapshot_ptr> selectedGeometriesSnapshots();
	void add_toolPreview(Drawable_ptr ent);
	void clear_toolPreview();
	void add_selectedGeometry(sketchEntity_ptr ent);
	void remove_selectedGeometry(sketchEntity_ptr ent);
	void clear_selectedGeometries();
	size_t n_selectedGeometries() { return mSelectedEntities.size(); }

	bool can_delete(sketchEntity_ptr ent);

	void for_each(std::function<void (Drawable_ptr)> func);
	void for_each_selected(std::function<void (sketchEntity_ptr)> func);
	void toggle_selection_from_area(glm::vec2 a, glm::vec2 b, bool contained);

	/*
		@function basePlane

		@return : The base plane of the sketch
	*/
	geom_3d::plane_abstr_ptr basePlane() { return mBasePlane; }

	folder_ptr origin() const { return mOrigin; }

	bool add_constraint(std::shared_ptr<constraint_entity> constr, sketchEntity_ptr immovable_hint = nullptr);
	bool toggle_constraint(std::shared_ptr<constraint_entity> constr, bool enable);
	bool update_constraints(bool safeUpdate, bool update_on_solveFail);

	void backup_system();
	void revert_system_to_backup();

	virtual void invoke_workspace(document* doc);
protected:
	/*
		@function draw_impl draws the part on screen, it calls draw on all it's components

		@param cam : The Camera used for rendering
		@param frame : 	The current frame id
	*/
	virtual void draw_impl(Camera_ptr cam, int frame);
};

#endif
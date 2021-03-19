
#ifndef SKETCH_HPP_
#define SKETCH_HPP_

#include "entity.hpp"
#include <geometry/geometry_3d/plane_abstr.hpp>
#include <entities/folder.hpp>
#include <constraintsSolver/constraintSystem.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchGeometry.hpp>

#include <memory>
#include <vector>

class sketch;
using sketch_ptr = std::shared_ptr<sketch>;

/*
	@class sketch describes a sketch which is a bunch of geometries with constraints
	@parent : entity
*/
class sketch : public entity, public std::enable_shared_from_this<sketch> {
private:
	geom_3d::plane_abstr_ptr mBasePlane; // Plane onto which every geometry is added, maybe it should descend from plane_abstract..

	std::vector<sketchGeometry_ptr> mGeometries;
	std::vector<entity_ptr> mToolPreview;
	std::vector<sketchEntity_ptr> mSelectedGeometries;

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
	sketch(geom_3d::plane_abstr_ptr base_plane, entity* parent);

	void init();

	void notify(int msg);

	void print(int depth = 0);

	sketchEntity_ptr entity_at_point(camera_ptr cam, glm::vec2 cursor);

	void add_geometry(sketchGeometry_ptr ent);
	void add_toolPreview(entity_ptr ent);
	void clear_toolPreview();
	void add_selectedGeometry(sketchEntity_ptr ent);
	void remove_selectedGeometry(sketchEntity_ptr ent);
	void clear_selectedGeometries();
	size_t n_selectedGeometries() { return mSelectedGeometries.size(); }

	bool can_delete(sketchEntity_ptr ent);

	void for_each(std::function<void (entity_ptr)> func);
	void for_each_selected(std::function<void (sketchEntity_ptr)> func);
	void toggle_selection_from_area(glm::vec2 a, glm::vec2 b, bool contained);

	/*
		@function basePlane

		@return : The base plane of the sketch
	*/
	geom_3d::plane_abstr_ptr basePlane() { return mBasePlane; }

	folder_ptr origin() const { return mOrigin; }

	bool add_constraint(std::shared_ptr<constraint_abstract> cons, sketchEntity_ptr immovable_hint = nullptr);
	bool update_constraints();

	void backup_system();
	void revert_system_to_backup();

	virtual void invoke_workspace(document* doc);
protected:
	/*
		@function draw_impl draws the part on screen, it calls draw on all it's components

		@param cam : The camera used for rendering
		@param frame : 	The current frame id
	*/
	virtual void draw_impl(camera_ptr cam, int frame);
};

#endif
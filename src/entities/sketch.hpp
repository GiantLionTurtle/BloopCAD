
#ifndef SKETCH_HPP_
#define SKETCH_HPP_

#include "entity.hpp"
#include <geometry/geometry_3d/plane_abstr.hpp>
#include <entities/folder.hpp>
#include <constraintsSolver/equationsSystem.hpp>
#include <constraintsSolver/constraint.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchEntity.hpp>

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

	std::vector<sketchEntity_ptr> mGeometries;
	folder_ptr mOrigin;

	equationsSystem mSystem;
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

	sketchEntity_ptr geometry_at_point(camera_ptr cam, glm::vec2 cursor);

	void add_geometry(sketchEntity_ptr ent);

	virtual void for_each(std::function<void (entity_ptr)> func);

	/*
		@function basePlane

		@return : The base plane of the sketch
	*/
	geom_3d::plane_abstr_ptr basePlane() { return mBasePlane; }

	folder_ptr origin() const { return mOrigin; }

	bool add_constraint(std::shared_ptr<constraint> cons);
	bool add_constraint(subEquationsSystem const& subSystem);

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
	
	void create_origin();
};

#endif
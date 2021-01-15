
#ifndef SKETCH_HPP_
#define SKETCH_HPP_

#include "entity.hpp"
#include <entities/geometry/plane_abstract.hpp>
#include <entities/folder.hpp>
#include <constraintsSolver/equationsSystem.hpp>
#include <constraintsSolver/constraint.hpp>

#include <memory>
#include <vector>

class sketch;
using sketch_ptr = std::shared_ptr<sketch>;

/*
	@class sketch describes a sketch which is a bunch of geometries with constraints
	@parent : entity
*/
class sketch : public entity {
private:
	plane_abstract_ptr mBasePlane; // Plane onto which every geometry is added, maybe it should descend from plane_abstract..

	folder_ptr mOrigin;

	equationsSystem mSystem;
public:
	/*
		@function sketch creates a sketch

		@param base_plane : The plane onto which everything is constructed in the sketch
	*/
	sketch(plane_abstract_ptr base_plane);
	/*
		@function sketch creates a sketch

		@param base_plane : The plane onto which everything is constructed in the sketch
		@param parent : 	An entity that contains this sketch
	*/
	sketch(plane_abstract_ptr base_plane, entity* parent);

	/*
		@function basePlane

		@return : The base plane of the sketch
	*/
	plane_abstract_ptr basePlane() { return mBasePlane; }

	entity_ptr origin() const { return mOrigin; }

	bool add_constraint(std::shared_ptr<constraint> cons);

	bool update_constraints();
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

#ifndef PART_HPP_
#define PART_HPP_

#include "entity.hpp"
#include "folder.hpp"

#include "tangibleEntities/plane.hpp"
#include "sketch.hpp"

#include <gtkmm.h>

#include <memory>

class document; // The forward declaration of document is needed for pointer declaration
class modelColumns;

/*
	@class part describes a part, which is composed of sub entities and sketches
	@parent : entity
*/
class part : public entity {
private:
	std::shared_ptr<plane> mXY, mYZ, mZX; // Origin planes
	folder_ptr mOrigin;
	std::vector<sketch_ptr> mSketches; // Sketches drawn in the part
public:
	/*
		@function part creates an empty part
	*/
	part();

	/*
		@function part creates an empty part that is owned by another entity

		@param parent : The parent entity to follow
	*/
	part(entity* parent);

	/*
		@function init_scene sets up the scene by :
		*creating the origin planes
	*/
	virtual void init_scene();

	virtual void set_handle(entityHandle* handle_);

	virtual void add(entity_ptr elem);

	/*
		@function get_sketch grants linear access to the part's sketches

		@param ind [defaults to -1] : The index of the desired sketch

		@return : The desired sketch or the last if the index is out of bounds
	*/
	sketch_ptr get_sketch(int ind = -1);

	bool has_volume() const { return false; }

	entity_ptr origin() const { return mOrigin; }
protected:
	/*
		@function draw_impl draws the part on screen, it calls draw on all it's components

		@param cam : The camera used for rendering
		@param frame : 	The current frame id
	*/
	virtual void draw_impl(camera_ptr cam, int frame);
};

#endif

#ifndef PART_HPP_
#define PART_HPP_

#include "entity.hpp"

#include "plane_abstract.hpp"
#include "plane.hpp"
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
	std::vector<std::shared_ptr<sketch>> mSketches; // Sketches drawn in the part

	modelColumns* mColumns;
	Glib::RefPtr<Gtk::TreeStore> mRefTreeModel;
	Gtk::TreeModel::Row mRow;
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

	void set_tree(modelColumns* columns, Gtk::TreeModel::Row row, Glib::RefPtr<Gtk::TreeStore> treeModel);

	void add_child_to_tree(std::shared_ptr<entity> child);

	virtual void add(std::shared_ptr<entity> elem);

	/*
		@function add_sketch adds a sketch to the list

		@param sketch_ : A shared pointer to the sketch to add
	*/
	void add_sketch(std::shared_ptr<sketch> sketch_);
	/*
		@function get_sketch grants linear access to the part's sketches

		@param ind [defaults to -1] : The index of the desired sketch

		@return : The desired sketch or the last if the index is out of bounds
	*/
	std::shared_ptr<sketch> get_sketch(int ind = -1);
protected:
	/*
		@function draw_impl draws the part on screen, it calls draw on all it's components

		@param cam : The camera used for rendering
		@param frame : 	The current frame id
	*/
	virtual void draw_impl(std::shared_ptr<camera> cam, int frame);
	/*
		@function draw_selection_impl draws the part on the selection buffer, it calls draw_selection on all it's components

		@param cam : The camera used for rendering
		@param frame : 	The current frame id
	*/
	virtual void draw_selection_impl(std::shared_ptr<camera> cam, int frame);
};

#endif
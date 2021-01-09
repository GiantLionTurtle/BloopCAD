
#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <graphics_utils/vertexBuffer.hpp>
#include <graphics_utils/vertexBufferLayout.hpp>
#include <graphics_utils/vertexArray.hpp>
#include <graphics_utils/indexBuffer.hpp>
#include <graphics_utils/shader.hpp>
#include <graphics_utils/camera.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <tuple>

class entity;
class entityHandle;

using entity_ptr = std::shared_ptr<entity>;

/*
	@define BLOOP_ENTITY_ describe state flags for an entity
*/
#define BLOOP_ENTITY_SELECTED_FLAG 	1
#define BLOOP_ENTITY_HOVERED_FLAG	2
#define BLOOP_ENTITY_HIDDEN_FLAG 	4
#define BLOOP_ENTITY_EXISTS_FLAG 	8

/*
	@class entity describes a basic entity that appears on screen
*/
class entity {
protected:
	int mState;	// The state of the entity, described by the above flags
	glm::ivec3 mSelectionID;
	glm::vec3 mSelectionColor; // The color of the entity on the selection buffer

	bool mRequire_redraw;

	std::string mName;

	std::vector<std::tuple<glm::ivec3, entity_ptr, bool>> mChildren; // The entities container
	glm::ivec3 mHighestInd; // The current highest 3 ints index
	glm::ivec3* highestInd;
	entity* mParent;

	entityHandle* mHandle;
public:
	/*
		@function entity creates an empty entity with reasonable state defaults:
		* Not selected
		* Not hovered
		* Not hidden
		* Does exist
	*/
	entity();
	entity(glm::ivec3 startInd);
	entity(entity* parent);

	/*
		@function draw draws the entity

		@param cam : 	The camera used for rendering
		@param frame : 	The current frame id
	*/
	void draw(camera_ptr cam, int frame);
	/*
		@function draw draws the entity with it's plain selection color

		@param cam : 	The camera used for rendering
		@param frame : 	The current frame id
	*/
	void draw_selection(camera_ptr cam, int frame);

	/*
		@function set_selected sets the selected

		@param select : The value of the flag
	*/	
	void set_selected(bool select);
	/*
		@function select is a shorthand to set the selected flag to true
	*/
	void select();
	/*
		@function unselect is a shorthand to set the selected flag to false
	*/
	void unselect();
	/*
		@function selected

		@return : Wheter or not the selected flag is up
	*/
	bool selected() const;

	/*
		@function set_hover sets the hovered flag

		@param hover : The value of the flag
	*/
	void set_hover(bool hover);
	/*
		@function hovered

		@return : Wheter or not the hovered flag is up
	*/
	bool hovered() const;

	/*
		@function set_hidden sets the hidden flag

		@param hidden : THe value of the flag
	*/
	void set_hidden(bool hidden);
	/*
		@function hide is a shorthand to set the hidden flag to true
	*/
	void hide();
	/*
		@function show is a shorthand to set the hidden flag to false
	*/
	void show();
	/*
		@function hidden

		@return : Whether or not the hidden flag is up
	*/
	bool hidden() const;
	/*
		@function visible

		@return : Wheter or not the hidden flag is down
	*/
	bool visible() const;

	/*
		@function set_exists

		@param exists_ : The value of the flag
	*/
	void set_exists(bool exists_);
	/*
		@function exists

		@return : Wether or not the exist flag is up
	*/
	bool exists() const;
	/*
		@operator bool

		@return : Wheter or not the exists flag is up
	*/
	operator bool() const { return exists(); }

	/*
		@function state

		@return : The entity's state
	*/
	int state() const { return mState; }

	glm::ivec3 selectionID() const { return mSelectionID; }
	void set_selectionID(glm::ivec3 id) { mSelectionID = id; mSelectionColor = mSelectionID; mSelectionColor /= 255.0f; }

	std::string name() const { return mName; }
	void set_name(std::string const& name_) { mName = name_; }

	entityHandle* handle() const { return mHandle; }
	virtual void set_handle(entityHandle* handle_) { mHandle = handle_; }

	/*
		@function add adds an element to the indexer and increments the index

		@param elem : The element to add
	*/
	virtual void add(entity_ptr elem);

	/*
		@function is the size getter

		@return : the number of entities indexed
	*/
	size_t size() const { return mChildren.size(); }

	/*
		@function get gives access to the indexer in a linear fashion

		@param ind : The linear index of the researched entity

		@return : The entity at a peculiar linear index, it it exists
	*/
	entity_ptr get(size_t ind) const;
	/*
		@function get gives access to the indexer at a 3 ints index

		@param ind : The 3 ints index of the researched entity

		@return : The entity at a peculiar linear index, it it exists
	*/
	entity_ptr get(glm::ivec3 const& ind) const;

	/*
		@function get_last returns the last element added

		@return : The last entity of the list
	*/
	entity_ptr get_last() const;

	/*
		@operator [] is a wrapper for the get function
	*/
	entity_ptr operator[](size_t ind) const;
	entity_ptr operator[](glm::ivec3 const& ind) const;

	/*
		@function for_each applies a function to all entities stored

		@param func : The function to apply. It takes a entity_ptr and optionaly a glm::ivec3 index
	*/
	void for_each(std::function<void (entity_ptr, glm::ivec3)> func);
	void for_each(std::function<void (entity_ptr)> func);

	/*
		@function is_following

		@return : Wheter or not the indexer is following another indexer
	*/
	bool is_following() const { if(mParent) return true; return false; }

	void set_require_redraw();
	bool require_redraw() const { return mRequire_redraw; }

	virtual void notify_childUpdate() {}

	void set_parent(entity* parent);
protected:
	/*
		@function draw_impl is an overiddable function for children classes to draw themselves

		@param cam : 	The camera used for rendering
		@param frame : 	The current frame id
	*/
	virtual void draw_impl(camera_ptr cam, int frame) {};
	/*
		@function draw_selection_impl is an overiddable function for children classes to draw themselves 
		in plain color on the selection buffer

		@param cam : 	The camera used for rendering
		@param frame : 	The current frame id
	*/	
	virtual void draw_selection_impl(camera_ptr cam, int frame) {};

	/*
		@function compare_indices makes a comparison between two 3 ints indices

		@param a : The left side operand of the comparison
		@param b : The right side operand of the comparison

		@return : -1 if a > b, 0 if a == b, 1 if b > a
	*/
	static int compare_indices(glm::ivec3 const& a, glm::ivec3 const& b);
	/*
		@function increment_index adds one to an index

		@param ind [out] : A reference to the index to increment
	*/
	static void increment_index(glm::ivec3 &ind);

	void add_child(glm::ivec3 id, entity_ptr child, bool first = false);
};

#endif
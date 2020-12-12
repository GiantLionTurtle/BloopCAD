
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
#include <memory>

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
	glm::vec3 mSelectionColor; // The color of the entity on the selection buffer
public:
	/*
		@function entity creates an empty entity with reasonable state defaults:
		* Not selected
		* Not hovered
		* Not hidden
		* Does exist
	*/
	entity();

	/*
		@function draw draws the entity

		@param cam : The camera used for rendering
	*/
	void draw(std::shared_ptr<camera> cam);
	/*
		@function draw draws the entity with it's plain selection color

		@param cam : The camera used for rendering
	*/
	void draw_selection(std::shared_ptr<camera> cam);

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

	/*
		@function set_selectionColor sets the color used to render on the selection buffer
	*/
	void set_selectionColor(glm::vec3 color) { mSelectionColor = color; }
protected:
	/*
		@function draw_impl is an overiddable function for children classes to draw themselves

		@param cam : The camera used for rendering
	*/
	virtual void draw_impl(std::shared_ptr<camera> cam) {};
	/*
		@function draw_selection_impl is an overiddable function for children classes to draw themselves 
		in plain color on the selection buffer

		@param cam : The camera used for rendering
	*/	
	virtual void draw_selection_impl(std::shared_ptr<camera> cam) {};
};

#endif
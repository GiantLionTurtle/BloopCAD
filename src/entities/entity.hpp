
#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <graphics_utils/vertexBuffer.hpp>
#include <graphics_utils/vertexBufferLayout.hpp>
#include <graphics_utils/vertexArray.hpp>
#include <graphics_utils/indexBuffer.hpp>
#include <graphics_utils/shader.hpp>
#include <graphics_utils/camera.hpp>
#include <constraintsSolver/equationsSystem.hpp>
#include <constraintsSolver/expressionVector3.hpp>

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
public: 
	enum draw_type { ALL, TRANSLUCID, ACTIVE, INACTIVE };
protected:
	int mState;	// The state of the entity, described by the above flags

	bool mRequire_redraw, mRequire_selfRedraw;

	float mHoverDistance;

	std::string mName;

	std::vector<entity_ptr> mChildren; // The entities container
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
	entity(entity* parent);

	/*
		@function draw draws the entity

		@param cam : 	The camera used for rendering
		@param frame : 	The current frame id
	*/
	void draw(camera_ptr cam, int frame, draw_type type = draw_type::ALL, bool on_required = false);

	void update();

	entity_ptr hovered_child(camera_ptr cam, glm::vec2 cursor_pos, std::function<bool (entity_ptr)> filter_func = 
	std::function<bool (entity_ptr)>([](entity_ptr ent) { return true; }));

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

	bool active() const;

	/*
		@function state

		@return : The entity's state
	*/
	int state() const { return mState; }

	virtual bool translucid() { return false; }

	virtual int selection_rank() { return -1; }

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
		@function get_last returns the last element added

		@return : The last entity of the list
	*/
	entity_ptr get_last() const;

	/*
		@operator [] is a wrapper for the get function
	*/
	entity_ptr operator[](size_t ind) const;

	/*
		@function for_each applies a function to all entities stored

		@param func : The function to apply. It takes a entity_ptr
	*/
	void for_each(std::function<void (entity_ptr)> func);

	/*
		@function is_following

		@return : Wheter or not the indexer is following another indexer
	*/
	bool is_following() const { if(mParent) return true; return false; }

	void set_require_redraw(bool self = true);
	bool require_redraw() const { return mRequire_redraw; }

	virtual void notify_childUpdate() {}

	virtual void move(glm::vec3 from, glm::vec3 to) {}
	virtual void set_constant() {}
	virtual void set_tmpConstant(bool const_) {}

	virtual std::vector<variable_ptr> variables() { return {}; };

	virtual subEquationsSystem coincidence() { return {}; }
	virtual subEquationsSystem_vec direction() { return {}; }
	virtual std::vector<subEquationsSystem> verticality() { return {}; }
	virtual std::vector<subEquationsSystem> horizontality() { return {}; };
protected:
	/*
		@function draw_impl is an overiddable function for children classes to draw themselves

		@param cam : 	The camera used for rendering
		@param frame : 	The current frame id
	*/
	virtual void draw_impl(camera_ptr cam, int frame) {};

	virtual void update_impl() {};

	bool should_draw_self(draw_type type, bool on_required);

	virtual float selection_depth(camera_ptr cam, glm::vec2 cursor_pos) { return -1.0f; }

	void hovered_child_internal(camera_ptr cam, glm::vec2 cursor_pos, entity_ptr& candidate, float& min_dist, std::function<bool (entity_ptr)> filter_func);
};

#endif
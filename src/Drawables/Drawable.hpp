
#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <Graphics_utils/VertexBuffer.hpp>
#include <Graphics_utils/VertexBufferLayout.hpp>
#include <Graphics_utils/VertexArray.hpp>
#include <Graphics_utils/IndexBuffer.hpp>
#include <Graphics_utils/Shader.hpp>
#include <Graphics_utils/Camera.hpp>
#include <baseObject.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <tuple>

class bloop;
class document;

class Drawable;
class entityHandle;

/*
	@define BLOOP_ENTITY_ describe state flags for an entity
*/
#define BLOOP_ENTITY_SELECTED_FLAG 	1
#define BLOOP_ENTITY_HOVERED_FLAG	2
#define BLOOP_ENTITY_HIDDEN_FLAG 	4
#define BLOOP_ENTITY_EXISTS_FLAG 	8

#define BLOOP_ENTITY_SELECTED_NOTIF 1
#define BLOOP_ENTITY_HOVERED_NOTIF	2
#define BLOOP_ENTITY_HIDDEN_NOTIF	4
#define BLOOP_ENTITY_EXISTS_NOTIF	8
#define BLOOP_ENTITY_UPDATED_NOTIF	16

enum Drawable_types {
	DRAWABLE = 1,
	SKDRAWABLE = 2,
	POINT = 4,
	AXIS = 8,
	PLANE = 16,
	ANNOTATION = 32,
	CURVE = 64,
};

struct SelPoint {
	SelPoint():
		ent(nullptr),
		dist_to_cam(0.0f)
	{}
	SelPoint(Drawable* d, float dist):
		ent(d),
		dist_to_cam(dist)
	{}

	operator bool() { return ent; }

	Drawable* ent;
	float dist_to_cam;
};

/*
	@class entity describes a basic entity that appears on screen
*/
class Drawable : virtual public baseObject {
public: 
	enum draw_type { ALL, TRANSLUCID, ACTIVE, INACTIVE };
	enum notifications { SELECTED, UNSELECTED, HOVERED, UNHOVERED, HIDEN, UNHIDEN, DELETED, RESURRECTED, UPDATED};
protected:
	int mState;	// The state of the entity, described by the above flags
	int mStdNotifs; // Standard notifications sent by a child to it's parent
	int mType;
	bool mInited;
	bool mNeed_redraw, mNeed_update;

	std::string mName;

	Drawable* mParent;

	entityHandle* mHandle;
public:
	/*
		@function entity creates an empty entity with reasonable state defaults:
		* Not selected
		* Not hovered
		* Not hidden
		* Does exist
	*/
	Drawable();
	virtual ~Drawable();

	void init();
	virtual void draw(Camera_ptr cam, int frame, draw_type type = draw_type::ALL);
	void update(bool force = false);

	void notify_parent(int msg); // BLOOPBLOOPCHECKIFUSED
	virtual void notify(Drawable* who, int msg, bool child) {}

	/*
		@function set_selected sets the selected

		@param select : The value of the flag
	*/	
	void set_selected(bool select_, bool silent = false);
	/*
		@function select is a shorthand to set the selected flag to true
	*/
	void select(bool silent = false);
	/*
		@function unselect is a shorthand to set the selected flag to false
	*/
	void unselect(bool silent = false);
	/*
		@function selected

		@return : Wheter or not the selected flag is up
	*/
	bool selected() const;

	/*
		@function set_hover sets the hovered flag

		@param hover : The value of the flag
	*/
	void set_hover(bool hover, bool silent = false);
	/*
		@function hovered

		@return : Wheter or not the hovered flag is up
	*/
	bool hovered() const;

	/*
		@function set_hidden sets the hidden flag

		@param hidden : THe value of the flag
	*/
	void set_hidden(bool hide, bool silent = false);
	/*
		@function hide is a shorthand to set the hidden flag to true
	*/
	void hide(bool silent = false);
	/*
		@function show is a shorthand to set the hidden flag to false
	*/
	void show(bool silent = false);
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
	void set_exists(bool exists_, bool silent);
	void set_exists(bool exists_) { set_exists(exists_, false); }
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

	virtual bool active() const;

	/*
		@function state

		@return : The entity's state
	*/
	int state() const { return mState; }
	int type() const { return mType; }
	void print_state();

	bool notif_on_selected();
	void set_notif_on_selected(bool do_);

	bool notif_on_hover();
	void set_notif_on_hover(bool do_);
	
	bool notif_on_hidden();
	void set_notif_on_hidden(bool do_);
	
	bool notif_on_exists();
	void set_notif_on_exists(bool do_);

	bool notif_on_update();
	void set_notif_on_update(bool do_);

	bool notifications() const { return mStdNotifs; }

	virtual bool translucid() { return false; }

	virtual int selection_rank() { return -1; }
	virtual int selection_dist() { return 0; } // pixel distance for selection
	virtual SelPoint closest(glm::vec2 cursor, Camera* cam, int filter) { return SelPoint(); }
	virtual void select_within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained) {}	

	std::string name() const { return mName; }
	void set_name(std::string const& name_) { mName = name_; }

	entityHandle* handle() const { return mHandle; }
	virtual void set_handle(entityHandle* handle_) { mHandle = handle_; }

	/*
		@function is_following

		@return : Wheter or not the indexer is following another indexer
	*/
	bool is_following() const { if(mParent) return true; return false; }
	Drawable* parent() const { return mParent; }
	void set_parent(Drawable* parent_) { mParent = parent_; }

	bool need_init() const { return !mInited; }
	void set_need_redraw();
	bool need_redraw() const { return mNeed_redraw; }
	void set_need_update();
	bool need_update() const { return mNeed_update; }
	
	virtual void invoke_workspace(document* doc) {}
protected:

	virtual void init_impl() {}
	virtual void draw_impl(Camera_ptr cam, int frame, draw_type type) {}
	virtual void update_impl() {}

	virtual void select_impl(bool sel) {}
	virtual void hover_impl(bool hov) {}
	virtual void hidden_impl(bool hid) {}
	virtual void exists_impl(bool ex) {}

	// bool should_draw_self(draw_type type, bool on_required);
};


#endif
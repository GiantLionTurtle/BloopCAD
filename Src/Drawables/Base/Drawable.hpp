
#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <Graphics_utils/VertexBuffer.hpp>
#include <Graphics_utils/VertexBufferLayout.hpp>
#include <Graphics_utils/VertexArray.hpp>
#include <Graphics_utils/IndexBuffer.hpp>
#include <Graphics_utils/Shader.hpp>
#include <Graphics_utils/Camera.hpp>
#include <BaseObject.hpp>
#include <Forward_bloop.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <tuple>

class Document;

class Drawable;

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
	SelPoint()
		: ent(nullptr)
		, dist_to_cam(0.0f)
	{}
	SelPoint(Drawable* d, float dist)
		: ent(d)
		, dist_to_cam(dist)
	{}

	operator bool() { return ent; }

	Drawable* ent;
	float dist_to_cam;
};

class UILink {
public:
	virtual void notify_hover(bool hov) {}
	virtual void notify_select(bool sel) {}
};

/*
	@class entity describes a basic entity that appears on screen
*/
class Drawable : virtual public BaseObject {
public: 
	enum draw_type { ALL, TRANSLUCID, ACTIVE, INACTIVE };
	enum notifications { SELECTED, UNSELECTED, HOVERED, UNHOVERED, HIDEN, UNHIDEN, DELETED, RESURRECTED, UPDATED};
protected:
	static int nDrawable;
	int mID;
	int mState;	// The state of the entity, described by the above flags
	int mType;
	bool mInited;
	bool mNeed_redraw;
	int mNeed_graphicUpdate; // 0 => no update ; 1 => need update ; 2 => need update and force

	std::string mName;

	Drawable* mParent;

	UILink* mUILink;
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

	virtual void init();
	virtual void draw(Camera* cam, int frame, draw_type type = draw_type::ALL);
	/*
		@function internalUpdate is used to update internal states
		eg. set a variable to "dragged"

		@note This update does not propagate
		@note This update must be quick and non-blocking
	*/
	virtual void internalUpdate();
	/*
		@function graphicUpdate is used to update opengl states
		eg. update a vertex buffer,
		
		@param force Wheter to force an update even if the mNeed_graphicUpdate is false

		@note This update propagates from top to bottom, all children are updated if
		the parent updates
	*/
	virtual void graphicUpdate(bool force = false);
	/*
		@function update does an internal update then schedules a graphic update
	*/
	void update(bool forceGraphic = false);

	void notify_parent(int msg); // BLOOPBLOOPCHECKIFUSED
	virtual void notify(Drawable* who, int msg, bool child) {}

	/*
		@function set_selected sets the selected

		@param select : The value of the flag
	*/	
	void set_selected(bool select_, bool parent_silent = false, bool ui_silent = false);
	/*
		@function select is a shorthand to set the selected flag to true
	*/
	void select(bool parent_silent = false, bool ui_silent = false);
	/*
		@function unselect is a shorthand to set the selected flag to false
	*/
	void unselect(bool parent_silent = false, bool ui_silent = false);
	/*
		@function selected

		@return : Wheter or not the selected flag is up
	*/
	virtual bool selected() const;

	/*
		@function set_hover sets the hovered flag

		@param hover : The value of the flag
	*/
	void set_hover(bool hover, bool parent_silent = false, bool ui_silent = false);
	/*
		@function hovered

		@return : Wheter or not the hovered flag is up
	*/
	bool hovered() const;

	/*
		@function set_hidden sets the hidden flag

		@param hidden : THe value of the flag
	*/
	void set_hidden(bool hide, bool parent_silent = false);
	/*
		@function hide is a shorthand to set the hidden flag to true
	*/
	void hide(bool parent_silent = false);
	/*
		@function show is a shorthand to set the hidden flag to false
	*/
	void show(bool parent_silent = false);
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
	void set_exists(bool exists_, bool parent_silent);
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

	virtual bool translucid() { return false; }

	virtual int selection_rank() { return -1; }
	virtual int selection_dist() { return 0; } // pixel distance for selection
	virtual SelPoint closest(glm::vec2 cursor, Camera* cam, int filter) { return SelPoint(); }
	virtual void select_within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained) {}	

	std::string name() const { return mName; }
	void set_name(std::string const& name_) { mName = name_; }

	UILink* handle() const { return mUILink; }
	virtual void set_handle(UILink* handle_) { mUILink = handle_; }

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
	void set_need_graphicUpdate(bool force = false);
	int need_graphicUpdate() const { return mNeed_graphicUpdate; }
	
	virtual void invoke_workspace(Document* doc) {}
protected:

	virtual void init_impl() {}
	virtual void draw_impl(Camera* cam, int frame, draw_type type) {}
	virtual void internalUpdate_impl() {}
	virtual void graphicUpdate_impl() {}

	virtual void select_impl(bool sel) {}
	virtual void hover_impl(bool hov) {}
	virtual void hidden_impl(bool hid) {}
	virtual void exists_impl(bool ex) {}
};


#endif
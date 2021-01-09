
#ifndef ENTITYVIEW_HPP_
#define ENTITYVIEW_HPP_

#include "entity.hpp"
#include <workspaces/workspace.hpp>

#include <gtkmm.h>

class document;
class entityView;

class entityHandle : public Gtk::EventBox {
private:
	Gtk::Box mBody;
	Gtk::Label mCollapser, mContent;
	Gtk::EventBox mCollapserEvents;

	entity_ptr mEntity;
	entityHandle* mParent;
	std::vector<entityHandle*> mChildren;
	entityView* mView;

	bool mCollapsed;
	int mLevel;
public:
	entityHandle(entity_ptr ent, entityView* view, entityHandle* parent);
	entityHandle(); // for root handles

	bool set_hover(GdkEventCrossing* event);
	bool unset_hover(GdkEventCrossing* event);

	bool manage_collapse(GdkEventButton* event);
	void collapse();
	void expend();
	void hide_rec();
	void show_rec();

	bool select(GdkEventButton* event);

	int count_upTo(entityHandle* child);
	int count_all();

	entityHandle* parent() const { return mParent; }
	void set_parent(entityHandle* parent_) { mParent = parent_; }

	entityView* view() { return mView; };
	void set_view(entityView* view_) { mView = view_;}

	void set_selected(bool selected);
	void set_hovered(bool hovered);

	void add_child(entityHandle* child);
};

class entityView : public Gtk::Box {
private:
	std::shared_ptr<workspaceState> mCurrentWorkspaceState;

	entityHandle mRootHandle;
	std::vector<entityHandle*> mHandles;
	document* mDoc;
public:
	entityView(document* doc);
	~entityView();

	void add_handle(entityHandle* handle, int at);

	entityHandle& root() { return mRootHandle; };
	document* doc() { return mDoc; }

	std::shared_ptr<workspaceState> currentWorkspaceState() const { return mCurrentWorkspaceState; }
	void set_workspaceState(std::shared_ptr<workspaceState> workspaceState_) { mCurrentWorkspaceState = workspaceState_; };
};

#endif
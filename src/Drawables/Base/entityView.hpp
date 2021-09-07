
#ifndef ENTITYVIEW_HPP_
#define ENTITYVIEW_HPP_

#include "Drawable.hpp"
#include <Workspaces/Workspace_abstr.hpp>

#include <gtkmm.h>

class document;
class entityView;

class entityHandle : public Gtk::EventBox {
private:
	Gtk::Box mBody;
	Gtk::Label mCollapser, mContent;
	Gtk::EventBox mCollapserEvents;

	Drawable* mEntity;
	entityHandle* mParent;
	std::vector<entityHandle*> mChildren;
	entityView* mView;

	bool mCollapsed;
	int mLevel;
public:
	entityHandle(Drawable* ent, entityView* view, entityHandle* parent);
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
	void set_exists(bool exists);

	void add_child(entityHandle* child);

	void update_name(std::string name);
};

class entityView : public Gtk::Box {
private:
	WorkspaceState* mCurrentWorkspaceState;

	entityHandle mRootHandle;
	std::vector<entityHandle*> mHandles;
	document* mDoc;
public:
	entityView(document* doc);
	~entityView();

	void add_handle(entityHandle* handle, int at);

	entityHandle& root() { return mRootHandle; };
	document* doc() { return mDoc; }

	WorkspaceState* state() const { return mCurrentWorkspaceState; }
	void set_WorkspaceState(WorkspaceState* WorkspaceState_) { mCurrentWorkspaceState = WorkspaceState_; };
};

#endif
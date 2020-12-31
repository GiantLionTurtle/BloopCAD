
#include "entityView.hpp"

#include <document.hpp>

entityHandle::entityHandle(std::shared_ptr<entity> ent, entityView* view, entityHandle* parent):
	mEntity(ent),
	mView(view),
	mParent(parent),
	mCollapsed(false)
{
	if(mView) {
		if(!mParent)
			mParent = &mView->root();
		mParent->add_child(this);
		mView->add_handle(this, mParent->count_upTo(this));
	}
	mLevel = mParent->mLevel + 1;

	mBody.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
	mCollapser.set_margin_left(15 + 15 * mLevel);
	mCollapser.set_label("");
	mCollapser.set_padding(5, 0);
	mCollapser.set_max_width_chars(15);

	mContent.set_padding(5, 0);
	if(mEntity) {
		mContent.set_label(mEntity->name());
	} else {
		mContent.set_label("Error, null entity.");
	}
	mContent.set_margin_left(5);

	mCollapserEvents.add(mCollapser);
	
	mBody.pack_start(mCollapserEvents, false, true);
	mBody.pack_start(mContent, false, true);

	add_events(	  Gdk::POINTER_MOTION_MASK
				| Gdk::BUTTON_PRESS_MASK
				| Gdk::BUTTON_RELEASE_MASK
				| Gdk::KEY_PRESS_MASK
				| Gdk::KEY_RELEASE_MASK
				| Gdk::STRUCTURE_MASK
				| Gdk::SCROLL_MASK
				| Gdk::ENTER_NOTIFY_MASK
				| Gdk::LEAVE_NOTIFY_MASK);
	
	
	add(mBody);

	mCollapserEvents.signal_button_press_event().connect(sigc::mem_fun(*this, &entityHandle::manage_collapse));
	
	signal_enter_notify_event().connect(sigc::mem_fun(*this, &entityHandle::set_hover));
	signal_leave_notify_event().connect(sigc::mem_fun(*this, &entityHandle::unset_hover));
	signal_button_press_event().connect(sigc::mem_fun(*this, &entityHandle::select));

	show_all();
}
entityHandle::entityHandle():
	mEntity(nullptr),
	mView(nullptr),
	mParent(nullptr),
	mLevel(-1)
{

}

bool entityHandle::manage_collapse(GdkEventButton* event)
{
	if(mChildren.empty())
		return true;
	
	if(!mCollapsed) {
		collapse();
	} else {
		expend();
	}

	return true;
}
void entityHandle::collapse()
{
	mCollapsed = true;
	mCollapser.set_label("+");
	for(int i = 0; i < mChildren.size(); ++i) {
		mChildren[i]->hide_rec();
	}
}
void entityHandle::expend()
{
	mCollapsed = false;
	mCollapser.set_label("-");
	for(int i = 0; i < mChildren.size(); ++i) {
		mChildren[i]->show_rec();
	}
}

void entityHandle::hide_rec()
{
	hide();
	for(int i = 0; i < mChildren.size(); ++i) {
		mChildren[i]->hide_rec();
	}
}
void entityHandle::show_rec()
{
	show();
	if(!mCollapsed) {
		for(int i = 0; i < mChildren.size(); ++i) {
			mChildren[i]->show_rec();
		}
	}
}

bool entityHandle::select(GdkEventButton* event)
{
	if(mView && mView->doc() && mEntity && mEntity->hovered()) {
		if(event->type != GDK_2BUTTON_PRESS || (event->type == GDK_2BUTTON_PRESS && !mEntity->selected()))
			mView->doc()->toggle_select(mEntity, event->state & GDK_CONTROL_MASK || event->state & GDK_SHIFT_MASK);
		if(event->type == GDK_2BUTTON_PRESS && mView->currentWorkspaceState()) {
			mView->currentWorkspaceState()->currentTool->act_on_entity(mEntity);
		} 
	}
	return true;
}

bool entityHandle::set_hover(GdkEventCrossing* event)
{
	if(mEntity && mView) {
		if(mView->currentWorkspaceState() && mView->currentWorkspaceState()->currentTool && !mView->currentWorkspaceState()->currentTool->should_hover(mEntity))
			return true;
		mEntity->set_hover(true);
	}
	return true;
}
bool entityHandle::unset_hover(GdkEventCrossing* event)
{
	if(mEntity)
		mEntity->set_hover(false);
	return true;
}

int entityHandle::count_upTo(entityHandle* child)
{
	int runningSum = (mParent ? mParent->count_upTo(this)+1 : 0);
	for(int i = 0; i < mChildren.size(); ++i) {
		if(mChildren[i] == child)
			break;
		runningSum += mChildren[i]->count_all();		
	}	
	return runningSum;
}
int entityHandle::count_all()
{
	int runningSum = 1;
	for(int i = 0; i < mChildren.size(); ++i) {
		runningSum += mChildren[i]->count_all();
	}
	return runningSum;
}

void entityHandle::set_selected(bool selected)
{
	if(mEntity->selected()) {
		get_style_context()->add_class("selected");
	} else {
		get_style_context()->remove_class("selected");
	}
}
void entityHandle::set_hovered(bool hovered)
{
	if(mEntity->hovered()) {
		get_style_context()->add_class("hovered");
	} else {
		get_style_context()->remove_class("hovered");
	}
}

void entityHandle::add_child(entityHandle* child)
{
	if(mChildren.empty()) {
		mCollapser.set_label("-");
	}

	mChildren.push_back(child);
}

entityView::entityView(document* doc):
	mCurrentWorkspaceState(nullptr),
	mRootHandle(),
	mDoc(doc)
{
	add_events(	  Gdk::POINTER_MOTION_MASK
				| Gdk::BUTTON_PRESS_MASK
				| Gdk::BUTTON_RELEASE_MASK
				| Gdk::KEY_PRESS_MASK
				| Gdk::KEY_RELEASE_MASK
				| Gdk::STRUCTURE_MASK
				| Gdk::SCROLL_MASK
				| Gdk::ENTER_NOTIFY_MASK
				| Gdk::LEAVE_NOTIFY_MASK);

	set_homogeneous(false);
	set_orientation(Gtk::ORIENTATION_VERTICAL);
	
	get_style_context()->add_class("entityView");
}
entityView::~entityView()
{
	for(int i = 0; i < mHandles.size(); ++i) {
		delete mHandles[i];
	}
}

void entityView::add_handle(entityHandle* handle, int at)
{
	if(!handle->parent()) {
		handle->set_parent(&mRootHandle);
	}
	
	mHandles.push_back(handle);
	pack_start(*handle, false, true);
	reorder_child(*handle, at);
	set_visible();
}
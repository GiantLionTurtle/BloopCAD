
#include "Drawable.hpp"

#include "entityView.hpp"
#include <utils/DebugUtils.hpp>

Drawable::Drawable(): 
	mState(BLOOP_ENTITY_EXISTS_FLAG),
	mStdNotifs(0),
	mParent(nullptr),
	mHandle(nullptr),
	mNeed_redraw(true),
	mNeed_update(false),
	mInited(false),
	mType(0)
{
	mType |= Drawable_types::DRAWABLE;
}

void Drawable::init()
{
	if(need_init()) {
		init_impl();
		mInited = true;
	}
}
void Drawable::draw(Camera_ptr cam, int frame, draw_type type)
{
	if(visible() && exists()) {
		draw_impl(cam, frame, type);
		mNeed_redraw = false;
	}
}
void Drawable::update(bool force)
{
	if((need_update() || force) && exists()) {
		update_impl();
		mNeed_update = false;
	}
}

void Drawable::notify_parent(int msg)
{
	if(mParent)
		mParent->notify(this, msg, true);
}

void Drawable::set_selected(bool select_, bool silent) 
{ 	
	if(exists() && selected() != select_) {
		set_need_redraw();
		if(select_) {
			mState |= BLOOP_ENTITY_SELECTED_FLAG;
		} else {
			mState &= ~BLOOP_ENTITY_SELECTED_FLAG;
		}
		if(!silent && notif_on_selected())
			notify_parent(select_ ? SELECTED : UNSELECTED);
		select_impl(select_);
		if(mHandle)
			mHandle->set_selected(select_);
	}
}
void Drawable::select(bool silent) 
{ 
	if(exists()) // TODO: Are all these if(exists()) {} overkill?
		set_selected(true, silent);
}
void Drawable::unselect(bool silent) 
{ 
	if(exists())
		set_selected(false, silent); 
}
bool Drawable::selected() const
{ 	
	if(exists())
		return mState & BLOOP_ENTITY_SELECTED_FLAG; 
	return false;
}

void Drawable::set_hover(bool hover, bool silent) 
{ 	
	if(exists() && hovered() != hover) {
		set_need_redraw();
		if(hover) {
			mState |= BLOOP_ENTITY_HOVERED_FLAG;
		} else {
			mState &= ~BLOOP_ENTITY_HOVERED_FLAG;
		}
		if(!silent && notif_on_hover())
			notify_parent(hover ? HOVERED : UNHOVERED);
		hover_impl(hover);
		if(mHandle)
			mHandle->set_hovered(hover);
	}
}
bool Drawable::hovered() const
{ 	
	if(exists())
		return mState & BLOOP_ENTITY_HOVERED_FLAG;
	return false; 
}

void Drawable::set_hidden(bool hide, bool silent)
{
	if(exists() && hidden() != hide) {
		set_need_redraw();
		if(hide) {
			mState |= BLOOP_ENTITY_HIDDEN_FLAG;
		} else {
			mState &= ~BLOOP_ENTITY_HIDDEN_FLAG;
		}
		if(!silent && notif_on_hidden())
			notify_parent(hide ? HIDEN : UNHIDEN);
		hidden_impl(hide);
	}
}
void Drawable::hide(bool silent)
{
	set_hidden(true, silent);
}
void Drawable::show(bool silent)
{
	set_hidden(false, silent);
}
bool Drawable::hidden() const
{
	if(mParent)
		return mState & BLOOP_ENTITY_HIDDEN_FLAG || mParent->hidden() || !exists();
	return mState & BLOOP_ENTITY_HIDDEN_FLAG; 
}
bool Drawable::visible() const
{
	return !hidden();
}

void Drawable::set_exists(bool exists_, bool silent) 
{
	if(exists() != exists_) {
		set_need_redraw();
		if(exists_) {
			mState |= BLOOP_ENTITY_EXISTS_FLAG;
		} else {
			mState &= ~BLOOP_ENTITY_EXISTS_FLAG;
		}
		exists_impl(exists_);
		if(!silent && notif_on_exists())
			notify_parent(exists_ ? RESURRECTED : DELETED);
		if(mHandle)
			mHandle->set_exists(exists_);
	}
}
bool Drawable::exists() const
{
	return mState & BLOOP_ENTITY_EXISTS_FLAG;
}

bool Drawable::active() const
{
	return hovered() || selected();
}

bool Drawable::notif_on_selected()
{
	return mStdNotifs & BLOOP_ENTITY_SELECTED_NOTIF;
}
void Drawable::set_notif_on_selected(bool do_)
{
	if(do_) {
		mStdNotifs |= BLOOP_ENTITY_SELECTED_NOTIF;
	} else {
		mStdNotifs &= ~BLOOP_ENTITY_SELECTED_NOTIF;
	}
}

bool Drawable::notif_on_hover()
{
	return mStdNotifs & BLOOP_ENTITY_HOVERED_NOTIF;
}
void Drawable::set_notif_on_hover(bool do_)
{
	if(do_) {
		mStdNotifs |= BLOOP_ENTITY_HOVERED_NOTIF;
	} else {
		mStdNotifs &= ~BLOOP_ENTITY_HOVERED_NOTIF;
	}
}

bool Drawable::notif_on_hidden()
{
	return mStdNotifs & BLOOP_ENTITY_HIDDEN_NOTIF;
}
void Drawable::set_notif_on_hidden(bool do_)
{
	if(do_) {
		mStdNotifs |= BLOOP_ENTITY_HIDDEN_NOTIF;
	} else {
		mStdNotifs &= ~BLOOP_ENTITY_HIDDEN_NOTIF;
	}
}

bool Drawable::notif_on_exists()
{
	return mStdNotifs & BLOOP_ENTITY_EXISTS_NOTIF;
}
void Drawable::set_notif_on_exists(bool do_)
{
	if(do_) {
		mStdNotifs |= BLOOP_ENTITY_EXISTS_NOTIF;
	} else {
		mStdNotifs &= ~BLOOP_ENTITY_EXISTS_NOTIF;
	}
}

bool Drawable::notif_on_update()
{
	return mStdNotifs & BLOOP_ENTITY_UPDATED_NOTIF;
}
void Drawable::set_notif_on_update(bool do_)
{
	if(do_) {
		mStdNotifs |= BLOOP_ENTITY_UPDATED_NOTIF;
	} else {
		mStdNotifs &= ~BLOOP_ENTITY_UPDATED_NOTIF;
	}
}

void Drawable::set_need_redraw()
{
	// TODO: test if this check actualy saves time, is the check worth the non-jump?
	if(mParent && !mNeed_redraw) {
		mParent->set_need_redraw();
	}
	mNeed_redraw = true;
}
void Drawable::set_need_update()
{
	if(mNeed_update)
		return;
	if(mParent) {
		mParent->set_need_update();
	}
	mNeed_update = true;
}

// bool Drawable::should_draw_self(draw_type type, bool on_required)
// {
// 	bool draw_self = (visible() && !on_required);
	
// 	switch(type) {
// 	case draw_type::ALL:
// 		return draw_self || (mRequire_selfRedraw && active());
// 	case draw_type::ACTIVE:
// 		return active() && !translucid();
// 	case draw_type::INACTIVE:
// 		return draw_self && !active() && !translucid();
// 	case draw_type::TRANSLUCID:
// 		return should_draw_self(draw_type::ALL, on_required) && translucid();
// 	}
// 	return false;
// }

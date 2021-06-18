
#include "entity.hpp"

#include "entityView.hpp"
#include <utils/errorLogger.hpp>

entity::entity(): 
	mState(BLOOP_ENTITY_EXISTS_FLAG),
	mStdNotifs(0),
	mParent(nullptr),
	mHandle(nullptr),
	mRequire_redraw(false),
	mRequire_selfRedraw(false),
	mInited(false)
{

}
entity::entity(entity* parent): 
	mState(BLOOP_ENTITY_EXISTS_FLAG),
	mStdNotifs(0),
	mParent(parent),
	mHandle(nullptr),
	mRequire_redraw(false),
	mRequire_selfRedraw(false),
	mInited(false)
{

}

void entity::draw(camera_ptr cam, int frame, draw_type type /*= draw_type::ALL*/, bool on_required /*= false*/)
{
	if(exists()) { // Only draw if it exists and it is either visible or selected (if it is selected in the tree view for instance)
		if(!mInited) {
			init();
			mInited = true;
		}
		bool on_require_child = !visible();
		bool draw_self = should_draw_self(type, on_required);
		if(draw_self)
			draw_impl(cam, frame);
		if(visible() || mRequire_redraw || draw_self)
			for_each([cam, frame, type, on_require_child](entity_ptr ent) { ent->draw(cam, frame, type, on_require_child); }); // Make the call for all components
		mRequire_redraw = false;
		mRequire_selfRedraw = false;
	}
}

void entity::update()
{
	for_each([](entity_ptr ent) { ent->update(); });
	update_impl();
	if(notif_on_update())
		notify_parent(UPDATED);
}

entity_ptr entity::hovered_child(camera_ptr cam, glm::vec2 cursor_pos, std::function<bool (entity_ptr)> filter_func)
{
	entity_ptr candidate_child(nullptr);
	float min_dist = std::numeric_limits<float>::max();
	hovered_child_internal(cam, cursor_pos, candidate_child, min_dist, filter_func);
	return candidate_child;
}

void entity::notify_parent(int msg)
{
	if(mParent)
		mParent->notify(msg);
}

void entity::set_selected(bool select_) 
{ 	
	if(exists() && selected() != select_) {
		set_require_redraw();
		if(select_) {
			mState |= BLOOP_ENTITY_SELECTED_FLAG;
		} else {
			mState &= ~BLOOP_ENTITY_SELECTED_FLAG;
		}
		if(notif_on_selected())
			notify_parent(select_ ? SELECTED : UNSELECTED);
		select_impl(select_);
		if(mHandle)
			mHandle->set_selected(select_);
	}
}
void entity::select() 
{ 
	if(exists()) // TODO: Are all these if(exists()) {} overkill?
		set_selected(true);
}
void entity::unselect() 
{ 
	if(exists())
		set_selected(false); 
}
bool entity::selected() const
{ 	
	if(exists())
		return mState & BLOOP_ENTITY_SELECTED_FLAG; 
	return false;
}

void entity::set_hover(bool hover) 
{ 	
	if(exists() && hovered() != hover) {
		set_require_redraw();
		if(hover) {
			mState |= BLOOP_ENTITY_HOVERED_FLAG;
		} else {
			mState &= ~BLOOP_ENTITY_HOVERED_FLAG;
		}
		if(notif_on_hover())
			notify_parent(hover ? HOVERED : UNHOVERED);
		hover_impl(hover);
		if(mHandle)
			mHandle->set_hovered(hover);
	}
}
bool entity::hovered() const
{ 	
	if(exists())
		return mState & BLOOP_ENTITY_HOVERED_FLAG;
	return false; 
}

void entity::set_hidden(bool hide)
{
	if(exists() && hidden() != hide) {
		set_require_redraw();
		if(hide) {
			mState |= BLOOP_ENTITY_HIDDEN_FLAG;
		} else {
			mState &= ~BLOOP_ENTITY_HIDDEN_FLAG;
		}
		if(notif_on_hidden())
			notify_parent(hide ? HIDEN : UNHIDEN);
		hidden_impl(hide);
	}
}
void entity::hide()
{
	set_hidden(true);
}
void entity::show()
{
	set_hidden(false);
}
bool entity::hidden() const
{
	if(mParent)
		return mState & BLOOP_ENTITY_HIDDEN_FLAG || mParent->hidden();
	return mState & BLOOP_ENTITY_HIDDEN_FLAG; 
}
bool entity::visible() const
{
	return !hidden();
}

void entity::set_exists(bool exists_) 
{
	if(exists() != exists_) {
		set_require_redraw();
		if(exists_) {
			mState |= BLOOP_ENTITY_EXISTS_FLAG;
		} else {
			mState &= ~BLOOP_ENTITY_EXISTS_FLAG;
		}
		for_each([exists_](entity_ptr ent) { ent->set_exists(exists_); });
		exists_impl(exists_);
		if(notif_on_exists())
			notify_parent(exists_ ? RESURRECTED : DELETED);
		if(mHandle)
			mHandle->set_exists(exists_);
	}
}
bool entity::exists() const
{
	return mState & BLOOP_ENTITY_EXISTS_FLAG;
}

bool entity::active() const
{
	return hovered() || selected();
}

bool entity::notif_on_selected()
{
	return mStdNotifs & BLOOP_ENTITY_SELECTED_NOTIF;
}
void entity::set_notif_on_selected(bool do_)
{
	if(do_) {
		mStdNotifs |= BLOOP_ENTITY_SELECTED_NOTIF;
	} else {
		mStdNotifs &= ~BLOOP_ENTITY_SELECTED_NOTIF;
	}
}

bool entity::notif_on_hover()
{
	return mStdNotifs & BLOOP_ENTITY_HOVERED_NOTIF;
}
void entity::set_notif_on_hover(bool do_)
{
	if(do_) {
		mStdNotifs |= BLOOP_ENTITY_HOVERED_NOTIF;
	} else {
		mStdNotifs &= ~BLOOP_ENTITY_HOVERED_NOTIF;
	}
}

bool entity::notif_on_hidden()
{
	return mStdNotifs & BLOOP_ENTITY_HIDDEN_NOTIF;
}
void entity::set_notif_on_hidden(bool do_)
{
	if(do_) {
		mStdNotifs |= BLOOP_ENTITY_HIDDEN_NOTIF;
	} else {
		mStdNotifs &= ~BLOOP_ENTITY_HIDDEN_NOTIF;
	}
}

bool entity::notif_on_exists()
{
	return mStdNotifs & BLOOP_ENTITY_EXISTS_NOTIF;
}
void entity::set_notif_on_exists(bool do_)
{
	if(do_) {
		mStdNotifs |= BLOOP_ENTITY_EXISTS_NOTIF;
	} else {
		mStdNotifs &= ~BLOOP_ENTITY_EXISTS_NOTIF;
	}
}

bool entity::notif_on_update()
{
	return mStdNotifs & BLOOP_ENTITY_UPDATED_NOTIF;
}
void entity::set_notif_on_update(bool do_)
{
	if(do_) {
		mStdNotifs |= BLOOP_ENTITY_UPDATED_NOTIF;
	} else {
		mStdNotifs &= ~BLOOP_ENTITY_UPDATED_NOTIF;
	}
}


void entity::print(int depth)
{
	for(int i = 0; i < depth; ++i) {
		std::cout<<'\t';
	}
	std::cout<<name()<<'\n';
	for_each([&depth](entity_ptr child) { child->print(depth+1); });
}

void entity::set_require_redraw(bool self /*= true*/)
{
	// TODO: test if this check actualy saves time, is the check worth the non-jump?
	if(mParent && !mRequire_redraw && !mRequire_selfRedraw) {
		mParent->set_require_redraw(false);
	}
	if(self) {
		mRequire_selfRedraw = true;
	} else {
		mRequire_redraw = true;
	}
}

bool entity::should_draw_self(draw_type type, bool on_required)
{
	bool draw_self = (visible() && !on_required);
	
	switch(type) {
	case draw_type::ALL:
		return draw_self || (mRequire_selfRedraw && active());
	case draw_type::ACTIVE:
		return active() && !translucid();
	case draw_type::INACTIVE:
		return draw_self && !active() && !translucid();
	case draw_type::TRANSLUCID:
		return should_draw_self(draw_type::ALL, on_required) && translucid();
	}
	return false;
}

void entity::hovered_child_internal(camera_ptr cam, glm::vec2 cursor_pos, entity_ptr& candidate, float& min_dist, std::function<bool (entity_ptr)> filter_func)
{
	for_each([&](entity_ptr ent) {
		ent->hovered_child_internal(cam, cursor_pos, candidate, min_dist, filter_func);

		if(ent->visible() && filter_func(ent)) {
			float dist = ent->selection_depth(cam, cursor_pos);
			if(dist > 0.0f) {
				if((dist < (min_dist)) || (dist == min_dist && (!candidate || candidate->selection_rank() > ent->selection_rank()))) {
					candidate = ent;
					min_dist = dist;
				}
			}
		}
	});
}

std::ostream& operator<<(std::ostream& os, entity& ent)
{
	ent.print();
	return os;
}
std::ostream& operator<<(std::ostream& os, entity ent)
{
	ent.print();
	return os;
}

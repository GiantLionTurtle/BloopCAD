
#include "entity.hpp"

#include "entityView.hpp"
#include <utils/errorLogger.hpp>

entity::entity(): 
	mState(BLOOP_ENTITY_EXISTS_FLAG),
	mParent(nullptr),
	mHandle(nullptr),
	mRequire_redraw(false),
	mRequire_selfRedraw(false)
{

}
entity::entity(entity* parent): 
	mState(BLOOP_ENTITY_EXISTS_FLAG),
	mParent(parent),
	mHandle(nullptr),
	mRequire_redraw(false),
	mRequire_selfRedraw(false)
{

}

void entity::draw(camera_ptr cam, int frame, bool on_required /*= false*/)
{
	if(exists()) { // Only draw if it exists and it is either visible or selected (if it is selected in the tree view for instance)
		bool draw_self = (visible() && !on_required) || (mRequire_selfRedraw && (hovered() || selected()));
		bool on_require_child = !visible();
		if(draw_self)
			draw_impl(cam, frame);
		if(visible() || mRequire_redraw || draw_self)
			for_each([cam, frame, on_require_child](entity_ptr ent) { ent->draw(cam, frame, on_require_child); }); // Make the call for all components
		mRequire_redraw = false;
		mRequire_selfRedraw = false;
	}
}

void entity::update()
{
	for_each([](entity_ptr ent) { ent->update(); });
	update_impl();
}

entity_ptr entity::hovered_child(camera_ptr cam, glm::vec2 cursor_pos, std::function<bool (entity_ptr)> filter_func)
{
	entity_ptr candidate_child(nullptr);
	float min_dist = std::numeric_limits<float>::max();
	hovered_child_internal(cam, cursor_pos, candidate_child, min_dist, filter_func);

	return candidate_child;
}

void entity::set_selected(bool select) 
{ 	
	if(exists()) {
		set_require_redraw();
		if(select) {
			mState |= BLOOP_ENTITY_SELECTED_FLAG;
		} else {
			mState &= ~BLOOP_ENTITY_SELECTED_FLAG;
		}

		if(mHandle)
			mHandle->set_selected(select);
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
	if(exists()) {
		set_require_redraw();
		if(hover) {
			mState |= BLOOP_ENTITY_HOVERED_FLAG;
		} else {
			mState &= ~BLOOP_ENTITY_HOVERED_FLAG;
		}

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

void entity::set_hidden(bool hidden)
{
	if(exists()) {
		set_require_redraw();
		if(hidden) {
			mState |= BLOOP_ENTITY_HIDDEN_FLAG;
		} else {
			mState &= ~BLOOP_ENTITY_HIDDEN_FLAG;
		}		
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
	if(exists())
		return mState & BLOOP_ENTITY_HIDDEN_FLAG;
	return false; 
}
bool entity::visible() const
{
	return !hidden();
}

void entity::set_exists(bool exists_) 
{ 
	set_require_redraw();
	if(exists_) {
		mState |= BLOOP_ENTITY_EXISTS_FLAG;
	} else {
		mState &= ~BLOOP_ENTITY_EXISTS_FLAG;
	}		
	if(!exists()) {
		set_hover(false);
		set_selected(false);
		set_hidden(false);
	}
}
bool entity::exists() const
{
	return mState & BLOOP_ENTITY_EXISTS_FLAG;
}

void entity::add(entity_ptr elem)
{
	if(elem) {
		set_require_redraw();
		elem->mParent = this;
		mChildren.push_back(elem);
	}
}

entity_ptr entity::get(size_t ind) const
{
	return (ind < size() ? mChildren[ind] : entity_ptr(nullptr)); // Basic range check
}
entity_ptr entity::get_last() const
{
	if(mChildren.empty())
		return nullptr;
	return mChildren.back();
}

entity_ptr entity::operator[](size_t ind) const
{
	return get(ind);
}
void entity::for_each(std::function<void (entity_ptr)> func)
{
	for(size_t i = 0; i < mChildren.size(); ++i) {
		func(mChildren[i]);
	}
}

void entity::set_require_redraw(bool self /*= true*/)
{
	if(mParent) {
		mParent->set_require_redraw(false);
	} 
	if(self) {
		mRequire_selfRedraw = true;
	} else {
		mRequire_redraw = true;
	}
}

void entity::hovered_child_internal(camera_ptr cam, glm::vec2 cursor_pos, entity_ptr& candidate, float& min_dist, std::function<bool (entity_ptr)> filter_func)
{
	for(int i = 0; i < mChildren.size(); ++i) {
		mChildren[i]->hovered_child_internal(cam, cursor_pos, candidate, min_dist, filter_func);

		if(mChildren[i]->visible() && filter_func(mChildren[i])) {
			float dist = mChildren[i]->selection_depth(cam, cursor_pos);
			if(dist > 0.0f) {
				if((dist < (min_dist)) || (dist == min_dist && (!candidate || candidate->selection_rank() > mChildren[i]->selection_rank()))) {
					candidate = mChildren[i];
					min_dist = dist;
				}
			}
		} 
	}
}
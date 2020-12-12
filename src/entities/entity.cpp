
#include "entity.hpp"

entity::entity(): 
	mState(BLOOP_ENTITY_EXISTS_FLAG),
	mSelectionColor(0.0f, 0.0f, 0.0f)
{

}

void entity::draw(std::shared_ptr<camera> cam)
{
	if(exists() && (visible() || selected())) // Only draw if it exists and it is either visible or selected (if it is selected in the tree view for instance)
		draw_impl(cam);
}
void entity::draw_selection(std::shared_ptr<camera> cam)
{
	if(exists() && (visible() || selected())) // Only draw if it exists and it is either visible or selected (if it is selected in the tree view for instance)
		draw_selection_impl(cam);
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
void entity::set_selected(bool select) 
{ 	
	if(exists()) {
		if(select) {
			mState |= BLOOP_ENTITY_SELECTED_FLAG;
		} else {
			mState &= ~BLOOP_ENTITY_SELECTED_FLAG;
		}		
	}
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
		if(hover) {
			mState |= BLOOP_ENTITY_HOVERED_FLAG;
		} else {
			mState &= ~BLOOP_ENTITY_HOVERED_FLAG;
		}		
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

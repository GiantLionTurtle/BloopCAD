
#include "entity.hpp"

entity::entity(): 
	mSelected(false), 
	mHovered(false), 
	mExists(true),
	mSelectionColor(0.0f, 0.0f, 0.0f)
{

}

void entity::draw(std::shared_ptr<camera> cam)
{
	if(exists())
		draw_impl(cam);
}
void entity::draw_selection(std::shared_ptr<camera> cam)
{
	if(exists())
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
	if(exists())
		mSelected = select; 
}
bool entity::selected() const
{ 	
	if(exists())
		return mSelected; 
	return false;
}

void entity::set_hover(bool hover) 
{ 	
	if(exists())
		mHovered = hover; 
}
bool entity::hovered() const
{ 	
	if(exists())
		return mHovered;
	return false; 
}

void entity::set_exists(bool exists_) 
{ 
	mExists = exists_; 
	if(!exists()) {
		set_hover(false);
		set_selected(false);
	}
}



#include "entity.hpp"

entity::entity(): 
	mSelected(false), 
	mHovered(false), 
	mExists(true),
	mSelectionColor(0.0f, 0.0f, 0.0f), 
	mTranslation(0.0f, 0.0f, 0.0f), 
	mScale(1.0f, 1.0f, 1.0f), 
	mTransform(1.0f)
{

}

void entity::draw(std::shared_ptr<camera> cam, glm::mat4 additionalTransform)
{
	if(exists())
		draw_impl(cam, additionalTransform);
}
void entity::draw_selection(std::shared_ptr<camera> cam, glm::mat4 additionalTransform)
{
	if(exists())
		draw_selection_impl(cam, additionalTransform);
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

void entity::translate(glm::vec3 trans) 
{ 	
	if(exists())
		mTranslation += trans; 
}
void entity::scale(float sc) 
{ 	
	if(exists())
		mScale *= sc; 
}

void entity::set_translation(glm::vec3 trans) 
{ 	
	if(exists())
		mTranslation = trans; 
}
void entity::set_scale(glm::vec3 sc) 
{ 	
	if(exists())
		mScale = sc; 
}

float entity::scale() const
{ 	
	if(exists())
		return mScale.x; 
	return 0.0f;	
}
glm::vec3 entity::translation() const 
{ 	
	if(exists())
		return mTranslation; 
	return glm::vec3(0.0f);
}

glm::mat4 entity::transform() 
{	
	if(exists()) {
		mTransform = glm::translate(glm::mat4(1.0f), mTranslation) * glm::scale(glm::mat4(1.0f), mScale);	
		return mTransform; 
	}
	return glm::mat4(0.0f);
}
void entity::set_transform(glm::mat4 trans) 
{ 	
	if(exists())
		mTransform = trans; 
}
void entity::set_transform(glm::vec3 trans, glm::vec3 sc) 
{ 	
	if(exists()) {
		set_translation(trans); 
		set_scale(sc); 
		transform(); 
	}
}
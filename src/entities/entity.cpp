
#include "entity.hpp"

#include "entityView.hpp"
#include <utils/errorLogger.hpp>

entity::entity(): 
	mState(BLOOP_ENTITY_EXISTS_FLAG),
	mSelectionColor(0.0f, 0.0f, 0.0f),
	mHighestInd(0, 0, 0),
	mParent(nullptr),
	mHandle(nullptr)
{
	highestInd = new glm::ivec3(mHighestInd);
}
entity::entity(glm::ivec3 startInd): 
	mState(BLOOP_ENTITY_EXISTS_FLAG),
	mSelectionColor(0.0f, 0.0f, 0.0f),
	mHighestInd(startInd),
	mParent(nullptr),
	mHandle(nullptr)
{
	highestInd = new glm::ivec3(mHighestInd);
	set_selectionID(startInd);
}
entity::entity(entity* parent): 
	mState(BLOOP_ENTITY_EXISTS_FLAG),
	mSelectionColor(0.0f, 0.0f, 0.0f),
	mHighestInd(parent->mHighestInd),
	highestInd(parent->highestInd),
	mParent(parent),
	mHandle(nullptr)
{

}

void entity::draw(std::shared_ptr<camera> cam, int frame)
{
	if(exists() && (visible() || selected())) { // Only draw if it exists and it is either visible or selected (if it is selected in the tree view for instance)
		mRequire_redraw = false;
		draw_impl(cam, frame);
		for_each([cam, frame](std::shared_ptr<entity> ent) { ent->draw(cam, frame); }); // Make the call for all components
	}
}
void entity::draw_selection(std::shared_ptr<camera> cam, int frame)
{
	if(exists() && (visible() || selected())) {// Only draw if it exists and it is either visible or selected (if it is selected in the tree view for instance)
		draw_selection_impl(cam, frame);
		for_each([cam, frame](std::shared_ptr<entity> ent) { ent->draw_selection(cam, frame); }); // Make the call for all components
	}
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

void entity::add(std::shared_ptr<entity> elem)
{
	if(elem) {
		set_require_redraw();
		elem->set_parent(this);
		add_child(*highestInd, elem, true);
	}
}

std::shared_ptr<entity> entity::get(size_t ind) const
{
	return (ind < size() ? std::get<1>(mChildren[ind]) : std::shared_ptr<entity>(nullptr)); // Basic range check
}
std::shared_ptr<entity> entity::get(glm::ivec3 const& ind) const
{
	if(ind != glm::ivec3(0, 0, 0)) {
		// Naive binary search within our list
		int upperBound = mChildren.size(), lowerBound = 0;
		while(upperBound > lowerBound) {
			int middle_ind = (upperBound - lowerBound) / 2 + lowerBound; // The int rounding should give consistent results
			int direction = compare_indices(ind, std::get<0>(mChildren[middle_ind]));
			if(direction == 1) {
				lowerBound = middle_ind;
			} else if(direction == -1) {
				upperBound = middle_ind;
			} else {
				return std::get<1>(mChildren[middle_ind]);
			}
		}

		LOG_WARNING("Could not find children entity at index: " + glm::to_string(ind));
	}
	return std::shared_ptr<entity>(nullptr); // No entity was found
}
std::shared_ptr<entity> entity::get_last() const
{
	if(mChildren.empty())
		return nullptr;
	return std::get<1>(mChildren.back());
}

std::shared_ptr<entity> entity::operator[](size_t ind) const
{
	return get(ind);
}
std::shared_ptr<entity> entity::operator[](glm::ivec3 const& ind) const
{
	return get(ind);
}

void entity::for_each(std::function<void (std::shared_ptr<entity>, glm::ivec3)> func)
{
	for(size_t i = 0; i < mChildren.size(); ++i) {
		if(std::get<2>(mChildren[i]) && std::get<1>(mChildren[i])) // TODO: should nullptr be allowed to be given to function?
			func(std::get<1>(mChildren[i]), std::get<0>(mChildren[i]));
	}
}
void entity::for_each(std::function<void (std::shared_ptr<entity>)> func)
{
	for(size_t i = 0; i < mChildren.size(); ++i) {
		if(std::get<2>(mChildren[i]) && std::get<1>(mChildren[i])) // TODO: should nullptr be allowed to be given to function?
			func(std::get<1>(mChildren[i]));
	}
}

void entity::set_require_redraw()
{
	if(mParent) {
		mParent->set_require_redraw();
	} else {
		mRequire_redraw = true;
	}
}

void entity::set_parent(entity* parent)
{
	if(mParent) {
		// TODO: detach the entity
	}
	if(!parent) {
		*highestInd = mHighestInd;
	} else {
		highestInd = parent->highestInd;
	}
	mParent = parent;
	increment_index(*highestInd);
	set_selectionID(*highestInd);
	for_each([this](std::shared_ptr<entity> child) {child->set_parent(this);});
}

int entity::compare_indices(glm::ivec3 const& a, glm::ivec3 const& b)
{
	for(int i = 0; i < 3; ++i) { // The first elements of the indices are the more significant
		if(*(&a.x+i) > *(&b.x+i)) { // Access to the members with i, as ivec3 are just a struct with 3 ints
			return 1;
		} else if(*(&a.x+i) < *(&b.x+i)) {
			return -1;
		}
	}
	return 0;
}

void entity::increment_index(glm::ivec3 &ind)
{
	for(int i = 2; i >= 0; --i) {
		(*(&ind.x+i))++;
		if((*(&ind.x+i)) > 255) { // Roll back index after 255 (our aim is to do color thingies)
			(*(&ind.x+i)) = 0;
			continue;
		}
		break;		
	}
}

void entity::add_child(glm::ivec3 id, std::shared_ptr<entity> elem, bool first)
{
	mChildren.push_back({id, elem, first});
	if(mParent)
		mParent->add_child(id, elem);
}
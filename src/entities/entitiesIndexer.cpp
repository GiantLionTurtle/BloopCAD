
#include "entitiesIndexer.hpp"
#include <utils/errorLogger.hpp>

#include "entity.hpp"

entitiesIndexer::entitiesIndexer():
	mToFollow(nullptr),
	mHighestInd(0, 0, 0)
{}

entitiesIndexer::entitiesIndexer(glm::ivec3 startInd):
	mToFollow(nullptr),
	mHighestInd(glm::abs(startInd)) // Doesn't deal with negative indices
{}

entitiesIndexer::entitiesIndexer(std::shared_ptr<entitiesIndexer> to_follow):
	mToFollow(to_follow),
	mHighestInd(to_follow->mHighestInd)
{}

void entitiesIndexer::add(std::shared_ptr<entity> elem)
{
	if(!is_following()) { // It is alone, it has to deal with all the indexing stuff
		increment_index(mHighestInd); // Create a new index
		mEntities.push_back({mHighestInd, elem});
		if(elem) { // Operations on a nullptr are rarelly a good plan
			glm::vec3 ind_float = mHighestInd;
			ind_float /= 255.0f;
			elem->set_selectionColor(ind_float);
		}
	} else { // Let the indexer that is followed deal with the indexing 
		mToFollow->add(nullptr); // The followed should not have access to our entity, slot is reserved by pushing a nullptr
		mHighestInd = mToFollow->mHighestInd; // Take the followed's last index
		mEntities.push_back({mHighestInd, elem});
	}
}

std::shared_ptr<entity> entitiesIndexer::get(size_t ind) const
{
	return (ind < size() ? std::get<1>(mEntities[ind]) : std::shared_ptr<entity>(nullptr)); // Basic range check
}
std::shared_ptr<entity> entitiesIndexer::get(glm::ivec3 const& ind) const
{
	// Naive binary search within our list
	int upperBound = mEntities.size(), lowerBound = 0;
	while(upperBound > lowerBound) {
		int middle_ind = (upperBound - lowerBound) / 2 + lowerBound; // The int rounding should give consistent results
		int direction = compare_indices(ind, std::get<0>(mEntities[middle_ind]));
		if(direction == 1) {
			lowerBound = middle_ind;
		} else if(direction == -1) {
			upperBound = middle_ind;
		} else {
			return std::get<1>(mEntities[middle_ind]);
		}
	}
	return std::shared_ptr<entity>(nullptr); // No entity was found
}
std::shared_ptr<entity> entitiesIndexer::get_last() const
{
	if(mEntities.empty())
		return nullptr;
	return std::get<1>(mEntities.back());
}

std::shared_ptr<entity> entitiesIndexer::operator[](size_t ind) const
{
	return get(ind);
}
std::shared_ptr<entity> entitiesIndexer::operator[](glm::ivec3 const& ind) const
{
	return get(ind);
}

void entitiesIndexer::for_each(std::function<void (std::shared_ptr<entity>, glm::ivec3)> func)
{
	for(size_t i = 0; i < mEntities.size(); ++i) {
		if(std::get<1>(mEntities[i])) // TODO: should nullptr be allowed to be given to function?
			func(std::get<1>(mEntities[i]), std::get<0>(mEntities[i]));
	}
}
void entitiesIndexer::for_each(std::function<void (std::shared_ptr<entity>)> func)
{
	for(size_t i = 0; i < mEntities.size(); ++i) {
		if(std::get<1>(mEntities[i])) // TODO: should nullptr be allowed to be given to function?
			func(std::get<1>(mEntities[i]));
	}
}

int entitiesIndexer::compare_indices(glm::ivec3 const& a, glm::ivec3 const& b)
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

void entitiesIndexer::increment_index(glm::ivec3 &ind)
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
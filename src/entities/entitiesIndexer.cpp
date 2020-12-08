
#include "entitiesIndexer.hpp"
#include <utils/errorLogger.hpp>

entitiesIndexer::entitiesIndexer():
	mToFollow(nullptr),
	mHighestInd(0, 0, 0)
{}

entitiesIndexer::entitiesIndexer(glm::ivec3 startInd):
	mToFollow(nullptr),
	mHighestInd(glm::abs(startInd))
{}

entitiesIndexer::entitiesIndexer(std::shared_ptr<entitiesIndexer> to_follow):
	mToFollow(to_follow),
	mHighestInd(to_follow->mHighestInd)
{}

void entitiesIndexer::add(std::shared_ptr<entity> elem)
{
	if(!is_following()) {
		incrementIndex(mHighestInd);
		mEntities.push_back({mHighestInd, elem});
		if(elem) {
			glm::vec3 ind_float = mHighestInd;
			ind_float /= 255.0f;
			elem->setSelectionColor(ind_float);
		}
	} else {
		mToFollow->add(nullptr);
		mHighestInd = mToFollow->mHighestInd;
		mEntities.push_back({mHighestInd, elem});
	}
}

std::shared_ptr<entity> entitiesIndexer::get(size_t ind) const
{
	return (ind < size() ? std::get<1>(mEntities[ind]) : std::shared_ptr<entity>(nullptr));
}
std::shared_ptr<entity> entitiesIndexer::get(glm::ivec3 const& ind) const
{
	int upperBound = mEntities.size(), lowerBound = 0;
	while(upperBound > lowerBound) {
		int middle_ind = (upperBound - lowerBound) / 2 + lowerBound;
		int direction = compareIndices(ind, std::get<0>(mEntities[middle_ind]));
		if(direction == 1) {
			lowerBound = middle_ind;
		} else if(direction == -1) {
			upperBound = middle_ind;
		} else {
			return std::get<1>(mEntities[middle_ind]);
		}
	}

	return std::shared_ptr<entity>(nullptr);
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
		if(std::get<1>(mEntities[i]))
			func(std::get<1>(mEntities[i]), std::get<0>(mEntities[i]));
	}
}
void entitiesIndexer::for_each(std::function<void (std::shared_ptr<entity>)> func)
{
	for(size_t i = 0; i < mEntities.size(); ++i) {
		if(std::get<1>(mEntities[i]))
			func(std::get<1>(mEntities[i]));
	}
}

int entitiesIndexer::compareIndices(glm::ivec3 const& a, glm::ivec3 const& b)
{
	for(int i = 0; i < 3; ++i) {
		if(*(&a.x+i) > *(&b.x+i)) {
			return 1;
		}
		if(*(&a.x+i) < *(&b.x+i)) {
			return -1;
		}
	}
	return 0;
}

void entitiesIndexer::incrementIndex(glm::ivec3 &ind)
{
	for(int i = 2; i >= 0; --i) {
		(*(&ind.x+i))++;
		if((*(&ind.x+i)) > 255) {
			(*(&ind.x+i)) = 0;
			continue;
		}
		break;		
	}
}
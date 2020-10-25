
#ifndef ENTITIESINDEXER_HPP_
#define ENTITIESINDEXER_HPP_

#include "entity.hpp"

//#include <map>
#include <vector>
#include <memory>
#include <tuple>
#include <functional>

#include <glm/glm.hpp>

class entitiesIndexer {
private:
	std::vector<std::pair<glm::ivec3, std::shared_ptr<entity>>> mEntities;
	glm::ivec3 mHighestInd;
public:
	entitiesIndexer();
	entitiesIndexer(glm::ivec3 startInd);
	
	void add(std::shared_ptr<entity> elem);

	size_t size() const { return mEntities.size(); }

	std::shared_ptr<entity> get(size_t ind) const;
	std::shared_ptr<entity> get(glm::ivec3 const& ind) const;
	std::shared_ptr<entity> operator[](size_t ind) const;
	std::shared_ptr<entity> operator[](glm::ivec3 const& ind) const;

	void for_each(std::function<void (std::shared_ptr<entity>, glm::ivec3)> func);
	void for_each(std::function<void (std::shared_ptr<entity>)> func);
private:
	static int compareIndices(glm::ivec3 const& a, glm::ivec3 const& b);
	static void incrementIndex(glm::ivec3 &ind);
};

#endif
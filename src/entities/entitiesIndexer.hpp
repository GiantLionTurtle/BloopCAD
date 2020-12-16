
#ifndef ENTITIESINDEXER_HPP_
#define ENTITIESINDEXER_HPP_

#include <vector>
#include <memory>
#include <tuple>
#include <functional>

#include <glm/glm.hpp>

class entity; // A forward declaration of entity is needed for the pointer types

/*
	@class entitiesIndexer stores entities and gives them a unique 3 ints id
*/
class entitiesIndexer {
private:
	std::vector<std::pair<glm::ivec3, std::shared_ptr<entity>>> mEntities; // The entities container
	glm::ivec3 mHighestInd; // The current highest 3 ints index

	std::shared_ptr<entitiesIndexer> mToFollow; // The entitiesIndexer it follows if there is one (if multiple entitiesIndexer must have unique ids between them)
public:
	/*
		@function entiesIndexer creates an entitiesIndexer that starts at (0, 0, 0) and is alone
	*/
	entitiesIndexer();
	/*
		@function entitiesIndexer create an entitiesIndexer that starts at a specified index

		@param startInd : The first index of the indexer
	*/
	entitiesIndexer(glm::ivec3 startInd);
	/*
		@function entitiesIndexer create an entitiesIndexer that follows an another one

		@param to_follow : the entitiesIndexer to follow
	*/
	entitiesIndexer(std::shared_ptr<entitiesIndexer> to_follow);
	
	/*
		@function add adds an element to the indexer and increments the index

		@param elem : The element to add
	*/
	void add(std::shared_ptr<entity> elem);

	/*
		@function is the size getter

		@return : the number of entities indexed
	*/
	size_t size() const { return mEntities.size(); }

	/*
		@function get gives access to the indexer in a linear fashion

		@param ind : The linear index of the researched entity

		@return : The entity at a peculiar linear index, it it exists
	*/
	std::shared_ptr<entity> get(size_t ind) const;
	/*
		@function get gives access to the indexer at a 3 ints index

		@param ind : The 3 ints index of the researched entity

		@return : The entity at a peculiar linear index, it it exists
	*/
	std::shared_ptr<entity> get(glm::ivec3 const& ind) const;

	/*
		@function get_last returns the last element added

		@return : The last entity of the list
	*/
	std::shared_ptr<entity> get_last() const;

	/*
		@operator [] is a wrapper for the get function
	*/
	std::shared_ptr<entity> operator[](size_t ind) const;
	std::shared_ptr<entity> operator[](glm::ivec3 const& ind) const;

	/*
		@function for_each applies a function to all entities stored

		@param func : The function to apply. It takes a std::shared_ptr<entity> and optionaly a glm::ivec3 index
	*/
	void for_each(std::function<void (std::shared_ptr<entity>, glm::ivec3)> func);
	void for_each(std::function<void (std::shared_ptr<entity>)> func);

	/*
		@function is_following

		@return : Wheter or not the indexer is following another indexer
	*/
	bool is_following() const { return (bool)mToFollow; }

	void set_require_redraw(bool require);
	bool require_redraw() const;
private:
	/*
		@function compare_indices makes a comparison between two 3 ints indices

		@param a : The left side operand of the comparison
		@param b : The right side operand of the comparison

		@return : -1 if a > b, 0 if a == b, 1 if b > a
	*/
	static int compare_indices(glm::ivec3 const& a, glm::ivec3 const& b);
	/*
		@function increment_index adds one to an index

		@param ind [out] : A reference to the index to increment
	*/
	static void increment_index(glm::ivec3 &ind);
};

#endif
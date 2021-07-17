
#ifndef PART_HPP_
#define PART_HPP_

#include "Collection.hpp"
#include "Folder.hpp"
#include "tangibleEntities/Plane.hpp"
#include "tangibleEntities/Point.hpp"
#include "Sketch.hpp"

#include <gtkmm.h>

#include <memory>

class document; // The forward declaration of document is needed for pointer declaration
class modelColumns;

class PartIndexer {
private:
	std::vector<Sketch*> mSketches;

	Folder mOrigin;
public:
	/*
		NOTE: 	the indices validity is never checked within this class
				because std::vector has a proper try-catch system (if the at() function is used), 
				to not cause overhead in other parts of the code (checking for nullptr)
				and to simplify debugging
	*/
	size_t size() { return num_sketches() + 4; }
	Drawable* at(size_t ind)
	{
		if(ind < num_sketches()) {
			return mSketches.at(ind);
		} 
		ind -= num_sketches();

		if(ind < num_origin()) {
			return mOrigin.child(ind);
		}

		LOG_WARNING("Index out of bound");
		return nullptr;
	}

	size_t num_sketches() { return mSketches.size(); }
	Sketch* sketch(size_t ind) { return mSketches.at(ind); }
	void add_sketch(Sketch* g) { mSketches.push_back(g); }

	size_t num_origin() { return mOrigin.num_children(); }
	Folder& origin() { return mOrigin; }
	void add_origin(Drawable* d) { mOrigin.add(d); }
};

/*
	@class part describes a part, which is composed of sub entities and sketches
	@parent : entity
*/
class Part : public Discrete_Collection<PartIndexer> {
private:
public:
	/*
		@function part creates an empty part
	*/
	Part();

	/*
		@function init sets up the scene by :
		*creating the origin planes
	*/
	virtual void init();

	virtual void set_handle(entityHandle* handle_);

	void add_sketch(Sketch* sk);

	bool has_volume() const { return false; }

	PartIndexer& indexer() { return mDrawList; }
};

#endif
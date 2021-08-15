
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

class PartIndexer : public Indexer_abstr {
private:
	std::vector<Sketch*> mSketches;
	std::vector<Drawable*> mOrigin;

	int mInitInd_sk, mInitInd_origin;
public:
	PartIndexer(Drawable* driven):
		Indexer_abstr(driven),
		mInitInd_sk(0),
		mInitInd_origin(0)
	{

	}
	/*
		NOTE: 	the indices validity is never checked within this class
				because std::vector has a proper try-catch system (if the at() function is used), 
				to not cause overhead in other parts of the code (checking for nullptr)
				and to simplify debugging
	*/
	size_t size() { return n_sketches() + n_origin(); }
	Drawable* at(size_t ind)
	{
		if(ind < n_sketches()) {
			return mSketches.at(ind);
		} 
		ind -= n_sketches();

		if(ind < n_origin()) {
			return mOrigin.at(ind);
		}

		LOG_WARNING("Index out of bound");
		return nullptr;
	}

	bool has_newElems() { return mInitInd_origin < mOrigin.size() || mInitInd_sk < mSketches.size(); }
	void init_newElems()
	{
		init_newElems_stat(mSketches, mInitInd_sk, driven());
		init_newElems_stat(mOrigin, mInitInd_origin, driven());
	}

	size_t n_sketches() { return mSketches.size(); }
	Sketch* sketch(size_t ind) { return mSketches.at(ind); }
	void add_sketch(Sketch* g) { mSketches.push_back(g); mDriven->set_need_update(); }

	size_t n_origin() { return mOrigin.size(); }
	Drawable* origin(size_t ind) { return mOrigin.at(ind); }
	void add_origin(Drawable* d) { mOrigin.push_back(d); mDriven->set_need_update(); }
};

/*
	@class part describes a part, which is composed of sub entities and sketches
	@parent : entity
*/
class Part : public Collection_abstr<PartIndexer> {
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
	virtual void init_impl();

	virtual void set_handle(entityHandle* handle_);

	SelPoint closest(glm::vec2 cursor, Camera* cam, int filter);

	void add_sketch(Sketch* sk);

	bool has_volume() const { return false; }

	PartIndexer& indexer() { return mDrawList; }

	void show_origin();
	void hide_origin();
};

#endif
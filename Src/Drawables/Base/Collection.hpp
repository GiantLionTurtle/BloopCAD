
// BloopCAD
// Copyright (C) 2020  BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef COLLECTION_HPP_
#define COLLECTION_HPP_

#include "Drawable.hpp"

#include <Utils/Expunge.hpp>

#include <vector>
#include <array>

template<typename containerType, bool linked = false>
class Collection_abstr : virtual public Drawable {
protected:
	containerType mDrawList;	// List of Drawables that must be rendered/updated
								// they will likely be duplicate of more precise types
public:
	Collection_abstr():
		mDrawList(this)
	{
		
	}
	virtual ~Collection_abstr()
	{
		for(size_t i = 0; i < mDrawList.size(); ++i) {
			delete mDrawList.at(i);
		}
	}

	void init_impl()
	{
		mDrawList.init_newElems();
	}
	void draw(Camera* cam, int frame, draw_type type)
	{
		mNeed_redraw = false;
		if(!exists())
			return;
		draw_impl(cam, frame, type);
	}
	void draw_impl(Camera* cam, int frame, draw_type type = draw_type::ALL)
	{
		for(size_t i = 0; i < n_children(); ++i) {
			auto ch = child(i);
			child(i)->draw(cam, frame, type);
		}
	}
	virtual void graphicUpdate_impl()
	{
		if(mDrawList.has_newElems()) {
			mDrawList.init_newElems();
		}

		for(size_t i = 0; i < n_children(); ++i) {
			auto ch = child(i);
			ch->graphicUpdate(linked || need_graphicUpdate() == 2);
		}
		if(notif_on_update())
			notify_parent(UPDATED);
	}

	virtual bool active() const { return true; }

	size_t n_children() { return mDrawList.size(); }
	Drawable* child(size_t ind) { return mDrawList.at(ind); }

	void select_within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained)
	{
		for(size_t i = 0; i < n_children(); ++i) {
			child(i)->select_within(top_left, bottom_right, contained);
		}
	}
};

class Indexer_abstr {
protected:
	Drawable* mDriven; // The collection that uses this indexer
public:
	Indexer_abstr(Drawable* driven_):
		mDriven(driven_)
	{

	}
	/*
		@function driven

		@return The Collection that uses this indexer
	*/
	Drawable* driven() { return mDriven; }
};

template<typename cT, typename eT>
class Linear_indexer : public Indexer_abstr {
protected:
	cT mList;
	int mInitInd;
public:
	Linear_indexer(Drawable* driven_, cT l):
		Indexer_abstr(driven_),
		mList(l),
		mInitInd(0)
	{
		static_assert(std::is_pointer<eT>::value, "eT must be pointer type!");
	}
	Linear_indexer(Drawable* driven_):
		Indexer_abstr(driven_),
		mList(),
		mInitInd(0)
	{

	}
	size_t size() { return mList.size(); }
	eT& at(size_t ind) { return mList.at(ind); }
	cT& list() { return mList; }

	bool has_newElems() { return mInitInd < mList.size(); }
	void init_newElems()
	{
		for(int i = mInitInd; i < mList.size(); ++i) {
			at(i)->set_parent(driven());
			at(i)->init();
		}
		mInitInd = mList.size();
	}

	void set(size_t ind, eT elem)
	{
		mList.at(ind) = elem;
		if(ind < mInitInd)
			mInitInd = ind;
	}
};

template<typename eT, size_t sz>
using LinearFixed_indexer = Linear_indexer<std::array<eT, sz>, eT>;

template<typename eT>
class LinearResizable_indexer : public Linear_indexer<std::vector<eT>, eT> {
public:
	LinearResizable_indexer(Drawable* driven_, std::vector<eT> list):
		Linear_indexer<std::vector<eT>, eT>(driven_, list)
	{

	}
	LinearResizable_indexer(Drawable* driven_):
		Linear_indexer<std::vector<eT>, eT>(driven_)
	{

	}
	Drawable* driven() { return Indexer_abstr::mDriven; }
	std::vector<eT>& list() { return Linear_indexer<std::vector<eT>, eT>::mList; }
	void add(eT elem)
	{
		driven()->update();
		list().push_back(elem);
	}
};

template<typename eT>
void init_newElems_stat(std::vector<eT>& elems, int& initInd, Drawable* parent)
{
	for(int i = initInd; i < elems.size(); ++i) {
		elems.at(i)->set_parent(parent);
		elems.at(i)->init();
	}
	initInd = elems.size();
}

#endif
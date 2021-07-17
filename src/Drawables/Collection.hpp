
#ifndef COLLECTION_HPP_
#define COLLECTION_HPP_

#include "Drawable.hpp"

#include <vector>
#include <array>

template<typename containerType>
class Collection_abstr : virtual public Drawable {
protected:
	containerType mDrawList;	// List of Drawables that must be rendered/updated
								// they will likely be duplicate of more precise types
public:
	Collection_abstr() {}
	virtual ~Collection_abstr()
	{
		for(size_t i = 0; i < mDrawList.size(); ++i) {
			delete mDrawList.at(i);
		}
	}

	void draw(Camera_ptr cam, int frame, draw_type type = draw_type::ALL)
	{
		// TODO: check logic for below line
		if(mNeed_redraw && exists()) { // Only draw if it exists and it is either visible or selected (if it is selected in the tree view for instance)
			if(!mInited) {
				init();
				mInited = true;
			}
			for(size_t i = 0; i < num_children(); ++i) {
				auto ch = child(i);
				if(ch->need_redraw()) // Is this right?
					child(i)->draw(cam, frame, type);
			}
			mNeed_redraw = false;
		}
	}
		
	void init()
	{
		// Keep in mind that Drawables added after the constructor need to be
		// assigned this as parent as well through indexer class or other
		for(size_t i = 0; i < num_children(); ++i) {
			child(i)->set_parent(this);
		}	
	}

	virtual void update() = 0;
	virtual void update_self() {}

	size_t num_children() { return mDrawList.size(); }
	Drawable* child(size_t ind) { return mDrawList.at(ind); }

	void select_within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained)
	{
		for(size_t i = 0; i < num_children(); ++i) {
			child(i)->select_within(top_left, bottom_right, contained);
		}
	}
};

template<typename containerType>
class Discrete_Collection : public Collection_abstr<containerType> {
public:
	void update()
	{
		using pT = Collection_abstr<containerType>;
		update_self();
		for(size_t i = 0; i < pT::num_children(); ++i) {
			if(pT::child(i)->need_update())
				pT::child(i)->update();
		}
		if(pT::notif_on_update())
			pT::notify_parent(pT::UPDATED);
	}

	virtual void update_self() {}
};

// Collection where all siblings must be updated when one of them needs to
template<typename containerType>
class Linked_Collection : public Collection_abstr<containerType> {
public:
	virtual void update()
	{
		using pT = Collection_abstr<containerType>;
		update_self();
		for(size_t i = 0; i < pT::num_children(); ++i) {
			pT::child(i)->update();
		}
		if(pT::notif_on_update())
			pT::notify_parent(pT::UPDATED);
	}

	virtual void update_self() {}
};

/*
	Collection is useful for a dynamic bundle of Drawables
*/
template<typename type>
using Simple_Collection = Discrete_Collection<std::vector<type*>>;

/*
	FixedSize_Collection is useful for known bundles of Drawables such as lines and curves
*/
template<typename type, size_t sz>
using FixedSize_Collection = Collection_abstr<std::array<type*, sz>>;

#endif
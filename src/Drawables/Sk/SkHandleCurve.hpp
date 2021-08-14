
#ifndef SKHANDLECURVE_HPP_
#define SKHANDLECURVE_HPP_

#include "SkCurve.hpp"
#include "SkPoint.hpp"
#include <Drawables/Collection.hpp>
#include "SkGeometry.hpp"

template<size_t nH, typename Curve>
class SkHandleCurve_FixedIndexer : public Indexer_abstr {
public:
	template<typename eT>
	using HandlesTemplate = std::array<eT, nH>;

	using HandlesVec = HandlesTemplate<glm::vec2>;
	using HandlesPoint = HandlesTemplate<SkPoint*>;
	using cT = Curve;
private:
	Curve* mCurve;
	HandlesPoint mHandles;
	int mInitInd;
public:
	SkHandleCurve_FixedIndexer(Drawable* driven_, SkCurve_abstr* c, std::array<SkPoint*, nH> h):
		Indexer_abstr(driven_),
		mCurve(c),
		mHandles(h),
		mInitInd(0)
	{

	}
	SkHandleCurve_FixedIndexer(Drawable* driven_):
		Indexer_abstr(driven_),
		mCurve(nullptr),
		mInitInd(0)
	{

	}
	size_t size() { return 1 + mHandles.size(); }
	SkDrawable* at(size_t ind)
	{
		if(ind == 0) {
			return mCurve;
	 	} else {
			return mHandles.at(ind-1);
		}
	}
	Curve* curve() { return mCurve; }
	std::array<SkPoint*, nH>& handles() { return mHandles; }

	bool has_newElems() { return mInitInd < size(); }
	void init_newElems()
	{
		for(size_t i = mInitInd; i < size(); ++i) {
			at(i)->set_parent(this->driven());
			at(i)->init();
		}
		mInitInd = size();
	}

	void set_handle(size_t ind, SkPoint* elem)
	{
		mHandles.at(ind) = elem;
		if(ind+1 < mInitInd)
			mInitInd = ind+1;
	}
	void set_curve(Curve* elem)
	{
		mCurve = elem;
		if(0 < mInitInd)
			mInitInd = 0;
	}
};

// SkHandleCurve represents a curve and it's handle points
template<typename Container>
class SkHandleCurve : public SkGeometry, public Collection_abstr<Container, true> {
public:
	using pT = Collection_abstr<Container, true>;
	using pointIndexer = typename Container::HandlesVec;
	using Curve = typename Container::cT;
	SkHandleCurve(pointIndexer handles, Geom3d::Plane_abstr* pl, bool fixed_):
		SkGeometry(pl, fixed_)
	{
		pT::mDrawList.set_curve(new Curve(pl, fixed_));
		curve()->set_notif_on_selected(true);

		for(size_t i = 0; i < handles.size(); ++i) {
			auto newPt = new SkPoint(handles.at(i), pl, fixed_);
			pT::mDrawList.set_handle(i, newPt);
			pT::mDrawList.curve()->set_handle(i, newPt);
		}
	}

	size_t n_handles() { return pT::mDrawList.handles().size(); }
	SkPoint* handle(size_t ind) { return pT::mDrawList.handles().at(ind); }
	Curve* curve() { return pT::mDrawList.curve(); }

	void notify(Drawable* who, int msg, bool child)
	{
		if(!child || who != curve())
			return;

		switch(msg) {
		case RESURRECTED:
			set_exists(true);
			break;
		case DELETED:
			set_exists(false);
			break;
		case SELECTED:
			set_selected(true);
			break;
		case UNSELECTED:
			set_selected(false);
			break;
		}
	}

	SelectionPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
	{
		// it is known that the curves come before the points in mDrawList,
		// because the curve has a lower selection rank, return as soon as 
		// a selection is valid when iterating in reverse
		for(int i = pT::mDrawList.size() - 1; i >= 0; --i) {
			SelectionPoint selPt = pT::mDrawList.at(i)->closest_2d(planePos, cam, cursorPos, filter);
			if(selPt.ent)
				return selPt;
		}
		return SelectionPoint();

	}
	std::unique_ptr<DraggableSelectionPoint> closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
	{
		// it is known that the curves come before the points in mDrawList,
		// because the curve has a lower selection rank, return as soon as 
		// a selection is valid when iterating in reverse
		for(int i = pT::mDrawList.size() - 1; i >= 0; --i) {
			std::unique_ptr<DraggableSelectionPoint> selPt = 
				pT::mDrawList.at(i)->closest_2d_draggable(planePos, cam, cursorPos, filter);
			if(selPt)
				return selPt;
		}
		return nullptr;
	}
	void add_annot(SkSprite* sp) { curve()->add_annot(sp); }
	void set_annotPos(SkSprite* sp) { curve()->set_annotPos(sp); }
	void set_annotOffset(SkSprite* sp, int ind) { curve()->set_annotOffset(sp, ind); }

	std::vector<var_ptr> all_vars()
	{
		std::vector<var_ptr> vars;
		for(int i = 0; i < n_handles(); ++i) {
			auto handle_vars = handle(i)->all_vars();
			vars.insert(vars.begin(), handle_vars.begin(), handle_vars.end());
		}
		return vars;
	}
protected:
	void select_impl(bool sel)
	{
		bool set_handles = false;
		if(sel != curve()->selected()) {
			curve()->set_selected(sel, true);
			set_handles = true;
		} 
		
		if(!sel || set_handles) {
			for(size_t i = 0; i < n_handles(); ++i) {
				handle(i)->set_selected(sel, true);
			}
		}
	}
	
	void move_selected_impl(glm::vec2 start, glm::vec2 end, glm::vec2 pix_mov)
	{
		// All this logic to prevent the curve to move two times (if the curve moves then the points)
		if(curve()->selected()) {
			curve()->move(start, end, pix_mov);
		} else {
			for(size_t i = 0; i < n_handles(); ++i) {
				if(handle(i)->selected())
					handle(i)->move(start, end, pix_mov);
			}
		}
	}
};

#endif
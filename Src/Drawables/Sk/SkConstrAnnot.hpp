
#ifndef SKCONSTRANNOT_HPP_
#define SKCONSTRANNOT_HPP_

#include <ConstraintsSolver/Constraint_abstr.hpp>
#include <Drawables/Base/Drawable.hpp>
#include <Drawables/Base/Collection.hpp>
#include "SkSprite.hpp"

class SkConstrAnnot : public SkSprite, public UI_Core_Link {
private:
	SkConstrAnnot* mTwin { nullptr };
	bool mBlockSend;
public:
	SkConstrAnnot(Geom3d::Plane_abstr* pl, std::string spritePath)
		: SkSprite(pl, glm::vec2(40, 40), spritePath)
	{

	}
	void set_twin(SkConstrAnnot* tw)
	{
		mTwin = tw;
	}
	void receive(int msg, UI_Core_Link* sender)
	{
		switch(msg) {
		case UI_Core_Link::LNK_ALIVE:
			set_exists(true);
			break;
		case UI_Core_Link::LNK_DEAD:
			set_exists(false);
			break;
		}
	}
	void exists_impl(bool ex)
	{
		if(mTwin && !mBlockSend) {
			mTwin->mBlockSend = true;
			mTwin->set_exists(ex);
			mTwin->mBlockSend = false;		
		}
		send(ex ? UI_Core_Link::LNK_ALIVE : UI_Core_Link::LNK_DEAD);
	}
	void hover_impl(bool hov)
	{
		if(mTwin && !mBlockSend) {
			mTwin->mBlockSend = true;
			mTwin->set_hover(hov);
			mTwin->set_hidden(!hov);
			mTwin->mBlockSend = false;
		}
	}
	static void make_single_annot(	SkConstrAnnot*& annot, Constraint_abstr* constr, 
									Geom3d::Plane_abstr* pl, std::string const& spritePath)
	{
		annot = new SkConstrAnnot(pl, spritePath);
	
		annot->set_linkTo(constr);
		constr->set_linkTo(annot);
	}
	static void make_dual_annot(SkConstrAnnot*& annot_a, SkConstrAnnot*& annot_b, Constraint_abstr* constr, 
								Geom3d::Plane_abstr* pl, std::string const& spritePath)
	{
		annot_a = new SkConstrAnnot(pl, spritePath);
		annot_b = new SkConstrAnnot(pl, spritePath);

		annot_a->set_twin(annot_b);
		annot_b->set_twin(annot_a);

		annot_a->set_linkTo(constr);
		constr->set_linkTo(annot_a);
	}
};

#endif
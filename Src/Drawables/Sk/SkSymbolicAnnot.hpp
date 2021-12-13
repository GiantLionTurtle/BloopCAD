

// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

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

#ifndef SKSYMBOLICANNOT_HPP_
#define SKSYMBOLICANNOT_HPP_

#include <ConstrsSolver/Constraint.hpp>
#include <Drawables/Base/Drawable.hpp>
#include <Drawables/Base/Collection.hpp>
#include "SkSprite.hpp"

class SkSymbolicAnnot : public SkSprite, public UI_Core_Link {
private:
	SkSymbolicAnnot* mTwin { nullptr };
	bool mBlockSend;
public:
	SkSymbolicAnnot(Geom3d::Plane_abstr* pl, std::string spritePath)
		: SkSprite(pl, glm::vec2(40, 40), spritePath)
	{

	}
	void set_twin(SkSymbolicAnnot* tw)
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
	static void make_single_annot(	SkSymbolicAnnot*& annot, Constraint* constr, 
									Geom3d::Plane_abstr* pl, std::string const& spritePath)
	{
		annot = new SkSymbolicAnnot(pl, spritePath);
	
		annot->set_linkTo(constr);
		constr->set_linkTo(annot);
	}
	static void make_dual_annot(SkSymbolicAnnot*& annot_a, SkSymbolicAnnot*& annot_b, Constraint* constr, 
								Geom3d::Plane_abstr* pl, std::string const& spritePath)
	{
		annot_a = new SkSymbolicAnnot(pl, spritePath);
		annot_b = new SkSymbolicAnnot(pl, spritePath);

		annot_a->set_twin(annot_b);
		annot_b->set_twin(annot_a);

		annot_a->set_linkTo(constr);
		constr->set_linkTo(annot_a);
	}
};

#endif
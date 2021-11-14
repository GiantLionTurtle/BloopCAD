
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

#include "DocumentTree.hpp"

#include <Utils/Expunge.hpp>
#include "Document.hpp"

DocumentNode::DocumentNode(std::string label, bool collapsed)
	: mTree(nullptr)
	, mParent(nullptr)
	, mCollapsed(collapsed)
	, mLevel(0)
{
	set_orientation(Gtk::ORIENTATION_VERTICAL);

	mBody.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
	mCollapser.set_label("");
	mCollapser.set_padding(5, 0);
	mCollapser.set_max_width_chars(15);

	mContent.set_padding(5, 0);
	mContent.set_label(label);
	mContent.set_margin_left(5);

	mCollapserEvents.add(mCollapser);
	
	mBody.pack_start(mCollapserEvents, false, true);
	mBody.pack_start(mContent, false, true);
	
	mBodyEvents.add(mBody);

	add_events(	  Gdk::POINTER_MOTION_MASK
				| Gdk::BUTTON_PRESS_MASK
				| Gdk::BUTTON_RELEASE_MASK
				| Gdk::KEY_PRESS_MASK
				| Gdk::KEY_RELEASE_MASK
				| Gdk::STRUCTURE_MASK
				| Gdk::SCROLL_MASK
				| Gdk::ENTER_NOTIFY_MASK
				| Gdk::LEAVE_NOTIFY_MASK);
	
	
	pack_start(mBodyEvents, false, true);

	mCollapserEvents.signal_button_press_event().connect(sigc::mem_fun(*this, &DocumentNode::manage_collapse));
	
	mBodyEvents.signal_enter_notify_event().connect(sigc::mem_fun(*this, &DocumentNode::manage_hover));
	mBodyEvents.signal_leave_notify_event().connect(sigc::mem_fun(*this, &DocumentNode::manage_hover));
	mBodyEvents.signal_button_press_event().connect(sigc::mem_fun(*this, &DocumentNode::manage_selection));

	signal_realize().connect(sigc::mem_fun(*this, &DocumentNode::do_realize));

	// show_all();
}
DocumentNode::~DocumentNode()
{
	for(int i = 0; i < mChildren.size(); ++i) {
		expunge(mChildren[i]);
	}
}

void DocumentNode::add_node(DocumentNode* nd)
{
	DEBUG_ASSERT(nd, "Tried to add null node...");

	if(mChildren.empty()) {
		mCollapser.set_label("-");
	}

	nd->set_tree(tree());
	nd->set_parent(this);
	pack_start(*nd, false, true);
	
	if(mCollapsed) {
		nd->hide();
	} else {
		nd->show_all();
	}
	mChildren.push_back(nd);
}

bool DocumentNode::manage_collapse(GdkEventButton* event)
{
	if(mChildren.empty())
		return true;
	
	if(!mCollapsed) {
		collapse();
	} else {
		expend();
	}

	return true;
}
void DocumentNode::collapse()
{
	mCollapsed = true;
	mCollapser.set_label("+");
	for(int i = 0; i < mChildren.size(); ++i) {
		mChildren[i]->hide_recursive();
	}
}
void DocumentNode::expend()
{
	mCollapsed = false;
	mCollapser.set_label("-");
	for(int i = 0; i < mChildren.size(); ++i) {
		mChildren[i]->show_recursive();
	}
}

void DocumentNode::hide_recursive()
{
	hide();
	for(int i = 0; i < mChildren.size(); ++i) {
		mChildren[i]->hide_recursive();
	}
}
void DocumentNode::show_recursive()
{
	show();
	if(!mCollapsed) {
		for(int i = 0; i < mChildren.size(); ++i) {
			mChildren[i]->show_recursive();
		}
	}
}

void DocumentNode::do_realize()
{
	if(mChildren.empty())
		return;
	if(mCollapsed) {
		collapse();
	} else {
		expend();
	}
}

bool DocumentNode::manage_hover(GdkEventCrossing* event)
{
	set_hover_recursive(event->type == GDK_ENTER_NOTIFY);
	return true;
}
bool DocumentNode::manage_selection(GdkEventButton* event)
{
	set_selected(!get_style_context()->has_class("selected"));
	return true;
}
void DocumentNode::set_hover(bool hov)
{
	if(hov) {
		get_style_context()->add_class("hovered");
	} else {
		get_style_context()->remove_class("hovered");
	}
}

void DocumentNode::set_selected(bool sel)
{
	if(sel) {
		get_style_context()->add_class("selected");
	} else {
		get_style_context()->remove_class("selected");
	}
}

void DocumentNode::set_hover_recursive(bool hov)
{
	set_hover(hov);
	for(auto nd : mChildren) {
		nd->set_hover_recursive(hov);
	}
}

void DocumentNode::set_selected_recursive(bool hov)
{
	set_selected(hov);
	for(auto nd : mChildren) {
		nd->set_selected_recursive(hov);
	}
}


void DocumentNode::set_parent(DocumentNode* nd)
{
	DEBUG_ASSERT(nd, "Tried to set null parent node...");
	mLevel = nd->mLevel + 1;
	mCollapser.set_margin_left(15 + 15 * mLevel);
	mParent = nd;
}

void DocumentNode::set_tree(DocumentTree* tree_)
{
	mTree = tree_;
	for(auto nd : mChildren) {
		nd->set_tree(tree_);
	}
}


/* DRAWABLE NODES */

DocumentDrawableNode::DocumentDrawableNode(std::string str, Drawable* drw, bool collapsed)
	: DocumentNode(str, collapsed)
	, mDrw(drw)
{
	mDrw->set_handle(this);
}
DocumentDrawableNode::DocumentDrawableNode(Drawable* drw, bool collapsed)
	: DocumentNode(drw->name(), collapsed)
	, mDrw(drw)
{
	mDrw->set_handle(this);
}

void DocumentDrawableNode::set_hover(bool hov)
{
	DocumentNode::set_hover(hov);
	mDrw->set_hover(hov, false, true);
}

bool DocumentDrawableNode::manage_selection(GdkEventButton* event)
{
	DocumentNode::manage_selection(event);
	if(event->type == GDK_2BUTTON_PRESS) {
		if(tree())
			tree()->doc()->state()->currentTool->act_on_entity(mDrw);
	}
	return true;
}

void DocumentDrawableNode::set_selected(bool sel)
{
	DocumentNode::set_selected(sel);
	mDrw->set_selected(sel, false, true);
}
void DocumentDrawableNode::notify_hover(bool hov)
{
	set_hover(hov);
}
void DocumentDrawableNode::notify_select(bool sel)
{
	set_selected(sel);
}


DocumentTree::DocumentTree(Document* doc_)
	: mRoot(doc_->name())
	, mDoc(doc_)
{
	mRoot.set_tree(this);

	pack_start(mRoot, false, true);

	set_homogeneous(false);
	set_orientation(Gtk::ORIENTATION_VERTICAL);
	
	get_style_context()->add_class("DocumentTree");
}
DocumentTree::~DocumentTree()
{
	
}

void DocumentTree::add_node(DocumentNode* node)
{
	mRoot.add_node(node);
}

void DocumentTree::set_lastSelected(DocumentNode* node)
{
	mLastSelected = node;
}
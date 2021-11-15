
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

#include "Drawable.hpp"

#include <Utils/Debug_util.hpp>
#include <Utils/Expunge.hpp>

int Drawable::nDrawable = 0;

Drawable::Drawable()
	: mState(BLOOP_ENTITY_EXISTS_FLAG)
	, mParent(nullptr)
	, mUILink(nullptr)
	, mNeed_redraw(true)
	, mNeed_graphicUpdate(false)
	, mInited(false)
	, mType(0)
	, mID(nDrawable++)
{
	mType |= Drawable_types::DRAWABLE;
}
Drawable::~Drawable()
{
	expunge(mUILink);
}
void Drawable::init()
{
	if(need_init()) {
		init_impl();
		mInited = true;
	}
}
void Drawable::draw(Camera* cam, int frame, draw_type type)
{
	mNeed_redraw = false;
	if(!exists() || 
	(type == draw_type::ACTIVE && !active()) || 
	(type == draw_type::INACTIVE && (!visible() || active())))
		return;

	draw_impl(cam, frame, type);
}
void Drawable::internalUpdate()
{
	internalUpdate_impl();
}
void Drawable::graphicUpdate(bool force)
{
	if((need_graphicUpdate() || force) && exists()) {
		graphicUpdate_impl();
		mNeed_graphicUpdate = 0;
	}
}
void Drawable::update(bool forceGraphic)
{
	internalUpdate();
	set_need_graphicUpdate(forceGraphic);
}

void Drawable::notify_parent(int msg)
{
	if(mParent)
		mParent->notify(this, msg, true);
}

void Drawable::set_selected(bool select_, bool parent_silent, bool ui_silent) 
{
	if(!exists())
		return; 	
	if(selected() != select_) {
		if(select_) {
			mState |= BLOOP_ENTITY_SELECTED_FLAG;
		} else {
			mState &= ~BLOOP_ENTITY_SELECTED_FLAG;
		}
		set_need_redraw();
		if(mUILink && !ui_silent)
			mUILink->notify_select(select_);
	}
	select_impl(select_);
}
void Drawable::select(bool parent_silent, bool ui_silent)
{ 
	set_selected(true, parent_silent, ui_silent);
}
void Drawable::unselect(bool parent_silent, bool ui_silent) 
{ 
	set_selected(false, parent_silent, ui_silent);
}
bool Drawable::selected() const
{ 	
	return mState & BLOOP_ENTITY_SELECTED_FLAG; 
}

void Drawable::set_hover(bool hover, bool parent_silent, bool ui_silent) 
{ 	
	if(!exists())
		return;
	if(hovered() != hover) {
		if(hover) {
			mState |= BLOOP_ENTITY_HOVERED_FLAG;
		} else {
			mState &= ~BLOOP_ENTITY_HOVERED_FLAG;
		}
		hover_impl(hover);
		set_need_redraw();
		if(mUILink && !ui_silent)
			mUILink->notify_hover(hover);
	}
}
bool Drawable::hovered() const
{ 	
	return mState & BLOOP_ENTITY_HOVERED_FLAG;
}

void Drawable::set_hidden(bool hide, bool parent_silent)
{
	if(!exists())
		return;
	if(hidden() != hide) {
		if(hide) {
			mState |= BLOOP_ENTITY_HIDDEN_FLAG;
		} else {
			mState &= ~BLOOP_ENTITY_HIDDEN_FLAG;
		}
		hidden_impl(hide);
		set_need_redraw();
	}
}
void Drawable::hide(bool parent_silent)
{
	set_hidden(true, parent_silent);
}
void Drawable::show(bool parent_silent)
{
	set_hidden(false, parent_silent);
}
bool Drawable::hidden() const
{
	if(mParent)
		return mState & BLOOP_ENTITY_HIDDEN_FLAG || mParent->hidden() || !exists();
	return mState & BLOOP_ENTITY_HIDDEN_FLAG; 
}
bool Drawable::visible() const
{
	return !hidden();
}

void Drawable::set_exists(bool exists_, bool parent_silent) 
{
	if(exists() != exists_) {
		set_need_redraw();
		if(exists_) {
			mState |= BLOOP_ENTITY_EXISTS_FLAG;
		} else {
			mState &= ~BLOOP_ENTITY_EXISTS_FLAG;
			set_hover(false);
		}
		exists_impl(exists_);
	}
}
bool Drawable::exists() const
{
	return mState & BLOOP_ENTITY_EXISTS_FLAG;
}

bool Drawable::active() const
{
	return hovered() || selected();
}

void Drawable::print_state()
{
	std::cout<<std::boolalpha	<<"Exists:\t\t"<<exists()<<"\n"
								<<"Selected:\t"<<selected()<<"\n"
								<<"Hovered:\t"<<hovered()<<"\n"
								<<"Hidden:\t\t"<<hidden()<<"\n";
}

void Drawable::set_need_redraw()
{
	// TODO: test if this check actualy saves time, is the check worth the non-jump?
	if(mParent && !mNeed_redraw) {
		mParent->set_need_redraw();
	}
	mNeed_redraw = true;
}
void Drawable::set_need_graphicUpdate(bool force)
{
	if(mNeed_graphicUpdate && !(mNeed_graphicUpdate != 2 && force)) // This makes sure that multiple graphic updates 
		return;				// are not spawn and create callback loops + it avoids loosing the "force" parameter
	set_need_redraw();

	if(mParent) {
		mParent->set_need_graphicUpdate(force);
	}
	mNeed_graphicUpdate = force ? 2 : 1;
}

// bool Drawable::should_draw_self(draw_type type, bool on_required)
// {
// 	bool draw_self = (visible() && !on_required);
	
// 	switch(type) {
// 	case draw_type::ALL:
// 		return draw_self || (mNeed_selfRedraw && active());
// 	case draw_type::ACTIVE:
// 		return active() && !translucid();
// 	case draw_type::INACTIVE:
// 		return draw_self && !active() && !translucid();
// 	case draw_type::TRANSLUCID:
// 		return should_draw_self(draw_type::ALL, on_required) && translucid();
// 	}
// 	return false;
// }

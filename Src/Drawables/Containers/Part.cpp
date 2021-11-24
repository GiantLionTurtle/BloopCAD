
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

#include "Part.hpp"

#include <Drawables/3d/Point.hpp>
#include <Drawables/3d/Plane.hpp>
#include <Workspaces/DocumentTree.hpp>

Part::Part()
	: Drawable()
{
	set_name("Part");
	// init();
	// init_scene();
}

void Part::init_impl()
{
	// Maybe this whole function should just be in the constructor
	
	// Create the 3 planes of the origin
	auto xy = new Plane(
		Geom3d::Plane_abstr::from_1Point2Vectors(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	xy->set_name("XY plane");
	xy->set_parent(this);
	auto yz = new Plane(
		Geom3d::Plane_abstr::from_1Point2Vectors(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), true)); // Inverted due to openGL's z axis being kind of backward
	yz->set_name("YZ plane");
	yz->set_parent(this);
	auto zx = new Plane(
		Geom3d::Plane_abstr::from_1Point2Vectors(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	zx->set_name("ZX plane");
	zx->set_parent(this);

	Point* center = new Point(glm::vec3(0.0f, 0.0f, 0.0f));
	center->set_name("Origin point");

	Plane* testplane = new Plane(
		Geom3d::Plane_abstr::from_1Point2Vectors(glm::vec3(-3.0f, 0.5f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f))); // Temporary test plane

	DocumentNode* node = new DocumentNode("Origin");
	static_cast<DocumentDrawableNode*>(mUILink)->add_node(node);

	node->add_node(new DocumentDrawableNode(center));
	node->add_node(new DocumentDrawableNode(xy));
	node->add_node(new DocumentDrawableNode(yz));
	node->add_node(new DocumentDrawableNode(zx));
	node->add_node(new DocumentDrawableNode(testplane));
	node->collapse();

	

	mDrawList.add_origin(center);
	mDrawList.add_origin(xy);
	mDrawList.add_origin(yz);
	mDrawList.add_origin(zx);
	mDrawList.add_origin(testplane);

	mDrawList.init_newElems();
}

void Part::set_handle(UILink* handle_)
{
	mUILink = handle_;
	
	// for(int i = 0; i < mDrawList.size(); ++i) {
	// 	auto drw = mDrawList.at(i);
	// 	if(drw->handle())
	// 		delete drw->handle();
	// 	drw->set_handle(new UILink(drw, mHandle->view(), mHandle));
	// }
}

SelPoint Part::closest(glm::vec2 cursor, Camera* cam, int filter)
{
	int maxpriority = -1;
	float minDist = std::numeric_limits<float>::max();
	SelPoint selPt;
	for(size_t i = 0; i < mDrawList.size(); ++i) {
		Drawable* ch = mDrawList.at(i);
		if(ch->visible() && ch->selection_rank() >= maxpriority) {
			SelPoint tmpSelPt = ch->closest(cursor, cam, filter);
			if(tmpSelPt.ent && tmpSelPt.dist_to_cam < minDist) {
				selPt = tmpSelPt;
				maxpriority = tmpSelPt.ent->selection_rank();
				minDist = tmpSelPt.dist_to_cam;
			}
		}
	}
	return selPt;
}

void Part::add_sketch(Sketch* sk)
{
	if(!sk) {
		LOG_WARNING("Tried adding a null sketch.");
		return;
	}
	set_need_redraw();
	sk->set_parent(this);
	mDrawList.add_sketch(sk);
	
	if(mUILink) {
		DocumentDrawableNode* sklink = new DocumentDrawableNode(sk);
		static_cast<DocumentDrawableNode*>(mUILink)->add_node(sklink);
		sk->set_handle(sklink);
	}
}

void Part::show_origin()
{
	for(int i = 0; i < mDrawList.n_origin(); ++i) {
		mDrawList.origin(i)->show();
	}
}
void Part::hide_origin()
{
	for(int i = 0; i < mDrawList.n_origin(); ++i) {
		mDrawList.origin(i)->hide();
	}
}
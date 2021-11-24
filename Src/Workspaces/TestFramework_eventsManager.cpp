
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

#include "TestFramework_eventsManager.hpp"

#include <Bloop/Bloop.hpp>
#include <Workspaces/Document.hpp>

#include <Tools/Part/StartSketch_tool.hpp>
#include <Tools/Sketch/Line_tool.hpp>
#include <Tools/Sketch/Coincidence_tool.hpp>
#include <Tools/Sketch/Horizontality_tool.hpp>
#include <Tools/Sketch/Perpendicularity_tool.hpp>

Test_EventsManager::Test_EventsManager():
	mTestStep(-1),
	mTestStepDone(0),
	mLastAction(std::chrono::steady_clock::now())
{

}

bool Test_EventsManager::manage_key_press(GdkEventKey* event)
{
	if(event->keyval == GDK_KEY_o) {
		mTestStep++;
	}
	do_test();
	return true;
}

bool Test_EventsManager::manage_tick(const Glib::RefPtr<Gdk::FrameClock>& frame_clock)
{
	return stimuli_EventsManager::manage_tick(frame_clock);
}

void Test_EventsManager::do_test()
{
	if(mTestStep >= mTestStepDone) {
		Document* doc = mBloopWindow->currentDocument();
		if(!doc) {
			LOG_ERROR("Invalid Document");
		}
	// 	switch(mTestStep) {
	// 	case 0:
	// 		mBloopWindow->partWorkspace()->startSketch()->init();
	// 		mBloopWindow->partWorkspace()->startSketch()->start_sketch(dynamic_cast<Geom3d::Plane_abstr*>(doc->target()->indexer().origin(1)), mBloopWindow->partWorkspace()->state()->cam->state());
	// 		mTestStep = 0;
	// 		break;
	// 	case 1:
	// 		mBloopWindow->sketchWorkspace()->line()->init();
	// 		mPoints[0] = mBloopWindow->sketchWorkspace()->line()->add_point(glm::vec2(0.238092,  0.299035));
	// 		mTestStep = 1;
	// 		break;
	// 	case 2:
	// 		mPoints[1] = mBloopWindow->sketchWorkspace()->line()->add_point(glm::vec2(1.02165,  0.171687));
	// 		mLines[0] = mBloopWindow->sketchWorkspace()->line()->lastAdded();
	// 		mTestStep = 2;
	// 		break;
	// 	case 3:
	// 		mPoints[2] = mBloopWindow->sketchWorkspace()->line()->add_point(glm::vec2(1.1587,  0.799526));
	// 		mLines[1] = mBloopWindow->sketchWorkspace()->line()->lastAdded();
	// 		mTestStep = 3;
	// 		break;
	// 	case 4:
	// 		mPoints[3] = mBloopWindow->sketchWorkspace()->line()->add_point(glm::vec2(0.362863,  1.27707));
	// 		mLines[2] = mBloopWindow->sketchWorkspace()->line()->lastAdded();
	// 		mTestStep = 4;
	// 		break;
	// 	case 5:
	// 		mPoints[4] = mBloopWindow->sketchWorkspace()->line()->add_point(glm::vec2(0.387337,  0.436465));
	// 		mLines[3] = mBloopWindow->sketchWorkspace()->line()->lastAdded();
	// 		mBloopWindow->sketchWorkspace()->line()->finish();
	// 		mTestStep = 5;
	// 		break;
	// 	case 6:
	// 		mBloopWindow->sketchWorkspace()->coincidence()->init();			
	// 		mBloopWindow->sketchWorkspace()->coincidence()->add_geom(mPoints[0]);
	// 		mBloopWindow->sketchWorkspace()->coincidence()->add_geom(mPoints[4]);
	// 		mBloopWindow->sketchWorkspace()->coincidence()->add_constraint();
	// 		mBloopWindow->sketchWorkspace()->coincidence()->finish();
	// 		mTestStep = 6;
	// 		break;
	// 	case 7:
	// 		mBloopWindow->sketchWorkspace()->horizontality()->init();
	// 		mBloopWindow->sketchWorkspace()->horizontality()->add_geom(mLines[0]->curve());
	// 		mBloopWindow->sketchWorkspace()->horizontality()->add_constraint();
	// 		mBloopWindow->sketchWorkspace()->horizontality()->finish();
	// 		mTestStep = 7;
	// 		break;
	// 	case 8:
	// 		mBloopWindow->sketchWorkspace()->perpendicularity()->init();
	// 		mBloopWindow->sketchWorkspace()->perpendicularity()->add_geom(mLines[0]->curve());
	// 		mBloopWindow->sketchWorkspace()->perpendicularity()->add_geom(mLines[1]->curve());
	// 		mBloopWindow->sketchWorkspace()->perpendicularity()->add_constraint();
	// 		mBloopWindow->sketchWorkspace()->perpendicularity()->finish();
	// 		mTestStep = 8;
	// 		break;
	// 	case 9:
	// 		mBloopWindow->sketchWorkspace()->perpendicularity()->init();
	// 		mBloopWindow->sketchWorkspace()->perpendicularity()->add_geom(mLines[1]->curve());
	// 		mBloopWindow->sketchWorkspace()->perpendicularity()->add_geom(mLines[2]->curve());
	// 		mBloopWindow->sketchWorkspace()->perpendicularity()->add_constraint();
	// 		mBloopWindow->sketchWorkspace()->perpendicularity()->finish();
	// 		mTestStep = 9;
	// 		break;
	// 	case 10:
	// 		mBloopWindow->sketchWorkspace()->perpendicularity()->init();
	// 		mBloopWindow->sketchWorkspace()->perpendicularity()->add_geom(mLines[2]->curve());
	// 		mBloopWindow->sketchWorkspace()->perpendicularity()->add_geom(mLines[3]->curve());
	// 		mBloopWindow->sketchWorkspace()->perpendicularity()->add_constraint();
	// 		mBloopWindow->sketchWorkspace()->perpendicularity()->finish();
	// 		mTestStep = 10;
	// 		break;
	// 	// case 11:
	// 	// 	mPoints[2]->set_dragged(true);
	// 	// 	for(int i = 0; i < 5; ++i) {
	// 	// 		std::cout<<"Point "<<i<<": ("<<mPoints[i]->x()->id()<<",  "<<mPoints[i]->y()->id()<<")\n";
	// 	// 	}
	// 	// 	mTestStep = 11;
	// 	// 	break;
	// 	default:
	// 		if(mTestStep%2 == 1) {
	// 			mPoints[2]->move(glm::vec2(-0.05f, -0.05f));
	// 		} else {
	// 			mBloopWindow->sketchWorkspace()->target()->update_constraints(false, true);
	// 		}
	// 		break;
	// 	}
	// 	mTestStepDone++;
	// 	// if(mTestStep > 0 && duration_cast<milliseconds>(steady_clock::now() - mLastAction).count() > 100) {
	// 	// 	mLastAction = steady_clock::now();
	// 	// 	std::cout<<"test! "<<i++<<"\n";
	// 	// }
	// }
	switch(mTestStep) {
		case 0:
			mBloopWindow->partWorkspace()->startSketch()->init();
			mBloopWindow->partWorkspace()->startSketch()->start_sketch(dynamic_cast<Geom3d::Plane_abstr*>(doc->target()->indexer().origin(1)), mBloopWindow->partWorkspace()->state()->cam->state());
			mTestStep = 0;
			break;
		case 1:
			mBloopWindow->sketchWorkspace()->line()->init();
			mPoints[0] = mBloopWindow->sketchWorkspace()->line()->add_point(glm::vec2(0.5,  0.3));
			mTestStep = 1;
			break;
		case 2:
			mPoints[1] = mBloopWindow->sketchWorkspace()->line()->add_point(glm::vec2(1.0,  0.3));
			mLines[0] = mBloopWindow->sketchWorkspace()->line()->lastAdded();
			mTestStep = 2;
			break;
		case 3:
			mPoints[2] = mBloopWindow->sketchWorkspace()->line()->add_point(glm::vec2(1.0,  0.8));
			mLines[1] = mBloopWindow->sketchWorkspace()->line()->lastAdded();
			mTestStep = 3;
			break;
		// case 4:
		// 	mBloopWindow->sketchWorkspace()->horizontality()->init();
		// 	mBloopWindow->sketchWorkspace()->horizontality()->add_geom(mLines[0]->curve());
		// 	mBloopWindow->sketchWorkspace()->horizontality()->add_constraint();
		// 	mBloopWindow->sketchWorkspace()->horizontality()->finish();
		// 	mTestStep = 4;
		// 	break;
		case 4:
			mBloopWindow->sketchWorkspace()->perpendicularity()->init();
			mBloopWindow->sketchWorkspace()->perpendicularity()->add_geom(mLines[0]->curve());
			mBloopWindow->sketchWorkspace()->perpendicularity()->add_geom(mLines[1]->curve());
			mBloopWindow->sketchWorkspace()->perpendicularity()->add_constraint();
			mBloopWindow->sketchWorkspace()->perpendicularity()->finish();
			mTestStep = 4;
			break;
		default:
			if(mTestStep%2 == 1) {
				mPoints[0]->move(glm::vec2(-1.0f, -1.0f));
			} else {
				mBloopWindow->sketchWorkspace()->target()->update_constraints(false, true);
			}
			break;
		}
		mTestStepDone++;
		// if(mTestStep > 0 && duration_cast<milliseconds>(steady_clock::now() - mLastAction).count() > 100) {
		// 	mLastAction = steady_clock::now();
		// 	std::cout<<"test! "<<i++<<"\n";
		// }
	}
}


// Point: 280,  281
// Point: 33,  34

// Point: 35,  36
// Point: 44,  45

// Point: 46,  47
// Point: 129,  130

// Point: 131,  132
// Point: 278,  279


// Fixed: 44 // pt1
// Fixed: 45

// Fixed: 36

// Fixed: 46 // Pt3
// Fixed: 47

// Fixed: 34

// Fixed: 44
// Fixed: 45
// Fixed: 36


// asnda
// added 278
// added 279
// added 280
// added 281

// Fixed: 44
// Fixed: 45
// Fixed: 35
// Fixed: 36
// Fixed: 129
// Fixed: 130
// Fixed: 46
// Fixed: 47
// Fixed: 278
// Fixed: 279
// Fixed: 131
// Fixed: 132
// Fixed: 33
// Fixed: 34
// Fixed: 280
// Fixed: 281
// DL solver, fail. n: 0; e: 0.0045073; fixed: 16 of 16; ID = 0